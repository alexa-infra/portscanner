#pragma once

#include <iostream>

#include "types.h"
#include "socket.h"
#include "thread.h"
#include "mutex.h"

namespace ext {

class Application {
private:

    JobList jobs;
    Mutex mutex_queue;
    Mutex mutex_output;
    SocketConnector conn;
    std::vector<Thread> thread_pool;

public:
    Application() {
        SocketConnector::Initialize();
    }
    ~Application() {
        SocketConnector::Shutdown();
    }
public:
    void put_job(ext::Host* host, u16 port, u32 timeout) {
        ext::ScopedLock locker(mutex_queue);
        ext::Job j;
        j.host = host;
        j.port = port;
        j.timeout = timeout;
        jobs.push(j);
    }
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
