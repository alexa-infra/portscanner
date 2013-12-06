#include "socket.h"

#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>

typedef int socklen_t;

namespace ext {

Socket::Socket(Host* host, u16 port, u32 timeout)
    : socket_(0)
    , host_(host)
    , port_(port)
    , status_("")
    , isConnected_(false)
    , timeout_(timeout)
{
    socket_ = ::WSASocket(host->type, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
}

Socket::~Socket() {
    if (socket_ != INVALID_SOCKET)
        ::closesocket(socket_);
}

void Socket::tryConnect() {
    isConnected_ = false;
    struct sockaddr_in sin;
    sin.sin_family = host_->type;
    sin.sin_port = htons(port_);
    if (host_->type == AF_INET)
        memcpy(&sin.sin_addr, host_->resolved, 4);
    else if (host_->type == AF_INET6)
        memcpy(&sin.sin_addr, host_->resolved, 16);

    u32 tmp = 1;
    ioctlsocket(socket_, FIONBIO, (u_long*)&tmp);

    if (connect(socket_, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        int err = ::WSAGetLastError();
        if (err != WSAEINPROGRESS && err != WSAEWOULDBLOCK)
            return;
    }

    fd_set fd_write, fd_error, fd_read;
    FD_ZERO(&fd_error);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_read);
    FD_SET(socket_, &fd_write);
    FD_SET(socket_, &fd_error);
    FD_SET(socket_, &fd_read);

    struct timeval tout;
    tout.tv_sec = timeout_ / 1000;
    tout.tv_usec = timeout_ % 1000;
    if (select(0, &fd_read, &fd_write, &fd_error, &tout) > 0)
    {
        if (FD_ISSET(socket_, &fd_read) || 
            FD_ISSET(socket_, &fd_write)) {
            isConnected_ = true;
            return;
        }
    }
}

void SocketConnector::initialize() {
    WSADATA WSAData_;
    int result = WSAStartup(MAKEWORD(2,2), &WSAData_);
                
    if (result != NO_ERROR)
        std::cout << "socket initialization error: " << WSAGetLastError() << std::endl;
}
void SocketConnector::shutdown() {
    int result = WSACleanup();
    if (result != NO_ERROR)
        std::cout << "socket cleanup error: " << WSAGetLastError() << std::endl;
}

bool SocketConnector::resolve(Host& host)
{
    struct hostent* hptr;
    hptr = gethostbyname(host.address.c_str());
    if (hptr == NULL)
        return false;
    host.type = hptr->h_addrtype;
    if (hptr->h_addrtype == AF_INET) {
        memcpy(host.resolved, hptr->h_addr, 4);
    } else if (hptr->h_addrtype == AF_INET6) {
        memcpy(host.resolved, hptr->h_addr, 16);
    }
    return true;
}

} // namespace ext

