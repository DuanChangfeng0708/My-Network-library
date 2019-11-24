#include"./EventLoop.h"
#include"./Poller.h"
#include"./Channel.h"
#include"MutexLockGrard.h"
#include"MutexLock.h"
#include<sys/eventfd.h>
#include<functional>
#include<errno.h>

EventLoop::EventLoop()
    :looping_(false),
    threadId_(pthread_self()),
    poller_(new Poller(this)),
    callQueueFunc(false),
    WeakUpFd_(CreatEventFd()),
    WeakUpChannel_(new Channel(WeakUpFd_,this)),
    mutex_(new MutexLock())
{
    std::cout<<"EventLoop: "<<this<<" be created in "<<threadId_<<" thread"<<std::endl;
    WeakUpChannel_->setReadCallBack(
        std::bind(&EventLoop::handleWeadupFdEvent,this)
    );
    WeakUpChannel_->enabelReading();
};
/*
  eventloop的真正业务 获得当前监听fd的活动事件 并调用其相应事件处理函数
*/
void EventLoop::loop()
{
    //assert(!looping_);
    assertinLoopThread();
    //looping_=true;
    this->quit_=false;
 
    while(!quit_){
        activityChannels.clear();
        poller_->poll(&activityChannels);
        for(std::vector<Channel*>::const_iterator it=activityChannels.begin();
        it!=activityChannels.end();
        it++)
        {
            (*it)->handleEvent();
        }
        doQueueFunc();
    }
};
void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->getOwnerLoop()==this);
    assertinLoopThread();
    poller_->updateChanels(channel);
}

void EventLoop::RunInLoopThread(const Functor cb)
{
    if(isInLoopThread())
        cb();
    else
        QueueInLoop(std::move(cb));
    
}
void EventLoop::QueueInLoop(const Functor cb)
{
    {
        MutexLockGrard lock(*mutex_);
        FuncQueue.push_back(std::move(cb));
    }
    if(!isInLoopThread()||callQueueFunc)
        weakUp();
}

void EventLoop::weakUp()
{
    //激活poll(3)
    uint64_t WeakMessage=2;
    int writelen =::write(WeakUpFd_,&WeakMessage, sizeof WeakMessage);
    if(writelen<0)
    {
        perror("weakUp write\n");
    }
}

void EventLoop::doQueueFunc()
{
    std::vector<Functor> functors;
    callQueueFunc=true;
    {
        MutexLockGrard lock(*mutex_);
        functors.swap(FuncQueue);
    }
    callQueueFunc=false;
    /*for(const Functor &f :functors)
        f();*/
    std::vector<Functor>::const_iterator it=functors.begin();
    for(;it!=functors.end();it++)
        (*it)();
    
}
int EventLoop::CreatEventFd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        std::cout << "Failed in eventfd"<<std::endl;
        abort();
    }
    return evtfd;
}

void EventLoop::quit()
{
    quit_=true;
    if(!isInLoopThread())
        weakUp();
}
void EventLoop::handleWeadupFdEvent()
{
    int64_t one;
    int nrd=::read(WeakUpFd_,&one,sizeof one);
    if(nrd<0)
        std::cout<<"error in EventLoop::handleWeadupFdEvent()"<<std::endl;
    
    
    
}
EventLoop::~EventLoop()
{
    ;
}

void EventLoop::removeChannel(Channel * channel)
{
    assert(isInLoopThread());
    poller_->removeChannel(channel);
}

