#include "generic/thread/ThreadPool.hpp"

namespace nano {

using ThreadPool = generic::thread::ThreadPool;

inline ThreadPool & Pool()
{
    static ThreadPool pool;
    return pool;
}

} // namespace nano