
#include"./Channel.h"
#include"./EventLoop.h"
#include<poll.h>
const int Channel::KNoneEvent = 0;
const int Channel::KReadEvent = POLLIN | POLLPRI;
const int Channel::KWriteEvent = POLLOUT;
Channel::Channel(int fd,EventLoop *loop)
    :loop_(loop),
    fd_(fd),
    revents_(0),
    events_(0),
    index(-1),
    isWriting_(false)
{
}
void Channel::handleEvent()
{
    if(revents_&POLLNVAL)
        std::cout<<"fd is illegal";
    if(revents_&(POLLIN|POLLPRI|POLLRDHUP))
        if(readCallback_)readCallback_();
    if(revents_&(POLLOUT))
        if(writeCallback_)writeCallback_();
    if(revents_&(POLLERR|POLLNVAL))
        if(errorCallback_)errorCallback_();
}

void Channel::setRevents(int revents)
{
    this->revents_=revents;
}
int Channel::getEvents()
{
    return this->events_;
}
Channel::~Channel() 
{
    close(fd_);
    //把自己unregister
    std::cout<<"~channel() fd="<<fd_<<"\n" ; 
}
void Channel::update()
{
    //它属于Eventloop 所以由EventLoop来设置它的回调事件
    loop_->updateChannel(this);
}
void Channel::disableAll()
{
    events_==KNoneEvent;

}
void Channel::UnregisterSelf()
{
    loop_->removeChannel(this);

    //TcpConnetion 用scope_ptr 连接的Channel TcpConnection析构Channel自动析构
    //this->~Channel();
}