#include "arraytask.h"

#include <thread>
#include <chrono>

ArrayTask::ArrayTask()
    : _num_tasks(1),
      _num_threads(1),
      _array(1)
{}

ArrayTask::~ArrayTask()
{}

void ArrayTask::set_num_tasks( uint32_t num_tasks ){
    _num_tasks = (num_tasks) ? num_tasks : 1;
    _array.resize(num_tasks);
}

void ArrayTask::set_num_threads( uint32_t num_threads ){
    _num_threads = (num_threads) ? num_threads : 1;
}

uint64_t ArrayTask::get_duration(){
   return _duration;
}

bool ArrayTask::run( ArrayTask::task tsk ){
    using namespace std::chrono;
    using task = ArrayTask::task;

    for( auto &elem : _array )
        elem = 0;

    _index_a = 0;
    _index_m = 0;

    void (ArrayTask::*func)(void);
    switch (tsk) {
    case task::atomic:
        func = &ArrayTask::atomic_func;
        break;
    case task::atomic_with_sleep:
        func = &ArrayTask::atomic_sleeping_func;
        break;
    case task::mutex:
        func = &ArrayTask::mutex_func;
        break;
    case task::mutex_with_sleep:
        func = &ArrayTask::mutex_sleeping_func;
        break;
    }

    std::thread threads[_num_threads];
    auto start{ high_resolution_clock::now() };

    for( auto i = 0u; i < _num_threads; ++i )
        threads[i] = std::thread( func, this );
    for( auto i = 0u; i < _num_threads; ++i )
        threads[i].join();

    auto end{ high_resolution_clock::now() };
    _duration = duration_cast<milliseconds>( end - start ).count();

    for( auto elem : _array )
        if ( elem != 1 )
            return false;

    return true;
}

void ArrayTask::atomic_func()
{
    uint32_t i;
    bool flag{ true };
    while( flag ) {
        i = _index_a.fetch_add( 1 );
        if( i < _num_tasks ) {
            ++_array[i];
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::atomic_sleeping_func()
{
    uint32_t i;
    bool flag{ true };
    while( flag ) {
        i = _index_a.fetch_add( 1 );
        if( i < _num_tasks ) {
            ++_array[i];
            std::this_thread::sleep_for( std::chrono::nanoseconds(10) );
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::mutex_func()
{
    uint32_t i;
    bool flag{ true };
    while( flag ) {
        _m.lock();
        i = _index_m;
        ++_index_m;
        _m.unlock();
        if( i < _num_tasks ) {
            ++_array[i];
        }
        else {
            flag = false;
        }
    }
}

void ArrayTask::mutex_sleeping_func()
{
    uint32_t i;
    bool flag{ true };
    while( flag ) {
        _m.lock();
        i = _index_m;
        ++_index_m;
        _m.unlock();
        if( i < _num_tasks ) {
            ++_array[i];
            std::this_thread::sleep_for( std::chrono::nanoseconds(10) );
        }
        else {
            flag = false;
        }
    }
}
