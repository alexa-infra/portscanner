#include "thread.h"
#include <iostream>

namespace ext {

Thread Thread::create(Function func, void* object) {
    DWORD threadId;
    Thread t;
    t.handle_ = ::CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)func, object, 0, &threadId );
    if (!t.handle_) {
        std::cout << "error thread creation" << std::endl;
    }
    return t;
}
Thread::Return Thread::exit() {
    return 0;
}
void Thread::join() {
    ::WaitForSingleObject(handle_, INFINITE);
    ::CloseHandle(handle_);
}

} // namespace ext
