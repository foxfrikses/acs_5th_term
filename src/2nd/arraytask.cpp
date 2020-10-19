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
    bool is_mutex   = tsk == task::mutex            || tsk == task::mutex_with_sleep;
    bool with_sleep = tsk == task::mutex_with_sleep || tsk == task::atomic_with_sleep;

    std::thread threads[_num_threads];
    auto start{ high_resolution_clock::now() };

    if( is_mutex )
        for( auto i = 0u; i < _num_threads; ++i )
           threads[i] = std::thread( &ArrayTask::_func<uint32_t>, this, std::ref(_index_m), is_mutex, with_sleep );
    else
        for( auto i = 0u; i < _num_threads; ++i )
           threads[i] = std::thread( &ArrayTask::_func<std::atomic<uint32_t>>, this, std::ref(_index_a), is_mutex, with_sleep );

    for( auto i = 0u; i < _num_threads; ++i )
        threads[i].join();

    auto end{ high_resolution_clock::now() };
    _duration = duration_cast<milliseconds>( end - start ).count();

    for( auto elem : _array )
        if ( elem != 1 )
            return false;

    return true;
}

template < class T >
void ArrayTask::_func( T& counter, bool is_mutex, bool with_sleep )
{
    uint32_t i;
    bool continue_flag{ true };
    while( continue_flag ) {
        if( is_mutex ) _m.lock();
        i = counter++;
        if( is_mutex ) _m.unlock();

        if( i < _num_tasks ) {
            ++_array[i];
            if( with_sleep )
                std::this_thread::sleep_for( std::chrono::nanoseconds(10) );
        }
        else
            continue_flag = false;
    }
}
