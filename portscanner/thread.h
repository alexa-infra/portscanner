#pragma once

#include "types.h"
#if defined(HAS_PTHREAD_H)
#   include <pthread.h>
#elif defined(OS_WIN)
#   include <windows.h>
#endif

namespace ext {

class Thread {
public:
#if defined(HAS_PTHREAD_H)
    typedef pthread_t Handle;
    typedef void* Return;
    typedef void* Arg;
#elif defined(OS_WIN)
    typedef HANDLE Handle;
    typedef i32 Return;
    typedef void* Arg;
#endif
    typedef Return (*Function)(Arg);

    struct ThreadWithArg {
        void* object;
        void* arg;

        ThreadWithArg(void* obj, void* a)
            : object(obj)
            , arg(a)
        {}
    };

public:
    template<class T, void (T::*method)()>
    static Thread startThread(T* object) {
        return create(entry<T, method>, reinterpret_cast<void*>(object));
    }

    template<class T, typename TT, void (T::*method)(TT*)>
    static Thread startThread(T* object, TT* arg) {
        ThreadWithArg* helper = new ThreadWithArg(object, arg);
        return create(entry<T, TT, method>, reinterpret_cast<void*>(helper));
    }
    void join();

private:
    template<class T, void (T::*method)()>
    static Return entry(Arg obj) {
        T* object = reinterpret_cast<T*>(obj);
        if (object != NULL)
            (object->*method)();
        return exit();
    }

    template<class T, typename TT, void (T::*method)(TT*)>
    static Return entry(Arg obj) {
        ThreadWithArg* helper = reinterpret_cast<ThreadWithArg*>(obj);
        if (helper == NULL)
            return exit();
        T* object = reinterpret_cast<T*>(helper->object);
        TT* arg = reinterpret_cast<TT*>(helper->arg);
        if (object != NULL && arg != NULL)
            (object->*method)(arg);
        delete helper;
        return exit();
    }

    static Thread create(Function func, void* object);
    static Return exit();
private:
    Handle handle_;
};

}
