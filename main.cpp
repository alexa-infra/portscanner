#include <iostream>

#include "types.h"
#include "socket.h"
#include "thread.h"

const string host = "127.0.0.1";
const u16 port_min = 1;
const u16 port_max = 10000;
const u32 timeout = 200;

class Test {
public:
    void run() {
        std::cout << "hello world" << std::endl;
    }
};

int main() {
    {
        Test t;
        ext::Thread::StartThread<Test, &Test::run>(&t);
        ext::SocketConnector conn;
        ext::Host h;
        h.address = host;
        if (!conn.resolve(h))
            return -1;
        for (int port=port_min; port<port_max; port++) {
            if (conn.TryConnect(&h, port, timeout))
                std::cout << port << " is ok" << std::endl;
        }
    }
    return 0;
}
