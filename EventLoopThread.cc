#include"EventLoopThread.h"
#include"MutexLock.h"
#include"MutexLockGrard.h"
#include"EventLoop.h"
void* threadFunc(void * arg)
{
    EventLoop *thisLoop;
    EventLoopThread *thread=(EventLoopThread*) arg;
    {
        MutexLockGrard LOCK(*(thread->getmutex()));
        EventLoop *IOloop=new EventLoop();
        thread->setIOloop(IOloop);
        pthread_cond_signal(thread->getCond());
        thisLoop=IOloop;
    }
    
    thread->loop();
    std::cout<<"EventLoop :"<<thisLoop<<" exited\n";
    //IOloop=new EventLoop();
}
EventLoopThread::EventLoopThread(/* args */)
:mutex_(new MutexLock()),
cond_(PTHREAD_COND_INITIALIZER)
{
}

EventLoopThread::~EventLoopThread()
{
}
EventLoop* EventLoopThread::startLoop()
{
   
    if(pthread_create(&(this->currentThread),NULL,threadFunc,this)!=0)
        perror("thread creat\n");
    IOloop =NULL;
    {
         MutexLockGrard LOCK(*mutex_);
         while(IOloop==NULL)
         {
            pthread_cond_wait(&cond_,mutex_->getMutex());
         }
    }
    return IOloop;
}
void EventLoopThread::loop()
{
    assert(IOloop!=NULL);
    IOloop->loop();
}