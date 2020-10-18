#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <atomic>
#include <poor_semaphore.h>

class Semaphore
{
protected:
    std::atomic<long> _count;
    PoorSemaphore     _sem;

public:
    Semaphore( long init_count );
    virtual ~Semaphore();
    void wait();
    void signal();
    bool wait_for( std::chrono::milliseconds milliseconds );
    void disable();
};

#endif // SEMAPHORE_H
