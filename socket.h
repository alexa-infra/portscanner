#pragma once

#include "types.h"
#if defined(OS_WIN)
#   include <winsock2.h>
#   include <windows.h>
#endif

#include "data.h"

namespace ext {

#if defined(OS_WIN)
    typedef SOCKET Handle;
#else
    typedef int Handle;
#endif

class Socket {
private:
    Handle socket_;
    Host* host_;
    u16 port_;
    string status_;
    bool is_connected_;
    u32 timeout_;
public:
    Socket(Host* host, u16 port, u32 timeout);
    ~Socket();

    const string& host() const { return host_->address; }
    u16 post() const { return port_; }
    const string& status() { return status_; }
    bool is_connected() const { return is_connected_; }

    //! Perform connection to host-port by given timeout
    void TryConnect();
};

class SocketConnector {
private:
    string status_;
public:
    SocketConnector() {}
    ~SocketConnector() {}

    //! Scan host-port pair for connection
    bool TryConnect(Host* host, u16 port, i32 timeout) {
        Socket socket(host, port, timeout);
        socket.TryConnect();
        if (socket.is_connected())
            return true;
        status_ = socket.status();
        return false;
    }
    const string& status() const { return status_; }

    //! Resolves host
    static bool resolve(Host& host);
    //! Initialization of socket layer
    static void Initialize();
    //! Finalization of socket layer
    static void Shutdown();
};

} // namespace ext
