#include "fixedmutexqueue.h"
#include <thread>

FixedMutexQueue::FixedMutexQueue(uint32_t size)
{
    _curSize = 0;
    _maxSize = size;
    _popInd = 0;
    _array = new uint8_t[size];
}

void FixedMutexQueue::push(uint8_t val)
{
    std::unique_lock lck(_m);
    _not_full_cond.wait( lck, [this]{ return !_full; } );

    ++_curSize;
    _array[(_popInd + _curSize) % _maxSize] = val;
    _full  = (_curSize == _maxSize);
    _empty = false;
    _not_empty_cond.notify_one();
}

bool FixedMutexQueue::pop(uint8_t& val)
{
    std::unique_lock lck(_m);
    if( !_not_empty_cond.wait_for( lck,
                                   std::chrono::milliseconds(10),
                                   [this]{ return !_empty; } ) )
        return false;

    val = _array[_popInd];
    _array[_popInd] = 0;
    _popInd = (_popInd + 1)%_maxSize;
    --_curSize;
    _full  = false;
    _empty = !_curSize;
    _not_full_cond.notify_one();
    return true;
}

FixedMutexQueue::~FixedMutexQueue(){
    delete []_array;
}
