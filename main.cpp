#include <iostream>

#include "types.h"
#include "socket.h"
#include "thread.h"
#include "mutex.h"

const string host = "127.0.0.1";
const u16 port_min = 1;
const u16 port_max = 65000;
const u32 timeout = 200;

class Test {
    ext::JobList jobs;
    ext::Mutex m;
    ext::SocketConnector conn;
public:
    void put_job(ext::Host* host, u16 port, u32 timeout) {
        ext::ScopedLock locker(m);
        ext::Job j;
        j.host = host;
        j.port = port;
        j.timeout = timeout;
        jobs.push(j);
    }
    void run() {
        ext::Job j;
        while(true) {
            {
                ext::ScopedLock locker(m);
                if (jobs.size() == 0)
                    break;
                j = jobs.front();
                jobs.pop();
            }

            if (conn.TryConnect(j.host, j.port, j.timeout))
                std::cout << j.port << " is ok" << std::endl;
        }
    }
};

int main() {
    ext::Mutex m;
    {
        ext::ScopedLock locker(m);
        Test t;
        ext::SocketConnector conn;
        ext::Host h;
        h.address = host;
        if (!conn.resolve(h))
            return -1;
        for (int port=port_min; port<port_max; port++) {
            t.put_job(&h, port, timeout);
        }
        ext::Thread tt = ext::Thread::StartThread<Test, &Test::run>(&t);
        ext::Thread tt2 = ext::Thread::StartThread<Test, &Test::run>(&t);

        tt.Join();
        tt2.Join();
    }
    return 0;
}
