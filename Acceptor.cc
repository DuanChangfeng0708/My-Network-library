#include"Acceptor.h"
#include"InetAddr.h"
#include"Channel.h"
#include"EventLoop.h"

Acceptor::Acceptor(EventLoop *loop,InetAddr *addr)
:loop_(loop),
linstenFd_(addr->Fd_),
linstenChannel_(new Channel(addr->Fd_,loop)),
islistenning_(false)
{
    int opt=1;
    setsockopt(linstenFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    const struct sockaddr *serverAddr=addr->getSockaddr();
    if(::bind(linstenFd_,serverAddr,sizeof(*serverAddr))<0)
        perror("bind(3)\n");
    loop_->assertinLoopThread();
    loop_->updateChannel(linstenChannel_);
    linstenChannel_->setReadCallBack(std::bind(
        &Acceptor::handleRead,this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::Listen()
{
    loop_->assertinLoopThread();
    islistenning_=true;
    if(::listen(linstenFd_,10))
        perror("lisnten(2)\n");
    linstenChannel_->enabelReading();
    loop_->setLoopState(true);

}

void Acceptor::handleRead()
{
        loop_->assertinLoopThread();
        
        
        struct sockaddr_in clientaddr;
        socklen_t clientAddrLen;
        int connfd=::accept(linstenFd_,(struct sockaddr *)&clientaddr,&clientAddrLen);
        int connprot=ntohs( clientaddr.sin_port);
        std::cout<<"port: "<<connprot<<std::endl;
        InetAddr* CAddr=new InetAddr(connprot,connfd);
        if(connfd>0)
            if(newConnetionCallback_)
                newConnetionCallback_(CAddr);
            else
                ::close(connfd);

}