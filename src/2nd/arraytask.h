#ifndef ARRAYTASK_H
#define ARRAYTASK_H

#include <chrono>
#include <atomic>
#include <mutex>


class ArrayTask
{
    std::atomic<uint32_t> _index_a;
    uint32_t _index_m;
    std::mutex _m;
    int *_array;
    long _duration;

    uint32_t NumTasks;
    uint32_t NumThreads;

public:
    enum class task : uint8_t {
        mutex             = 0,
        mutex_with_sleep  = 1,
        atomic            = 2,
        atomic_with_sleep = 3
    };

    uint64_t getDuration();
    bool run( task );
    void setNumsTasksThreads( uint32_t NumTasks, uint32_t NumThreads );
    ArrayTask();
    ~ArrayTask();

protected:
    void atomicFunc();
    void atomicSleepingFunc();
    void mutexFunc();
    void mutexSleepingFunc();
};

#endif // ARRAYTASK_H
