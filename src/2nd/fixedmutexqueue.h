#ifndef FIXEDMUTEXQUEUE_H
#define FIXEDMUTEXQUEUE_H

#include <inttypes.h>
#include <mutex>
#include <condition_variable>

class FixedMutexQueue
{
protected:
    std::mutex _m;
    std::condition_variable not_filled_cond;
    uint8_t *array;
    uint size;
    uint pop_i = 0;
    uint push_i = 0;

public:
    void push(uint8_t val);
    bool pop(uint8_t& val);

    FixedMutexQueue(uint size);
    ~FixedMutexQueue();
};

#endif // FIXEDMUTEXQUEUE_H
