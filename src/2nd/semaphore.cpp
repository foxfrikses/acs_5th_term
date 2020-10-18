#include "semaphore.h"

Semaphore::Semaphore( long init_count )
    : _count(init_count)
{}

Semaphore::~Semaphore(){
    disable();
}

void Semaphore::wait(){
    long count = _count--;
    if( count <= 0 )
        _sem.wait();
}

void Semaphore::signal(){
    long count = _count++;
    if( count < 0 )
        _sem.signal();
}

bool Semaphore::wait_for( std::chrono::milliseconds milliseconds ){
    long count = _count--;
    if( count <= 0 ){
        return _sem.wait_for( milliseconds );
    }
    return true;
}

void Semaphore::disable(){
    _sem.disable();
}
