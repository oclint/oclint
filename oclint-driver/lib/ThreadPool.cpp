#include "oclint/ThreadPool.h"
#include "oclint/Options.h"

using namespace oclint;

unsigned int ThreadPool::getNumberOfThreads()
{
    return option::numThreads();
}

void ThreadPool::run(std::function<void ()> function)
{
    _threads = std::vector<std::thread>(getNumberOfThreads());
    for (unsigned int i = 0; i < getNumberOfThreads(); i++)
    {
        _threads[i] = std::thread(function);
    }
    join();
}

void ThreadPool::join()
{
    for (std::thread &thread : _threads)
    {
        thread.join();
    }
}
