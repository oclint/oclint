#ifndef OCLINT_QUEUE_H
#define OCLINT_QUEUE_H

#include <vector>
#include <mutex>

// This class must be thread-safe!

namespace oclint
{

template <typename T>
class Queue
{
private:
    std::vector<T> _vector;
    std::mutex _mutex;

public:
    Queue() {}
    Queue(const std::vector<T> &vector) : _vector(vector) {}

    void add(const T &object)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _vector.push_back(object);
    }

    bool pop(T &object)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_vector.size() > 0)
        {
            object = _vector.back();
            _vector.pop_back();
            return true;
        }
        return false;
    }
};

} // end namespace oclint

#endif
