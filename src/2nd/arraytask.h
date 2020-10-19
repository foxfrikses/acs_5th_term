#ifndef ARRAYTASK_H
#define ARRAYTASK_H

#include <chrono>
#include <atomic>
#include <mutex>
#include <vector>


class ArrayTask
{
    std::atomic<uint32_t> _index_a;
    uint32_t              _index_m;
    std::mutex _m;
    long _duration;

    uint32_t _num_tasks;
    uint32_t _num_threads;

    std::vector<int> _array;

public:
    enum class task : uint8_t {
        mutex             = 0,
        mutex_with_sleep  = 1,
        atomic            = 2,
        atomic_with_sleep = 3
    };

    uint64_t get_duration();
    bool run( task );
    void set_num_tasks  ( uint32_t num_tasks   );
    void set_num_threads( uint32_t num_threads );
    ArrayTask();
    ~ArrayTask();

protected:
    template<class T>
    void _func( T& counter, bool is_mutex, bool is);
};

#endif // ARRAYTASK_H
