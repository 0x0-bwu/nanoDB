#include "generic/thread/ThreadPool.hpp"

namespace nano::thread {

inline static std::atomic_size_t threads = std::thread::hardware_concurrency();

using ThreadPool = generic::thread::ThreadPool;

inline void SetThreads(size_t n)
{
    threads.store(n);
}

inline size_t Threads()
{
    return threads.load();
}

inline ThreadPool Pool()
{
    return ThreadPool(Threads());
}

} // namespace nano::thread