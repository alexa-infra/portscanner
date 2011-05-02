#pragma once

#include "types.h"
#if defined(OS_POSIX)
#   include <pthread.h>
#elif defined(OS_WIN)
#   include <windows.h>
#endif


namespace ext {

class Mutex {
public:
#if defined(OS_POSIX)
    typedef pthread_mutex_t Handle;
#elif defined(OS_WIN)
    typedef CRITICAL_SECTION Handle;
#endif
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
private:
    Handle handle_;
};

class ScopedLock {
    Mutex& mutex_;
public:
    ScopedLock(Mutex& mutex) : mutex_(mutex) {
        mutex_.lock();
    }
    ~ScopedLock() {
        mutex_.unlock();
    }
};

} // namespace ext
