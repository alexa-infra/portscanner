#pragma once

#include "types.h"
#if defined(HAS_PTHREAD_H)
#   include <pthread.h>
#elif defined(OS_WIN)
#   include <windows.h>
#endif


namespace ext {

//! Mutex object (CS for windows), used for scoped lock
class Mutex {
public:
#if defined(HAS_PTHREAD_H)
    typedef pthread_mutex_t Handle;
#elif defined(OS_WIN)
    typedef CRITICAL_SECTION Handle;
#endif
    Mutex();
    ~Mutex();

    //! Lock mutex (block current thread until mutex release by another thread)
    void lock();
    //! Leave mutex
    void unlock();
private:
    Handle handle_;
};

class ScopedLock {
public:
    ScopedLock(Mutex& mutex) : mutex_(mutex) {
        mutex_.lock();
    }
    ~ScopedLock() {
        mutex_.unlock();
    }
private:
    Mutex& mutex_;
};

} // namespace ext
