#include <iostream>
#include "coroutine.h"


int main() {
    co::Schedule sche;
    sche.createFiber([]() {
        std::cout << "XiaHai" << std::endl;
    });
    sche.createFiber([]() {
        std::cout << "GuoYin" << std::endl;
    });
    sche.resume();
}