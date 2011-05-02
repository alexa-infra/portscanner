#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <errno.h>
#include <iostream>

namespace ext {

Socket::Socket(Host* host, u16 port, u32 timeout)
    : socket_(0)
    , host_(host)
    , port_(port)
    , status_("")
    , is_connected_(false)
    , timeout_(timeout)
{
    socket_ = socket(host->type, SOCK_STREAM, IPPROTO_IP); 
}

Socket::~Socket() {
    if (socket_ >= 0)
        close(socket_);
}

void Socket::TryConnect() {
    is_connected_ = false;
    struct sockaddr_in sin;
    sin.sin_family = host_->type;
    sin.sin_port = htons(port_);
    if (host_->type == AF_INET)
        memcpy(&sin.sin_addr, host_->resolved, 4);
    else if (host_->type == AF_INET6)
        memcpy(&sin.sin_addr, host_->resolved, 16);
    int flags = fcntl(socket_, F_GETFL, 0);
    fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
    if (connect(socket_, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        if (errno != EINPROGRESS)
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
    tout.tv_sec = 0;
    tout.tv_usec = timeout_;
    if (select(socket_ + 1, &fd_read, &fd_write, &fd_error, &tout) > 0)
    {
        if (FD_ISSET(socket_, &fd_error))
        {
            std::cout << "error set" << std::endl;
            return;
        }
        if (FD_ISSET(socket_, &fd_read) ||
            FD_ISSET(socket_, &fd_write)) {
            void* error = 0;
            socklen_t len = sizeof(error);
            if(getsockopt(socket_, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
                return;
            if (error > 0)
                return;
            is_connected_ = true;
            return;
        }
    }
    std::cout << "select return -1 or 0" << std::endl;
}

void SocketConnector::Initialize() {}
void SocketConnector::Shutdown() {}

bool SocketConnector::resolve(Host& host)
{
    struct hostent* hptr;
    hptr = gethostbyname(host.address.c_str());
    if (hptr == NULL)
        return false;
    host.type = hptr->h_addrtype;
    if (hptr->h_addrtype == AF_INET) {
        host.resolved = new u8[4];
        memcpy(host.resolved, hptr->h_addr, 4);
    } else if (hptr->h_addrtype == AF_INET6) {
        host.resolved = new u8[16];
        memcpy(host.resolved, hptr->h_addr, 16);
    }
    return true;
}

} // namespace ext

