#include "generic/thread/ThreadPool.hpp"

namespace nano::thread {

using ThreadPool = generic::thread::ThreadPool;

inline ThreadPool & Pool()
{
    static ThreadPool pool;
    return pool;
}

inline void SetThreads(size_t threads)
{
    return Pool().Resize(threads);
}

inline size_t Threads()
{
    return thread::Pool().Threads();
}

} // namespace nano::thread