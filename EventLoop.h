
#include<unistd.h>
#include<boost/utility.hpp>
#include<boost/scoped_ptr.hpp>
#include<assert.h>
#include<vector>
#include<pthread.h>
#include<iostream>
#include<functional>


class Poller;
class Channel;
class MutexLock;
/*
什么都不做的Eventloop 目前功能只用于确定one thread per loop,其他线程不能调用此loop
*/

class EventLoop :boost::noncopyable
{
    
private:
    typedef std::function<void()> Functor;
    void QueueInLoop(const Functor cb);
    void weakUp();
    void doQueueFunc();

    typedef std::vector<Channel*>   ActivityChannels;
    const pthread_t                 threadId_;
    bool                            looping_;
    std::unique_ptr<Poller>         poller_;
    ActivityChannels                activityChannels;
    std::vector<Functor>            FuncQueue;
    bool                            quit_;
    MutexLock*                      mutex_;
    bool                            callQueueFunc;
    int                             WeakUpFd_;
    std::unique_ptr<Channel>        WeakUpChannel_;
public:
    void assertinLoopThread()const {assert(isInLoopThread());};
    bool isInLoopThread() const {return threadId_==pthread_self();};
    void RunInLoopThread(const Functor );
    void loop();
    void quit();
    void updateChannel(Channel *);
    EventLoop();
    ~EventLoop();
    int CreatEventFd();
    void handleWeadupFdEvent();
    void removeChannel(Channel *channel);
    bool isLooping(){return looping_;};
    bool setLoopState(bool state){looping_=state;};
};



