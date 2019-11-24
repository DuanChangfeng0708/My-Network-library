#include<boost/noncopyable.hpp>
#include<pthread.h>
#include<functional>
class EventLoop;
class MutexLock;




class EventLoopThread:boost::noncopyable
{
private:
    /* data */
    pthread_t currentThread;
    MutexLock* mutex_;
    pthread_cond_t cond_;
    EventLoop* IOloop;//必须在线程里面new
   
public:
    EventLoopThread(/* args */);
    /** 
     *  在此函数中创建新线程并且返回IOloop
    */
    EventLoop * startLoop();
    void *setIOloop(EventLoop * loop){IOloop=loop;};
    MutexLock *getmutex()const {return mutex_;};
    pthread_cond_t * getCond(){return &cond_;};
    ~EventLoopThread();
    void loop();
};

