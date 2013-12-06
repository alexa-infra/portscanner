#include "app.h"
#include "socket.h"

namespace ext
{

Application::Application()
{
    SocketConnector::initialize();
}

Application::~Application()
{
    SocketConnector::shutdown();
}

void Application::run(const string& filename)
{
    HostList hostList;
    if (!hostList.parse(filename)) {
        return;
    }
    jobs_ = hostList.jobs;

    const i32 numThreads = 5;
    threadPool_.clear();
    for (int i=0; i<numThreads; i++) {
        Thread tt = Thread::startThread<Application, &Application::worker>(this);
        threadPool_.push_back(tt);
    }
    for (int i=0; i<numThreads; i++) {
        threadPool_[i].join();
    }
}

void Application::worker()
{
    Job job;
    while(true) {
        {
            ScopedLock locker(mutexQueue_);
            if (jobs_.size() == 0)
                break;
            job = jobs_.front();
            jobs_.pop();
        }
        SocketConnector connection;
        if (connection.tryConnect(job.host, job.port, job.timeout))
        {
            ScopedLock locker(mutexOutput_);
            std::cout << job.host->address << ":" << job.port << " is ok" << std::endl;
        }
    }
}


}