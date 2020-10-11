#include "fixedmutexqueue.h"
#include <thread>

FixedMutexQueue::FixedMutexQueue(uint32_t size)
{
    this->size = size + 1u;
    array = new uint8_t[this->size];
}

void FixedMutexQueue::push(uint8_t val)
{
    std::unique_lock lck(_m);
    while( pop_i == (push_i + 1u)%size )
        not_filled_cond.wait( lck );

    array[push_i] = val;
    push_i = (push_i + 1u)%size;
}

bool FixedMutexQueue::pop(uint8_t& val)
{
    std::unique_lock lck(_m);
    if ( pop_i == push_i ){
        lck.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        lck.lock();
        if ( pop_i == push_i )
            return false;
    }
    val = array[pop_i];
    pop_i = (pop_i + 1u)%size;
    not_filled_cond.notify_one();
    lck.unlock();
    return true;
}

FixedMutexQueue::~FixedMutexQueue(){
    delete []array;
}
