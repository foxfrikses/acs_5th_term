#include "fixedatomicqueue.h"

bool FixedAtomicQueue::pop(uint8_t &val)
{
    if( _is_disabled.load(std::memory_order::memory_order_acquire) )
        return false;

    if ( !_sem_num_to_pop.wait_for( std::chrono::milliseconds(10) ) ){
        _pop_running.clear();
        return false;
    }

    while( _pop_running.test_and_set() );

    val = (*_items)[_tail].load(std::memory_order::memory_order_acquire);
    _tail = (_tail + 1) % _max_size;
    
    _pop_running.clear();

    _sem_num_to_push.signal();
    return true;
}

void FixedAtomicQueue::push(uint8_t val)
{
    if( _is_disabled.load(std::memory_order::memory_order_acquire) )
        return;
    
    _sem_num_to_push.wait();

    while( _push_running.test_and_set() );

    (*_items)[_head].store(val, std::memory_order::memory_order_release);
    _head = (_head + 1) % _max_size;
    
    _push_running.clear();

    _sem_num_to_pop.signal();
}

FixedAtomicQueue::FixedAtomicQueue(size_t size)
    : _items( new std::vector<std::atomic<uint8_t>>(size) ),
      _head(0),
      _tail(0),
      _max_size{size},
      _sem_num_to_push(size),
      _sem_num_to_pop(0),
      _is_disabled(false)
{
}

FixedAtomicQueue::~FixedAtomicQueue(){
    _is_disabled.store(true, std::memory_order::memory_order_release);
    _sem_num_to_pop.disable();
    _sem_num_to_push.disable();
}
