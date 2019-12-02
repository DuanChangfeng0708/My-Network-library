#include"../../TcpServer.h"
#include"../../EventLoop.h"
#include"../../InetAddr.h"
#include"../../TcpConnection.h"
#include"../../Callback1.h"
#include"../../Buffer.h"
#include<glog/logging.h>

class EchoServer
{
private:
    /* data */
    EventLoop *loop_;
    InetAddr *addr_;
    TcpServer *server_;
    
public:
    EchoServer(EventLoop *loop,InetAddr *addr)
    :loop_(loop),
    addr_(addr),
    server_(new TcpServer(loop_,addr_))
    {
        server_->setMessageCallback(
            std::bind(
                &EchoServer::testread,
                this,
                std::placeholders::_1
                ,std::placeholders::_2));
        server_->setConnectionCallback(std::bind(&EchoServer::ConnetionCallback,this,std::placeholders::_1));
        server_->start(3);
    }
    void testread(const TcpConnectionPtr& conn, Buffer& str)
    {
        std::cout<<str.retrieveAllAsString()<<"\n";
        conn.get()->send(str.retrieveAllAsString());
    }
    void ConnetionCallback(const TcpConnectionPtr& conn)
    {
       conn.get()->send("hello client\n");
    }
    
};







int main(int argc,char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr=false;
    FLAGS_colorlogtostderr=true;
    FLAGS_log_dir="./";
    FLAGS_max_log_size=1024;
    FLAGS_logbufsecs=0;
    EventLoop loop;
    InetAddr addr(9900);  
    EchoServer server(&loop,&addr);
    
    loop.loop(); 
    google::ShutdownGoogleLogging();
    sleep(2);
}