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
}