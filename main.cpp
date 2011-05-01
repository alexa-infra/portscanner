#include <iostream>

#include "types.h"
#include "socket.h"

const string host = "127.0.0.1";
const u16 port_min = 1;
const u16 port_max = 10000;
const u32 timeout = 200;

int main() {
    {
        ext::SocketConnector conn;
        for (int port=port_min; port<port_max; port++) {
            if (conn.TryConnect(host, port, timeout))
                std::cout << port << " is ok" << std::endl;
        }
    }
    return 0;
}
