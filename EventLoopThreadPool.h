#include<boost/noncopyable.hpp>
#include<memory>
#include<vector>
#include<set>
class EventLoop;
class EventLoopThread;
class MutexLock;
class EventLoopThreadPool
{
private:
    /* data */
    EventLoop *loop_;
    bool isStarted;
    typedef std::unique_ptr<EventLoopThread> Thread;
    std::set<Thread> threads_;
    std::vector<EventLoop *> loops_;
    unsigned int nextLoopIndex;
    MutexLock *mutex_;
    pthread_cond_t cond_;
    int LiveThreadCount;   //存活的线程总数
    int BusyThreadCount;
    void closeThread(int closeCount);
    int CreatThread(int ThreadsCount);
public:
    EventLoopThreadPool(EventLoop *loop);

    void start(int ThreadCount);//开启ThreadCount个线程
    int getCurrentCountOfThread(){return threads_.size();};
    EventLoop* getNextLoop();
    ~EventLoopThreadPool();
    void handleClientExit();    //有客户端退出，归还线程
};


