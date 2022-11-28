#include "coroutine.h"
#include "socket.h"
#include <iostream>

/*
    用于测试，bug很多 :)
*/

int main() {
    co::Schedule sche;
    sche.createFiber([&]() {
        // listen
        co::Listenner listenner;
        listenner.ListenTcp(8888);
        while (true) {
            // accept
            int cli_fd = listenner.Accepter();
            if (cli_fd < 0) {
                continue;
            }
            sche.createFiber([&]() {
                // recv and send
                /*
                    简单测试，实际情况read和write会阻塞，这时需要调用yield
                    让出cpu
                */
                char buf[128]{};
                read(cli_fd, buf, 128);
                std::cout << buf << std::endl;
                write(cli_fd, buf, 128);
            });

            // for debug
            sche.yield();
        }
    });
    sche.resume();

    return 0;
}