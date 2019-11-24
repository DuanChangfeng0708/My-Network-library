#include"MutexLockGrard.h"
#include"MutexLock.h"
MutexLockGrard::MutexLockGrard(MutexLock &mutex)
    :mutex_(mutex)
{
    mutex_.Lock();
}

MutexLockGrard::~MutexLockGrard()
{
    mutex_.UnLock();
}
