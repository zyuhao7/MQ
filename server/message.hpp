#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <memory>
#include <list>

namespace mq
{
#define DATAFILE_SUBFIX ".mqd"
#define TMPFILE_SUBFIX ".mqd.tmp"
    using MessagePtr = std::shared_ptr<mq::Message>;
    class MessageMapper
    {
    public:
        MessageMapper(std::string &basedir, const std::string &qname)
            : _qname(qname)
        {
            if (basedir.back() != '/')
            {
                basedir += "/";
            }
            _datafile = basedir + qname + DATAFILE_SUBFIX;
            _tmpfile = basedir + qname + TMPFILE_SUBFIX;
            assert(FileHelper::createDirectory(basedir));
            createMsgFile();
        }
        bool createMsgFile()
        {
            bool ret = FileHelper::createFile(_datafile);
            if (ret == false)
            {
                LOG_DEBUG("创建队列文件 %s 失败", _datafile.c_str());
                return false;
            }
            return true;
        }
        void removeMsgFile()
        {
            FileHelper::removeFile(_datafile);
            FileHelper::removeFile(_tmpfile);
        }

        bool insert(MessagePtr &msg)
        {
            return insert(_datafile, msg);
        }
        bool remove(MessagePtr &msg)
        {
            msg->mutable_payload()->set_valid("0");

            std::string body = msg->payload().SerializeAsString();
            if (body.size() != msg->length())
            {
                LOG_DEBUG("消息长度不一致!");
                return;
            }
            FileHelper helper(_datafile);
            size_t fsize = helper.size();
            bool ret = helper.write(body.c_str(), fsize, body.size());
            if (ret == false)
            {
                LOG_DEBUG("向队列数据文件写入数据失败!");
                return false;
            }
            return true;
        }
        std::list<MessagePtr> gc()
        {
            std::list<MessagePtr> res;
            bool ret;
            ret = load(res);
            if (ret == false)
            {
                LOG_DEBUG("加载队列数据文件失败!");
                return res;
            }

            for (auto &msg : res)
            {
                ret = insert(_tmpfile, msg);
                if (ret == false)
                {
                    LOG_DEBUG("消息写入临时文件失败!");
                    return res;
                }
            }
            ret = FileHelper::removeFile(_datafile);
            if (ret == false)
            {
                LOG_DEBUG("删除队列数据文件失败!");
                return res;
            }
            ret = FileHelper(_tmpfile).rename(_datafile);
            {
                LOG_DEBUG("重命名临时文件失败!");
                return res;
            }
            return res;
        }

    private:
        bool load(std::list<MessagePtr> &res)
        {
            FileHelper helper(_datafile);
            size_t fsize = helper.size();
            size_t offset = 0, msg_size;
            bool ret;
            while (offset < fsize)
            {
                ret = helper.read((char *)&msg_size, offset, 4);
                if (ret == false)
                {
                    LOG_DEBUG("读取消息长度失败!");
                    return false;
                }
                offset += 4;

                std::string msg_body(msg_size, '\0');
                ret = helper.read(&msg_body[0], offset, msg_size);
                if (ret == false)
                {
                    LOG_DEBUG("读取消息体失败!");
                    return false;
                }
                offset += msg_size;

                MessagePtr msg = std::make_shared<Message>();
                msg->ParseFromString(msg_body);
                if (msg->payload().valid() == "0")
                    continue;
                res.push_back(msg);
            }
            return true;
        }
        bool insert(const std::string &filename, MessagePtr &msg)
        {
            std::string body = msg->payload().SerializeAsString();
            FileHelper helper(filename);
            size_t fsize = helper.size();
            bool ret = helper.write(body.c_str(), fsize, body.size());
            if (ret == false)
            {
                LOG_DEBUG("向队列数据文件写入数据失败!");
                return false;
            }
            msg->set_offset(fsize);
            msg->set_length(body.size());
            return true;
        }
        std::string _qname;
        std::string _datafile;
        std::string _tmpfile;
    };
    class QueueMessage
    {
    public:
        using ptr = std::shared_ptr<QueueMessage>;
        QueueMessage(std::string &basedir, const std::string &qname)
            : _qname(qname), _mapper(basedir, qname) {}

        bool recover()
        {
            _msgs = _mapper.gc();
            for (auto &msg : _msgs)
            {
                _durable_msgs.insert(std::make_pair(msg->payload().properties().id(), msg));
            }
            _valid_count = _total_count = _msgs.size();
        }

        bool insert(const BasicProperties *bp, const std::string &body, DeliverMode delivery_mode)
        {
            // 1. 构造消息对象
            MessagePtr msg = std::make_shared<Message>();
            msg->mutable_payload()->set_body(body);
            if (bp != nullptr)
            {
                msg->mutable_payload()->mutable_properties()->CopyFrom(*bp);
            }
            else
            {
                msg->mutable_payload()->mutable_properties()->set_id(UUIDHelper::uuid());
                msg->mutable_payload()->mutable_properties()->set_delivery_mode(delivery_mode);
                msg->mutable_payload()->mutable_properties()->set_routing_key("");
            }
            // 2. 判断是否需要持久化
            std::unique_lock<std::mutex> lock(_mutex);
            if (msg->payload().properties().delivery_mode() == DeliverMode::DURABLE)
            {
                msg->mutable_payload()->set_valid("1"); // 在持久化存储中表示数据有效

                // 3. 进行持久化存储
                bool ret = _mapper.insert(msg);
                if (ret == false)
                {
                    LOG_DEBUG(" %s 消息持久化存储失败!", body.c_str());
                    return false;
                }
                _valid_count++;
                _total_count++;
                _durable_msgs.insert(std::make_pair(msg->payload().properties().id(), msg));
            }
            //  4. 内存的管理
            _msgs.push_back(msg);
            return true;
        }
        MessagePtr front()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            MessagePtr msg = _msgs.front();
            _msgs.pop_front();
            _waitack_msgs.insert(std::make_pair(msg->payload().properties().id(), msg));
            return msg;
        }

        bool remove(const std::string &msg_id) // 每次删除消息后判断是否需要垃圾回收
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _waitack_msgs.find(msg_id);
            if (it == _waitack_msgs.end())
            {
                LOG_DEBUG("消息 %s 不存在!", msg_id.c_str());
                return false;
            }
            if (it->second->payload().properties().delivery_mode() == DeliverMode::DURABLE)
            {
                _mapper.remove(it->second);
                _durable_msgs.erase(msg_id);
                _valid_count--;
                gc(); // 垃圾回收
            }
            _waitack_msgs.erase(msg_id);
        }
        size_t getable_count() // 可推送
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _msgs.size();
        }
        size_t total_count()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _total_count;
        }
        size_t durable_count()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _durable_msgs.size();
        }
        size_t waitack_count()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _waitack_msgs.size();
        }
        void clear()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _mapper.removeMsgFile();
            _msgs.clear();
            _durable_msgs.clear();
            _waitack_msgs.clear();
            _valid_count = 0;
            _total_count = 0;
        }

    private:
        bool GCCheck()
        {
            // 持久化的消息总量大于 2000, 且有效比例低于 50%  则需要持久化
            if (_durable_msgs.size() > 2000 && _valid_count * 100 / _durable_msgs.size() < 50)
            {
                return true;
            }
            return false;
        }
        void gc()
        {
            if (!GCCheck())
                return;
            auto msgs = _mapper.gc();
            for (auto &msg : msgs)
            {
                auto it = _durable_msgs.find(msg->payload().properties().id());
                if (it != _durable_msgs.end())
                {
                    LOG_DEBUG("垃圾回收后, 有一条持久化消息, 在内存中没有管理");
                    _msgs.push_back(msg);
                    _durable_msgs.insert(std::make_pair(msg->payload().properties().id(), msg));
                    continue;
                }
                // 更新每一条消息的实际存储位置
                it->second->set_offset(msg->offset());
                it->second->set_length(msg->length());
            }
            // 更新有效消息数量
            _valid_count = msgs.size();
        }

    private:
        std::mutex _mutex;
        std::string _qname;
        size_t _valid_count;
        size_t _total_count;
        MessageMapper _mapper;
        std::list<MessagePtr> _msgs;
        std::unordered_map<std::string, MessagePtr> _durable_msgs; // 持久化消息
        std::unordered_map<std::string, MessagePtr> _waitack_msgs; // 待确认消息
    };
    class MessageManager
    {
    public:
        using ptr = std::shared_ptr<MessageManager>;
        MessageManager(const std::string &basedir)
            : _basedir(basedir) {}
        void clear()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            for (auto &it : _queue_msgs)
            {
                it.second->clear();
            }
            _queue_msgs.clear();
        }
        
        void initQueueMessage(const std::string &qname)
        {
            QueueMessage::ptr qmp;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_queue_msgs.find(qname) != _queue_msgs.end())
                {
                    LOG_DEBUG("队列 %s 已经存在!", qname.c_str());
                    return;
                }
                qmp = std::make_shared<QueueMessage>(_basedir, qname);
                _queue_msgs.insert(std::make_pair(qname, qmp));
            }
            qmp->recover();
        }

        void destroyQueueMessage(const std::string &qname)
        {
            QueueMessage::ptr qmp;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                auto it = _queue_msgs.find(qname);
                if (it == _queue_msgs.end())
                {
                    LOG_DEBUG("队列 %s 不存在!", qname.c_str());
                    return;
                }
                qmp = it->second;
                _queue_msgs.erase(it);
            }

            qmp->clear();
        }

        bool insert(const std::string &qname, const BasicProperties *bp, const std::string &body, DeliverMode delivery_mode)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("Insert Failed, 队列 %s 不存在!", qname.c_str());
                return false;
            }
            return it->second->insert(bp, body, delivery_mode);
        }
        MessagePtr front(const std::string &qname)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("获取队首消息失败, 队列 %s 不存在!", qname.c_str());
                return nullptr;
            }
            return it->second->front();
        }
        void ack(const std::string &qname, const std::string &msg_id)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("确认队列%s消息%s失败, 没有找到消息管理句柄!", qname.c_str(), msg_id.c_str());
                return;
            }
            it->second->remove(msg_id);
        }
        size_t getable_count(const std::string &qname)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("获取队列 %s 待推送消息数量失败:没有找到消息管理句柄!", qname.c_str());
                return 0;
            }
            return it->second->getable_count();
        }
        size_t total_count(const std::string &qname)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("队列 %s 不存在!", qname.c_str());
                return 0;
            }
            return it->second->total_count();
        }

        size_t waitack_count(const std::string &qname)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _queue_msgs.find(qname);
            if (it == _queue_msgs.end())
            {
                LOG_DEBUG("队列 %s 不存在!", qname.c_str());
                return 0;
            }
            return it->second->waitack_count();
        }

    private:
        std::mutex _mutex;
        std::string _basedir;
        std::unordered_map<std::string, QueueMessage::ptr> _queue_msgs;
    };
}