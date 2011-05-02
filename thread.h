#pragma once

#include "types.h"
#if defined(OS_POSIX)
    #include <pthread.h>
#endif

namespace ext {

class Thread {
public:
#if defined(OS_POSIX)
    typedef pthread_t Handle;
    typedef void* Return;
    typedef void* Arg;
#elif defined(OS_WIN)
    typedef void* Handle;
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
private:
    Handle handle_;

    template<class T, void (T::*method)()>
    static Return Entry(Arg obj) {
        T* object = reinterpret_cast<T*>(obj);
        if (object != NULL)
            (object->*method)();
        return Exit();
    }

    template<class T, typename TT, void (T::*method)(TT*)>
    static Return Entry(Arg obj) {
        ThreadWithArg* helper = reinterpret_cast<ThreadWithArg*>(obj);
        if (helper == NULL)
            return Exit();
        T* object = reinterpret_cast<T*>(helper->object);
        TT* arg = reinterpret_cast<TT*>(helper->arg);
        if (object != NULL && arg != NULL)
            (object->*method)(arg);
        delete helper;
        return Exit();
    }

    static bool Create(Function func, void* object);

public:
    template<class T, void (T::*method)()>
    static bool StartThread(T* object) {
        return Create(Entry<T, method>, reinterpret_cast<void*>(object));
    }

    template<class T, typename TT, void (T::*method)(TT*)>
    static bool StartThread(T* object, TT* arg) {
        ThreadWithArg* helper = new ThreadWithArg(object, arg);
        return Create(Entry<T, TT, method>, reinterpret_cast<void*>(helper));
    }

private:
    static Return Exit();
};

}
