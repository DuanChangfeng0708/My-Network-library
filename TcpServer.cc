#include"./TcpServer.h"
#include"./Acceptor.h"
#include"./TcpConnection.h"
#include"./Channel.h"
#include"./InetAddr.h"
#include"./EventLoop.h"
#include<arpa/inet.h>
#include"./EventLoopThread.h"
#include"./EventLoopThreadPool.h"

TcpServer::TcpServer(EventLoop *loop,InetAddr* port)
    :loop_(loop),
    port_(port),
    isstart_(false),
    acceptor_(new Acceptor(loop,port)),
    name_("Client"),
    threadPool(new EventLoopThreadPool(loop_)),
    MinThreadSize_(0)
{
    acceptor_->setNewConnetionCallback(
       std::bind(&TcpServer::NewConnection,this,std::placeholders::_1)
    );
}
void TcpServer::NewConnection(const InetAddr *clientaddr)
{
    char str[INET_ADDRSTRLEN];
    const struct sockaddr_in* client=(const struct sockaddr_in *)clientaddr->getSockaddr();
    std::cout<<"client : "<<inet_ntop(AF_INET,&(client->sin_addr.s_addr),str,sizeof str)<<" \n";
    char buf[32];
    snprintf(buf,sizeof buf,"#%d",clientaddr->Fd_);
    std::string ClientName_= name_ + buf;

    // 待加入threadpoll
    EventLoop* IOloop=loop_;
    if(MinThreadSize_!=0)
    {
        IOloop=threadPool->getNextLoop();
    }
    //EventLoop * IOloop=loop_;

    TcpConnectionPtr tcpConnection(new TcpConnection(ClientName_,IOloop,port_,clientaddr));
    //TcpConnection *tcpConnetion=new TcpConnection(loop_,port_,clientaddr);
    tcpConnection->setMessageCallback(messageCallback);
    tcpConnection->setConnectionCallback(connectionCallback);
    tcpConnection->setcloseCallback(std::bind(&TcpServer::removeConn,this,std::placeholders::_1));
    //将tcpConnetion 加入map
    connections_[ClientName_]=tcpConnection;
    IOloop->RunInLoopThread(std::bind(&TcpConnection::ConnetEstablished,tcpConnection.get()));
    	                
    
}
void TcpServer::start(int MinThreadSize)
{
    std::cout<<"Tcpserver start "<<std::endl;
    ::bind(port_->Fd_,port_->getSockaddr(),sizeof(port_->getSockaddr()));
   
    if(MinThreadSize!=0)
    {
        MinThreadSize_=MinThreadSize;
        threadPool->start(MinThreadSize_);
    }
    
    loop_->RunInLoopThread(std::bind(&Acceptor::Listen,acceptor_.get()));
}

void TcpServer::removeConn(const TcpConnectionPtr conn)
{
    connections_.erase(conn->getName());

    conn.get()->getIOloop()->RunInLoopThread(std::bind(&TcpConnection::UnregisterChannel,conn));
    //连接关闭 有一个线程空闲了
    this->threadPool.get()->handleClientExit();
}
TcpServer::~TcpServer()
{

}