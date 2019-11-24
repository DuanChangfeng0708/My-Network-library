
#include<boost/noncopyable.hpp>
#include<pthread.h>
#include<assert.h>
/**
 * 只实现了锁和解锁 暂时不支持condition
*/
class MutexLock: boost::noncopyable
{
private:
    /* data */
    pthread_mutex_t mutex_;
    pthread_t hodler_;
public:
    MutexLock(/* args */);
    bool isLockBythisThread(){return hodler_==pthread_self();};
    void Lock();//给 MutexLockGrund使用
    void UnLock();//同 Look（）
    pthread_mutex_t* getMutex() {return &mutex_;};
    ~MutexLock();
};

