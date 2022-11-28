#include "socket.h"

namespace co {

void Listenner::ListenTcp(uint16_t port) {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd < 0) {
        perror("socket");
        exit(0);
    }

    // 设置fd为非阻塞-> 最终目的是以同步的编码方式实现异步的效果
    if (fcntl(m_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(0);
    }
    // 常规socket调用过程
    struct sockaddr_in addr;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family      = AF_INET;

    if (bind(m_fd, ( sockaddr* )&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(0);
    }

    if (listen(m_fd, 8) < 0) {
        perror("listen");
        exit(0);
    }
    // for debug
    printf("listen %d success\n", port);
}

int Listenner::Accepter() {
    while (true) {
        int cli_fd = accept(m_fd, nullptr, nullptr);
        if (cli_fd > 0) {
            // for debug
            printf("accept success\n");
            return cli_fd;
        }
        if (errno == EAGAIN) {
            // yield
        }
        else if (errno == EINTR) {
            continue;
        }
        else {
            perror("accept");
            return -1;
        }
    }
    return -1;
}

}  // namespace co
