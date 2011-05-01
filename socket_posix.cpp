#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

namespace ext {

Socket::Socket(const string& host, u16 port, u32 timeout)
    : socket_(0)
    , host_(host)
    , port_(port)
    , status_("")
    , is_connected_(false)
    , timeout_(timeout)
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0); 
}

Socket::~Socket() {
    if (socket_ >= 0)
        close(socket_);
}

void Socket::TryConnect() {
    struct sockaddr_in sin;
    struct hostent *hptr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_); 
    hptr =  gethostbyname(host_.c_str());
    if (hptr == NULL) {
        return;
    }
    memcpy( &sin.sin_addr, hptr->h_addr, hptr->h_length);
    if (connect (socket_, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ) {
        return;
    }
    is_connected_ = true;
}

void SocketConnector::Initialize() {}
void SocketConnector::Shutdown() {}

} // namespace ext

