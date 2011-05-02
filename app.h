#pragma once

#include <iostream>

#include "types.h"
#include "socket.h"
#include "thread.h"
#include "mutex.h"

namespace ext {

class Application {
private:
    JobList jobs;               //!< List of current tasks
    Mutex mutex_queue;          //!< Mutex for queue access
    Mutex mutex_output;         //!< Mutex for console output
    SocketConnector conn;       //!< Socket connector object
    std::vector<Thread> thread_pool;    //!< Very simple pool of threads
public:
    Application() {
        SocketConnector::Initialize();
    }
    ~Application() {
        SocketConnector::Shutdown();
    }
public:
    //! Load job(task) from file, calculate rules, and perform port scan
    //! (by given host-port-timeout), outputs success results to console
    //! note: uses fixed number of threads, they are joined to current thread, and
    //! current thread will wait until execution
    void run(const string& filename) {
        HostList ll;
        if (!ll.Parse("config.json"))
            return;
        jobs = ll.GenJobs();

        thread_pool.clear();
        for (int i=0; i<5; i++) {
            Thread tt = Thread::StartThread<Application, &Application::worker>(this);
            thread_pool.push_back(tt);
        }
        for (int i=0; i<5; i++) {
            thread_pool[i].Join();
        }
    }
    //! Worker method (single thread entry) - pops element from working queue,
    //! and perform scan for specific host-port
    void worker() {
        ext::Job j;
        while(true) {
            {
                ScopedLock locker(mutex_queue);
                if (jobs.size() == 0)
                    break;
                j = jobs.front();
                jobs.pop();
            }
            if (conn.TryConnect(j.host, j.port, j.timeout))
            {
                ScopedLock locker(mutex_output);
                std::cout << j.host->address << ":" << j.port << " is ok" << std::endl;
            }
        }
    }
};

}
