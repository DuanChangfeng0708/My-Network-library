#include"MutexLock.h"


MutexLock::MutexLock(/* args */)
:hodler_(0)
{
    pthread_mutex_init(&mutex_,NULL);
}
void MutexLock::Lock()
{
    pthread_mutex_lock(&mutex_);
    hodler_=pthread_self();
}
void MutexLock::UnLock()
{
    hodler_=0;
    pthread_mutex_unlock(&mutex_);
}
MutexLock::~MutexLock()
{
    assert(hodler_==0);
    pthread_mutex_destroy(&mutex_);
}
