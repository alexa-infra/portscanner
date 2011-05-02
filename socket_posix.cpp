#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

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
    socket_ = socket(host->type, SOCK_STREAM, 0); 
}

Socket::~Socket() {
    if (socket_ >= 0)
        close(socket_);
}

void Socket::TryConnect() {
    struct sockaddr_in sin;
    sin.sin_family = host_->type;
    sin.sin_port = htons(port_);
    if (host_->type == AF_INET)
        memcpy(&sin.sin_addr, host_->resolved, 4);
    else if (host_->type == AF_INET6)
        memcpy(&sin.sin_addr, host_->resolved, 16);
    if (connect(socket_, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        return;
    }
    is_connected_ = true;
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
        std::cout << hptr->h_length << std::endl;
        memcpy(host.resolved, hptr->h_addr, 4);
    } else if (hptr->h_addrtype == AF_INET6) {
        host.resolved = new u8[16];
        memcpy(host.resolved, hptr->h_addr, 16);
    }
    return true;
}

} // namespace ext

