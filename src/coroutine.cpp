#include "coroutine.h"

namespace co {

Schedule::Schedule() {}
Schedule::~Schedule() {}
void Schedule::createFiber(func run) {
    Fiber* fiber = new Fiber(run, this, 1024 * 128);
    m_ready_fibers.emplace_back(fiber);
}
void Schedule::resume() {
    while (true) {
        if (m_ready_fibers.empty()) {
            continue;
        }
        m_running_fibers = m_ready_fibers;
        m_ready_fibers.clear();
        for (auto& it : m_running_fibers) {
            swapcontext(&m_scheCtx, it->getCtx());
            if (it->isFinished()) {
                delete it;
            }
        }
        m_running_fibers.clear();
    }
}
void Schedule::yield() {}

Fiber::Fiber(func run, Schedule* sched, int size) : m_status(0), m_run(run), m_size(size), m_stack(new char[m_size]) {
    getcontext(&m_ctx);
    m_ctx.uc_link          = sched->SchedCtx();
    m_ctx.uc_stack.ss_sp   = m_stack;
    m_ctx.uc_stack.ss_size = m_size;
    makecontext(&m_ctx, ( void (*)() )Fiber::start, 1, this);
}
Fiber::~Fiber() {
    if (m_stack) {
        delete[] m_stack;
        m_stack = nullptr;
        m_size = 0;
    }
}
void Fiber::start(Fiber* fiber) {
    fiber->m_run();
    fiber->m_status = -1;
}

}  // namespace co
