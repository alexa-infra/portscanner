#include "mutex.h"

namespace ext {

#if defined(OS_MAC)
#   define recursive_mutex PTHREAD_MUTEX_RECURSIVE
#else
#   define recursive_mutex PTHREAD_MUTEX_RECURSIVE_NP
#endif

Mutex::Mutex() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, recursive_mutex);
    pthread_mutex_init(&handle_, &attr);
    pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&handle_);
}

void Mutex::lock() {
    pthread_mutex_lock(&handle_);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&handle_);
}

} // namespace ext
