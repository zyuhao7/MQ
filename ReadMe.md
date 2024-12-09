## 模拟 Rabbit 实现消息队列

### 项目介绍

`我们通常会把阻塞队列封装成⼀个独⽴的服务器程序, 并且赋予其更丰富的功能。 这样的服务程序我们就称为 消息队列 (Message Queue, MQ).`

### 技术涵盖
* 开发主语言:  C++
* 序列化框架: Protobuf 二进制序列化
* 网络通信: ⾃定义应⽤层协议 + muduo库:对tcp⻓连接的封装、并且使⽤epoll的事件驱动模式,实现⾼并发服务器与客⼾端
* 源数据信息数据库： SQLite3
* 单元测试框架： Gtest


