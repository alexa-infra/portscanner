#pragma once

#include "types.h"
#if defined(OS_WIN)
#   include <winsock2.h>
#   include <windows.h>
#endif

#include "data.h"

namespace ext {

class Socket {
public:
    Socket(Host* host, u16 port, u32 timeout);
    ~Socket();

    const string& host() const { return host_->address; }
    u16 post() const { return port_; }
    const string& status() { return status_; }
    bool isConnected() const { return isConnected_; }

    //! Perform connection to host-port by given timeout
    void tryConnect();
private:
    #if defined(OS_WIN)
        typedef SOCKET Handle;
    #else
        typedef int Handle;
    #endif
    Handle socket_;
    Host* host_;
    u16 port_;
    string status_;
    bool isConnected_;
    u32 timeout_;
};

class SocketConnector {
public:
    SocketConnector() {}
    ~SocketConnector() {}

    //! Scan host-port pair for connection
    bool tryConnect(Host* host, u16 port, i32 timeout) {
        Socket socket(host, port, timeout);
        socket.tryConnect();
        if (socket.isConnected())
            return true;
        status_ = socket.status();
        return false;
    }
    const string& status() const { return status_; }
public:
    //! Resolves host
    static bool resolve(Host& host);
    //! Initialization of socket layer
    static void initialize();
    //! Finalization of socket layer
    static void shutdown();
private:
    string status_;
};

} // namespace ext
