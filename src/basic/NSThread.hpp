#pragma once
#include "generic/thread/ThreadPool.hpp"

namespace nano::thread {

using ThreadPool = generic::thread::ThreadPool;

class ThreadMgr
{
public:
    static ThreadMgr & Instance()
    {
        static ThreadMgr instance;
        return instance;
    }

    void SetThreads(size_t n)
    {
        m_threads.store(n);
    }

    size_t Threads() const
    {
        return m_threads.load();
    }

    ThreadPool Pool() const
    {
        return ThreadPool(Threads());
    }

private:
    ThreadMgr() : m_threads(std::thread::hardware_concurrency()) {}
    ThreadMgr(const ThreadMgr &) = delete;
    ThreadMgr & operator= (const ThreadMgr &) = delete;

    std::atomic_size_t m_threads;
};

inline void SetThreads(size_t n)
{
    ThreadMgr::Instance().SetThreads(n);
}

inline size_t Threads()
{
    return ThreadMgr::Instance().Threads();
}

inline ThreadPool Pool()
{
    return ThreadMgr::Instance().Pool();
}

} // namespace nano::thread