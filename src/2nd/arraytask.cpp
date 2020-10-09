#include "arraytask.h"

#include <thread>
#include <chrono>

ArrayTask::ArrayTask()
    : _array{nullptr}
{}

ArrayTask::~ArrayTask(){
    if( !_array )
        delete[] _array;
}

void ArrayTask::setNumsTasksThreads( uint32_t NumTasks, uint32_t NumThreads ){
    if( !_array )
        delete[] _array;
    this->NumTasks   = NumTasks;
    this->NumThreads = NumThreads;
    if( NumTasks != 0u && NumThreads != 0u ){
        _array = new int[NumTasks];
        for( auto i = 0u; i < NumTasks; ++i )
            _array[i] = 0;
        _index_a = 0;
        _index_m = 0;
    }
    else _array = nullptr;
}

uint64_t ArrayTask::getDuration(){
   return _duration;
}

bool ArrayTask::run( ArrayTask::task tsk ){
    using namespace std::chrono;

    if ( !_array )
        return false;

    for( auto i = 0u; i < NumTasks; ++i )
        _array[i] = 0;

    void (ArrayTask::*f)(void);
    using task = ArrayTask::task;
    switch (tsk) {
    case task::atomic:
        f = &ArrayTask::atomicFunc;
        _index_a.store(0u);
        break;
    case task::atomic_with_sleep:
        f = &ArrayTask::atomicSleepingFunc;
        _index_a.store(0u);
        break;
    case task::mutex:
        f = &ArrayTask::mutexFunc;
        _index_m = 0;
        break;
    case task::mutex_with_sleep:
        f = &ArrayTask::mutexSleepingFunc;
        _index_m = 0;
        break;
    }

    std::thread threads[NumThreads];

    auto start{high_resolution_clock::now()};

    for( auto i = 0u; i < NumThreads; ++i )
        threads[i] = std::thread(f, this);
    for( auto i = 0u; i < NumThreads; ++i )
        threads[i].join();

    auto end{high_resolution_clock::now()};
    _duration = duration_cast<milliseconds>(end - start).count();

    for( auto i = 0u; i < NumTasks; ++i )
        if ( _array[i] != 1 )
            return false;

    return true;
}

void ArrayTask::atomicFunc()
{
    uint32_t i;
    bool flag{true};
    while( flag ) {
        i = _index_a.fetch_add(1);
        if( i < NumTasks ) {
            ++_array[i];
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::atomicSleepingFunc()
{
    uint32_t i;
    bool flag{true};
    while( flag ) {
        i = _index_a.fetch_add(1);
        if( i < NumTasks ) {
            ++_array[i];
            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::mutexFunc()
{
    uint32_t i;
    bool flag{true};
    while( flag ) {
        _m.lock();
        i = _index_m;
        ++_index_m;
        _m.unlock();
        if( i < NumTasks ) {
            ++_array[i];
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::mutexSleepingFunc()
{
    uint32_t i;
    bool flag{true};
    while( flag ) {
        _m.lock();
        i = _index_m;
        ++_index_m;
        _m.unlock();
        if( i < NumTasks ) {
            ++_array[i];
            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        }
        else {
            flag = false;
        }
    }
}
