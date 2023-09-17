/*
    muduo 网络库给用户提供了两个主要类
    tcpserver:编写服务器程序
    tcpclient:同与编写客户端程序

    epopp+线程池
    好处：能把网络I/O的代码和业务代码区分开
                                                用户的连接和断开
   用户的可读写事件


*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
/*
    基于muduo网络库开发服务器程序
    1.组合TcpServer对象
    2.创建EventLoop事件循环对象的指针
    3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
    4.在n档期啊服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
    5.设置合适的服务端线程数量，muduo库会自己分配I/Ohn先线程和workeric线程

*/
class ChatServer
{
public:
    ChatServer(EventLoop* loop,               //事件循环
               const InetAddress& listenAddr, // IP+PORT
               const string& nameArg)         //服务器名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        //给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(
            std::bind(&ChatServer::onConnetcion, this, _1));
        //给服务器注册用户读写事件回
        _server.setMessageCallback(
            std::bind(&ChatServer::onMessage, this, _1, _2, _3));
        //设置服务器端的回调事件1个io线程，3个worker线程
        _server.setThreadNum(4);
    }
    //开启事件循环
    void start() { _server.start(); }

private:
    //专门处理用户的连接  epoll listenfd accept
    void onConnetcion(const TcpConnectionPtr& conn)
    {
        if (conn->connected()) {
            cout << conn->peerAddress().toIpPort() << "->"
                 << conn->localAddress().toIpPort() << "state:online" << endl;
        }
        else {
            cout << conn->peerAddress().toIpPort() << "->"
                 << conn->localAddress().toIpPort() << "state:offline" << endl;
            conn->shutdown();
            //_loop->quit();
        }
    }
    //专门处理用户读写事件的
    void onMessage(const TcpConnectionPtr& conn, //表示连接
                   Buffer* buffer, Timestamp time)
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }                  //时间信息
    TcpServer _server; // 1定义一个server
    EventLoop* _loop;  // 2epoll 事件循环指针
};

int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // listenfd epoll_ctl=>epoll
    loop.loop(); //调用epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件。

    return 0;
}