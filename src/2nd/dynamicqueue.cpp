#include "dynamicqueue.h"
#include <thread>
#include <chrono>

void DynamicQueue::push(uint8_t val){
    _m.lock();
    deque::push_back(val);
    _m.unlock();
}

bool DynamicQueue::pop(uint8_t& val){
    _m.lock();
    if( deque::empty() ) {
        _m.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        _m.lock();
        if ( deque::empty() )
            return false;
    }
    val = deque::front();
    deque::pop_front();
    return true;
}

DynamicQueue::DynamicQueue(){

}
