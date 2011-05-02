#include "mutex.h"

namespace ext {

Mutex::Mutex() {
    ::InitializeCriticalSection((CRITICAL_SECTION*)&handle_);
}

Mutex::~Mutex() {
    ::DeleteCriticalSection((CRITICAL_SECTION*)&handle_);
}

void Mutex::lock() {
    ::EnterCriticalSection((CRITICAL_SECTION*)&handle_);
}

void Mutex::unlock() {
    ::LeaveCriticalSection((CRITICAL_SECTION*)&handle_);
}

} // namespace ext
