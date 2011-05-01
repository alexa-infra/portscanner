#pragma once

namespace ext {

class Thread {
public:
    typedef void* Return;
    typedef void* Arg;
    typedef Return (*Function)(Arg);

    struct ThreadWithArg {
        void* object;
        void* arg;

        ThreadWithArg(void* obj, void* a)
            : object(obj)
            , arg(a)
        {}
    };

    template<class T, void (T::*method)()>
    static Return Entry(Arg obj) {
        T* object = dynamic_cast<T*>(obj);
        if (object != NULL)
            (object->*method)();
        return Exit();
    }
    
    template<class T, typename TT, void (T::*method)(TT*)>
    static Return Entry(Arg obj) {
        ThreadArgs* helper = dynamic_cast<ThreadArgs*>(obj);
        if (helper == NULL)
            return Exit();
        T* object = dynamic_cast<T*>(helper->object);
        TT* args = dynamic_cast<TT*>(helper->args);
        if (object != NULL && args != NULL)
            (object->*method)(args);
        return Exit();
    }

    static Return Exit();
};

}
