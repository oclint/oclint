#ifndef OCLINT_THREADPOOL_H
#define OCLINT_THREADPOOL_H

#include <vector>
#include <thread>
#include <functional>

namespace oclint
{

class ThreadPool
{
private:
    std::vector<std::thread> _threads;

    void join();

public:
    unsigned int getNumberOfThreads();
    void run(std::function<void ()> function);
};

} // end namespace oclint

#endif
