#include "poor_semaphore.h"
#include <chrono>

PoorSemaphore::PoorSemaphore()
    : _count(0),
      _is_disabled(false)
{}

PoorSemaphore::~PoorSemaphore(){
   disable();
}

void PoorSemaphore::signal(){
    std::lock_guard<std::mutex> lck(_m);
    ++_count;
    _cv.notify_one();
}

void PoorSemaphore::wait(){
    std::unique_lock<std::mutex> lck(_m);

    while( !_count ) {
        _cv.wait(lck);
        if( _is_disabled.load(std::memory_order_acquire) )
            return;
    }
    --_count;
    lck.unlock();
}

bool PoorSemaphore::wait_for( std::chrono::milliseconds milliseconds ){
    std::unique_lock<std::mutex> lck(_m);

    auto until_time = std::chrono::steady_clock::now() + milliseconds;
    while( !_count ) {
        if( _cv.wait_until( lck, until_time ) != std::cv_status::timeout )
        {
            if( _is_disabled.load(std::memory_order_acquire) )
            {
                lck.unlock();
                return false;
            }
        }
        else
        {
            lck.unlock();
            return false;
        }
    }
    --_count;
    lck.unlock();
    return true;
}

void PoorSemaphore::disable(){
    _is_disabled.store(true, std::memory_order::memory_order_release);
    _cv.notify_all();
}
