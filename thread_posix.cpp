#include "thread.h"

namespace ext {

bool Thread::Create(Function func, void* object) {
    Thread t;
    if (pthread_create(&t.handle_, NULL, func, object) == 0) {
        return true;
    }
    return false;
}
Thread::Return Thread::Exit() {
    pthread_exit(NULL);
    return NULL;
}

} // namespace ext
