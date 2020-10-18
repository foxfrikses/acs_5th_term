#ifndef POORSEMAPHORE_H
#define POORSEMAPHORE_H

#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <inttypes.h>

class PoorSemaphore
{
protected:
    std::condition_variable _cv;
    std::mutex              _m;
    std::size_t             _count;
    std::atomic<bool>       _is_disabled;

public:
    PoorSemaphore();
    virtual ~PoorSemaphore();
    void signal();
    void wait();
    bool wait_for( std::chrono::milliseconds milliseconds );
    void disable();
};

#endif // POORSEMAPHORE_H
