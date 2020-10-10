#ifndef FIXEDMUTEXQUEUE_H
#define FIXEDMUTEXQUEUE_H

#include <inttypes.h>
#include <mutex>
#include <condition_variable>

#include "iqueue.h"

class FixedMutexQueue : IQueue
{
protected:
    std::mutex _m;
    std::condition_variable not_filled_cond;
    uint8_t *array;
    uint size;
    uint pop_i = 0;
    uint push_i = 0;

public:
    virtual void push(uint8_t val) override;
    virtual bool pop(uint8_t& val) override;

    FixedMutexQueue(uint size);
    virtual ~FixedMutexQueue() override;
};

#endif // FIXEDMUTEXQUEUE_H
