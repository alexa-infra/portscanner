#include "thread.h"

namespace ext {

Thread Thread::Create(Function func, void* object) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    Thread t;
    if (pthread_create(&t.handle_, &attr, func, object) == 0) {
        return t;
    }
    pthread_attr_destroy(&attr);
    return t;
}
Thread::Return Thread::Exit() {
    pthread_exit(NULL);
    return NULL;
}
void Thread::Join() {
    void *status;
    pthread_join(handle_, &status);
}

} // namespace ext
