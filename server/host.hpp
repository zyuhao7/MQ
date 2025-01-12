#ifndef __HOST_H__
#define __HOST_H__
#include "exchange.hpp"
#include "message.hpp"
#include "binding.hpp"
#include "queue.hpp"

namespace mq
{
    class VirtualHost
    {
    public:
        using ptr = std::shared_ptr<VirtualHost>;
        VirtualHost(const std::string &hname, const std::string &basedir, const std::string &dbfile)
            : _host_name(hname),
              _emp(std::make_shared<ExchangeManager>(dbfile)),
              _mqmp(std::make_shared<MsgQueueManager>(dbfile)),
              _bmp(std::make_shared<BindManager>(dbfile)),
              _mmp(std::make_shared<MessageManager>(basedir))
        {
            QueueMap qm = _mqmp->getAllQueues();
            for (auto &it : qm)
            {
                _mmp->initQueueMessage(it.first);
            }
        }

        bool declareExchange(const std::string &name, ExchangeType type,
                             bool durable, bool auto_delete,
                             google::protobuf::Map<std::string, std::string> &args)
        {
            return _emp->declareExchange(name, type, durable, auto_delete, args);
        }

        void deleteExchange(const std::string &name)
        {
            // 删除交换机需要将交换机绑定信息删除
            _bmp->removeExchangeBindings(name);
            _emp->deleteExchange(name);
        }

        bool existsExchange(const std::string &name)
        {
            return _emp->exists(name);
        }

        bool declareQueue(const std::string &qname, bool qdurable, bool qexclusive, bool qauto_delete, google::protobuf::Map<std::string, std::string> &qargs)
        {
            // 初始化队列的消息句柄
            _mmp->initQueueMessage(qname);
            return _mqmp->declareQueue(qname, qdurable, qexclusive, qauto_delete, qargs);
        }
        void deleteQueue(const std::string &qname)
        {
            // 删除队列需要将队列的消息句柄删除
            _mmp->destroyQueueMessage(qname);
            _bmp->removeMsgQueueBindings(qname);
            _mqmp->deleteQueue(qname);
        }

        bool existsQueue(const std::string &qname)
        {
            return _mqmp->exists(qname);
        }

        bool bind(const std::string &ename, const std::string &qname, const std::string &binding_key)
        {
            Exchange::ptr exp = _emp->selectExchange(ename); // 获取交换机对象
            if (exp == nullptr)
            {
                LOG_DEBUG("交换机 %s 不存在!", ename.c_str());
                return false;
            }
            MsgQueue::ptr mqp = _mqmp->selectQueue(qname); // 获取队列对象
            if (mqp == nullptr)
            {
                LOG_DEBUG("队列 %s 不存在!", qname.c_str());
                return false;
            }
            return _bmp->bind(ename, qname, binding_key, exp->durable && mqp->durable);
        }
        void unBind(const std::string &ename, const std::string &qname)
        {
            _bmp->unBind(ename, qname);
        }
        msgQueueBindingMap ExchangeBindings(const std::string &ename)
        {
            return _bmp->getExchangeBindings(ename);
        }

        bool existsBinding(const std::string &ename, const std::string &qname)
        {
            return _bmp->exists(ename, qname);
        }

        bool basicPublish(const std::string &qname, const BasicProperties *bp, const std::string &body)
        {
            MsgQueue::ptr mqp = _mqmp->selectQueue(qname);
            if (mqp == nullptr)
            {
                LOG_DEBUG("发布消息失败,队列 %s 不存在!", qname.c_str());
                return false;
            }
            return _mmp->insert(qname, bp, body, mqp->durable);
        }
        MessagePtr basicConsume(const std::string &qname)
        {
            return _mmp->front(qname);
        }
        void basicAck(const std::string &qname, const std::string &msg_id)
        {
            return _mmp->ack(qname, msg_id);
        }
        void clear()
        {
            _emp->clear();
            _mqmp->clear();
            _bmp->clear();
            _mmp->clear();
        }

    private:
        std::string _host_name;
        ExchangeManager::ptr _emp;
        MsgQueueManager::ptr _mqmp;
        BindManager::ptr _bmp;
        MessageManager::ptr _mmp;
    };
}
#endif