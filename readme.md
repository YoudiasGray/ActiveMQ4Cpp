---
title: ActiveMQWithCpp
author: Youdias
date: 2021-06-10 15:29:26
tags:
  - ActiveMQ
  - C++
  - VS2019
  - win32
---

## ActiveMQ简介

Apache下的一个子项目，一个消息中间件，支持点对点和订阅发布模式，支持多种语言客户端如C++，Java，.Net，Python，php，ruby等。

## ActiveMQ C++ VS2019 win32编译

目前需要实现订阅获取数据和发布数据两个需求，目前网络上看到的都是多多少少有些问题，需要逐个解决。这里整理下处理过程以及提供完整的可以直接编译的工程文件，有需要可以直接下载验证修改使用。

### 1 下载启动ActiveMQServcer

下载地址：https://activemq.apache.org/components/classic/download/

选择Windows  [apache-activemq-5.16.2-bin.zip](https://www.apache.org/dyn/closer.cgi?filename=/activemq/5.16.2/apache-activemq-5.16.2-bin.zip&action=download)

可以根据需要启动ActiveMQ，如bin/win64/activemq.bat，可以通过http://127.0.0.1:8161/  admin/admin验证，也可以通过这里对指定Topic发送数据

需要jre环境。

### 2 编译apr库

http://apr.apache.org/download.cgi [下载APR17.0]( https://downloads.apache.org//apr/apr-1.7.0-win32-src.zip)

解压后用vs2019打开apr.dsw，按照提示确认升级，然后将解决方案另存为apr_2019.sln。

需要对版本进行修改：

 apr-1.7.0-win32-src\apr-1.7.0\include\apr.hw line89将

```
#define _WIN32_WINNT 0x0501
```

修改为

```
#define _WIN32_WINNT 0x0601
```

工程已经设置好依赖关系，直接生成libapr，会自动编译依赖，最终生成libapr-1.dll和libapr-1.lib

### 3 下载编译activemq-cpp

去https://downloads.apache.org/activemq/activemq-cpp/3.9.5/ 下载activemq-cpp-library-3.9.5-src.zip。

首先将apr的include拷贝到main目录下，可以改名apr_include。

用vs2019打开activemq-cpp.sln，然后另存为activemq-cpp_2019.sln。设置启动项目为当前选择项，只需要编译example，会自动编译cpp，直接编译会报一堆错误，需要将apr_include添加到附加包含目录中，在原有基础上添加“../src/main/apr_include;”。修改目标文件名为libactivemq-cpp。编译生成libactivemq-cpp.lib和libactivemq-cpp.dll。

参考：

https://blog.csdn.net/lishenluo/article/details/107864758

## ActiveMQC++ 使用与验证

### 编译activemq-cpp-example

如果没有编译activemq-cpp，直接编译example会自动编译得到libactivemq-cpp.lib。

原有example已经创建了一个生产者和消费者，同时发送2000条数据。

可以进行修改，在原有处理之外插入一段代码：

```

    string message = "";
    string topic = "";
    while (true)
    {
        Sleep(200);
        cout << "input message and topic:";
        cin >> message >> topic;
        producer.SendMessage(message, topic);
    }
```

同时为生产者添加如下代码：

```

    void SendMessage(string message, string topic)
    {
        try {

            // Create a ConnectionFactory
            auto_ptr<ConnectionFactory> connectionFactory(
                ConnectionFactory::createCMSConnectionFactory(brokerURI));

            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();

            // Create a Session
            if (this->sessionTransacted) {
                session = connection->createSession(Session::SESSION_TRANSACTED);
            }
            else {
                session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
            }

            // Create the destination (Topic or Queue)
            if (useTopic) {
                destination = session->createTopic(topic);
            }
            else {
                destination = session->createQueue(topic);
            }

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer(destination);
            producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

            // Create the Thread Id String
            string threadIdStr = Long::toString(Thread::currentThread()->getId());

            // Create a messages
            string text = message + threadIdStr;
            std::auto_ptr<TextMessage> message(session->createTextMessage(text));
            //message->setIntProperty("Integer", ix);
            printf("Sent message   from thread %s\n",   threadIdStr.c_str());
            producer->send(message.get());
            //for (int ix = 0; ix < numMessages; ++ix) {
            //    std::auto_ptr<TextMessage> message(session->createTextMessage(text));
            //    message->setIntProperty("Integer", ix);
            //    printf("Sent message #%d from thread %s\n", ix + 1, threadIdStr.c_str());
            //    producer->send(message.get());
            //}

        }
        catch (CMSException& e) {
            e.printStackTrace();
        }
    }

```

实现在原有业务处理完成后可以持续接收订阅的TEST.FOO主题数据，也可以通过输入消息内容和主题向指定topic发送内容。

## 其他

1 目前主要只是实现编译通过以及稍微修改用于测试发送和接受，没有进一步的封装，也没有真正学习使用。

2 i7-8700CPU编译example需要十几分钟，太慢了。

3 编译example的时候目前做法是将lib和dll放到了vs2010-build目录，vs2010-build\Win32\Debug目录，\vs2010-build\Win32\Debug\activemq-cpp-example目录，实际上应该不需要每次都放，但是编译一次时间太长，目前确认这么放可以编译通过，哪个是必须的待测试。

4 如果只是使用实验，也可以直接下载整理过的可编译工程，地址：
https://github.com/YoudiasGray/ActiveMQ4Cpp


