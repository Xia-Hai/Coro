#pragma once

#include <functional>
#include <list>
#include <map>
#include <ucontext.h>
#include <vector>
// schedule
namespace co {

class Fiber;

using func = std::function<void()>;

class Schedule {
public:
    Schedule();
    ~Schedule();
    void createFiber(func run);
    void resume();
    void yield();
    ucontext_t *SchedCtx() {
        return &m_scheCtx;
    }
private:
    Fiber *cur_fiber;
    ucontext_t          m_scheCtx;
    std::vector<Fiber*> m_ready_fibers, m_running_fibers;
};

class Fiber {
public:
    Fiber(func run, Schedule *sched, int size);
    ~Fiber();
    static void start(Fiber* fiber);
    ucontext_t *getCtx() {
        return &m_ctx;
    }
    bool isFinished() {
        return m_status == -1;
    }
private:
    int m_status;
    ucontext_t m_ctx;
    func m_run;
    size_t m_size;
    char *m_stack;
};

}  // namespace co
