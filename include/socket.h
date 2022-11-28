#pragma once
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

namespace co {
class Listenner {
public:
    void ListenTcp(uint16_t port);
    int  Accepter();

private:
    int m_fd;
};

}  // namespace co
