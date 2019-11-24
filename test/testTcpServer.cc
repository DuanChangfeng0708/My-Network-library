#include"../TcpServer.h"
#include"../EventLoop.h"
#include"../InetAddr.h"
#include"../TcpConnection.h"
#include"../Callback1.h"
#include"../Buffer.h"
class testTcpServer
{
private:
    /* data */
    EventLoop *loop_;
    InetAddr *addr_;
    TcpServer *server_;
    
public:
    testTcpServer(EventLoop *loop,InetAddr *addr)
    :loop_(loop),
    addr_(addr),
    server_(new TcpServer(loop_,addr_))
    {
        server_->setMessageCallback(
            std::bind(
                &testTcpServer::testread,
                this,
                std::placeholders::_1
                ,std::placeholders::_2));
        server_->setConnectionCallback(std::bind(&testTcpServer::ConnetionCallback,this,std::placeholders::_1));
        server_->start(3);
    }
    void testread(const TcpConnectionPtr conn, Buffer str)
    {
        std::cout<<str.retrieveAllAsString()<<"\n";
        conn.get()->send(str.retrieveAllAsString());
    }
    void ConnetionCallback(TcpConnectionPtr conn)
    {
       conn.get()->send("hello client\n");
    }
    
};







int main()
{
    EventLoop loop;
    InetAddr addr(9900);  
    testTcpServer server(&loop,&addr);
    
    loop.loop(); 
}