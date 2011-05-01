#pragma once

#include "types.h"
#if defined(OS_WIN)
#   include <windows.h>
#endif

namespace ext {

#if defined(OS_WIN)
    typedef HANDLE Handle;
#else
    typedef int Handle;
#endif

class Socket {
private:
    Handle socket_;
    string host_;
    u16 port_;
    string status_;
    bool is_connected_;
    u32 timeout_;
public:
    Socket(const string& host, u16 port, u32 timeout);
    ~Socket();

    const string& host() const { return host_; }
    u16 post() const { return port_; }
    const string& status() { return status_; }
    bool is_connected() const { return is_connected_; }

    void TryConnect();
};

class SocketConnector {
private:
    string status_;
public:
    SocketConnector() {
        Initialize();
    }
    ~SocketConnector() {
        Shutdown();
    }

    bool TryConnect(const string& host, u16 port, i32 timeout) {
        Socket socket(host, port, timeout);
        socket.TryConnect();
        if (socket.is_connected())
            return true;
        status_ = socket.status();
        return false;
    }
    const string& status() const { return status_; }
private:
    void Initialize();
    void Shutdown();
};

} // namespace ext
