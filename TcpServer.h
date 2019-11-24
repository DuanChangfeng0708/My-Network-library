
#include<boost/utility.hpp>
#include<iostream>
#include<map>
#include<boost/scoped_ptr.hpp>
#include<string.h>
#include"Callback1.h"

class EventLoop;
class InetAddr;
class TcpConnection;
class Acceptor;
class EventLoopThreadPool;
/**
 * 123
*/
class TcpServer:boost::noncopyable
{
    typedef std::map<std::string,TcpConnectionPtr> connectionMap;
private:
    /* data */
    EventLoop *loop_;
    InetAddr *port_;
    MessageCallback messageCallback;
    ConnectionCallback connectionCallback;
    bool isstart_;
    connectionMap connections_;
    boost::scoped_ptr<Acceptor> acceptor_;
    const std::string name_;
    std::unique_ptr<EventLoopThreadPool> threadPool;
    void removeConn(const TcpConnectionPtr Conn);
    int MinThreadSize_;

public:
    TcpServer(EventLoop *loop,InetAddr* port);
    ~TcpServer();
    void setMessageCallback(MessageCallback cb){ messageCallback=cb;};
    void setConnectionCallback(ConnectionCallback cb){connectionCallback=cb;};
    void start(int MinThreadSize=0);       //start
    void NewConnection(const InetAddr *clientaddr);
    
};

