#include "app.h"
#include "socket.h"

namespace ext
{

Application::Application()
{
    SocketConnector::Initialize();
}

Application::~Application()
{
    SocketConnector::Shutdown();
}

void Application::run(const string& filename)
{
    HostList hostList;
    if (!hostList.Parse("config.json"))
        return;
    jobs_ = hostList.GenJobs();

    const i32 numThreads = 5;
    threadPool_.clear();
    for (int i=0; i<numThreads; i++) {
        Thread tt = Thread::StartThread<Application, &Application::worker>(this);
        threadPool_.push_back(tt);
    }
    for (int i=0; i<numThreads; i++) {
        threadPool_[i].Join();
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
        if (connection.TryConnect(job.host, job.port, job.timeout))
        {
            ScopedLock locker(mutexOutput_);
            std::cout << job.host->address << ":" << job.port << " is ok" << std::endl;
        }
    }
}


}