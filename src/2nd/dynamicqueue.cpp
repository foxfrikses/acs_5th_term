#include "dynamicqueue.h"
#include <thread>
#include <chrono>

void DynamicQueue::push(uint8_t val){
    std::unique_lock lck(_m);
    deque::push_back(val);
}

bool DynamicQueue::pop(uint8_t& val){
    std::unique_lock lck(_m);
    if( deque::empty() ) {
        lck.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        lck.lock();
        if ( deque::empty() )
            return false;
    }
    val = deque::front();
    deque::pop_front();
    return true;
}

DynamicQueue::DynamicQueue(){

}
