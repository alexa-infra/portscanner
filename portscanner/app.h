#pragma once

#include <iostream>

#include "types.h"
#include "thread.h"
#include "mutex.h"
#include "data.h"

namespace ext {

class Application {
public:
    Application();
    ~Application();
public:
    //! Load job(task) from file, calculate rules, and perform port scan
    //! (by given host-port-timeout), outputs success results to console
    //! note: uses fixed number of threads, they are joined to current thread, and
    //! current thread will wait until execution
    void run(const string& filename);
private:
    //! Worker method (single thread entry) - pops element from working queue,
    //! and perform scan for specific host-port
    void worker();
private:
    JobList jobs_;                      //!< List of current tasks
    Mutex mutexQueue_;                  //!< Mutex for queue access
    Mutex mutexOutput_;                 //!< Mutex for console output
    std::vector<Thread> threadPool_;    //!< Very simple pool of threads
}; // class Application

} // namespace ext
