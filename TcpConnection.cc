#include"TcpConnection.h"
#include"Channel.h"
#include"EventLoop.h"
#include"InetAddr.h"
#include"Buffer.h"

TcpConnection::TcpConnection(const std::string name,EventLoop *loop,const InetAddr *thisAddr,const InetAddr *peerAddr)
    :name_(name),
    thisAddr_(thisAddr),
    peerAddr_(peerAddr),
    state_(Kconnecting),
    loop_(loop),
    inputBuffer_(new Buffer()),
    outputBuffer_(new Buffer())
{
    std::cout<<"TcpConnection\n";
    clientChannel_.reset(new Channel(peerAddr->Fd_,loop_));
    clientChannel_->setReadCallBack(std::bind(&TcpConnection::handleRead,this));
    clientChannel_->setWriteCallBack(std::bind(&TcpConnection::handleWrite,this));
    std::cout<<inputBuffer_->getReadableSize()<<"----------\n";
    std::cout<<inputBuffer_->getWriteableSize()<<"-***********\n";
    std::cout<<outputBuffer_->getReadableSize()<<"----------\n";
    std::cout<<outputBuffer_->getWriteableSize()<<"-***********\n";
}

void TcpConnection::handleRead()
{
    /*char * buf=new char[BUFSIZ];
    int nread=read(clientChannel_->fd_,buf,sizeof buf);
    */
    //改用inputBuffer读取数据
    int nread=inputBuffer_->readFd(clientChannel_->fd_);
    if(nread==0)//对方关闭连接
        handleClose();
    else if(nread>0)
        messageCallback_(shared_from_this(), *inputBuffer_);
    else
        handleError();
    
}
TcpConnection::~TcpConnection(){
    loop_->quit();
    std::cout<<"~TcpConnection()\n";
};
void TcpConnection::ConnetEstablished()
{
    loop_->isInLoopThread();
    assert(state_==Kconnecting);
    setstate(Kconnected);
    clientChannel_->enabelReading();
    connetionCallBack_(shared_from_this());
}
void TcpConnection::handleClose()
{
    loop_->assertinLoopThread();
    assert(state_==Kconnected);
    clientChannel_->disableAll();// 删除本对象
    closeCallback(shared_from_this());
}
void TcpConnection::handleError()
{
    std::cout<<"handleError\n";
}
void TcpConnection::UnregisterChannel()
{
    clientChannel_->UnregisterSelf();
    //当它不监听客户端SOCKET则认为IOloop不再loop了（实际上还在）
    loop_->setLoopState(false);
}

void TcpConnection::send(const std::string &message)
{
    assert(state_==Kconnected);
    if(loop_->isInLoopThread())
        sendInLoop(message);
    else
        loop_->RunInLoopThread(std::bind(&TcpConnection::sendInLoop,this,message));
}

void TcpConnection::sendInLoop(const std::string &message)
{
    assert(loop_->isInLoopThread());
    //assert(!(clientChannel_.get()->getWriteState()));
    
    //ssize_t writenLen=::write(peerAddr_->Fd_,message.data(),message.size());
    //改用outputBuffer;

    outputBuffer_->append(message);
    int writenLen=::write(peerAddr_->Fd_,message.c_str(),message.size());
    if(writenLen<0)
        std::cout<<"write error\n";
    else if(writenLen<message.size())   //由于某种原因没有发送完
    {
        std::string buf(message.c_str()+writenLen);
        outputBuffer_->append(buf);
        clientChannel_.get()->enableWrite();
    }
    else if(writenLen==message.size()&&clientChannel_.get()->getWriteState())
    {
        //writeindex==readIndex
        assert(outputBuffer_->getReadableSize()==0);
        clientChannel_->disableWrite();
    }
}
void TcpConnection::handleWrite()
{
    assert(loop_->isInLoopThread());
    assert((clientChannel_.get()->getWriteState()));
    sendInLoop(outputBuffer_->retrieveAllAsString());
}