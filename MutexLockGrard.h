
#include<boost/utility.hpp>

class MutexLock;
class MutexLockGrard:boost::noncopyable
{
private:
    MutexLock &mutex_;
public:
    MutexLockGrard(MutexLock &mutex);
    ~MutexLockGrard();
};
