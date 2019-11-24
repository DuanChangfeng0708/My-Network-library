#include"EventLoopThreadPool.h"
#include"EventLoop.h"
#include"EventLoopThread.h"
#include"MutexLock.h"
#include"MutexLockGrard.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop)
    :loop_(loop),
    nextLoopIndex(0),
    LiveThreadCount(0),
    mutex_(new MutexLock()),
    cond_(PTHREAD_COND_INITIALIZER)
{

};

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(int ThreadCount)
{
    loop_->assertinLoopThread();
    assert(!isStarted);
    isStarted=true;
    for(int i=0;i<ThreadCount;i++)
    {
        EventLoopThread *ThreadTemp=new EventLoopThread();
        //threads_.insert(Thread(ThreadTemp));
        loops_.push_back(ThreadTemp->startLoop());
    }
    {
        MutexLockGrard lock(*mutex_);
        LiveThreadCount=loops_.size();
    }
    

}
int EventLoopThreadPool::CreatThread(int ThreadsCount)
{
    loop_->assertinLoopThread();
    assert(isStarted);
    isStarted=true;
    for(int i=0;i<ThreadsCount;i++)
    {
        EventLoopThread *ThreadTemp=new EventLoopThread();
        //threads_.insert(Thread(ThreadTemp));
        loops_.push_back(ThreadTemp->startLoop());
    }
    return ThreadsCount;
}
EventLoop* EventLoopThreadPool::getNextLoop()
{
    assert(loop_->isInLoopThread());
    //找到第一个空闲线程
    while(1)
    {
        if(!(loops_[(nextLoopIndex)%LiveThreadCount]->isLooping()))
            break;
        else
            nextLoopIndex++;
    }
    EventLoop *IOLoop=loops_[nextLoopIndex];
    {
        MutexLockGrard lock(*mutex_);
        BusyThreadCount++;
        //如果忙线程占总线程80%以上则扩大线程池
        if(BusyThreadCount*1.8>=LiveThreadCount)
            LiveThreadCount+=CreatThread(LiveThreadCount);
    }
    return IOLoop;
}
void EventLoopThreadPool::handleClientExit()
{
    MutexLockGrard lock(*mutex_);
    BusyThreadCount--;
    //如果忙线程不足总线程的50 则关闭部分线程
    if(LiveThreadCount*0.5>BusyThreadCount)
        closeThread((LiveThreadCount-BusyThreadCount)*0.8);
}
void EventLoopThreadPool::closeThread(int CloseCount)
{
    while(1)
    {
        if(!loops_[nextLoopIndex%LiveThreadCount]->isLooping())
        {
            loops_[nextLoopIndex%LiveThreadCount]->quit();
            std::vector<EventLoop*>::iterator it=loops_.begin();
            loops_.erase(it+nextLoopIndex%LiveThreadCount);
            if(--CloseCount==0)
                break;
        }
        else
            nextLoopIndex++;
        
    }
    LiveThreadCount=loops_.size();
}
