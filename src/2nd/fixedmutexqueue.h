#ifndef FIXEDMUTEXQUEUE_H
#define FIXEDMUTEXQUEUE_H

#include <inttypes.h>
#include <mutex>
#include <condition_variable>

#include "iqueue.h"

class FixedMutexQueue : public IQueue
{
protected:
    uint32_t _maxSize;
    uint32_t _curSize;
    uint32_t _popInd;

    std::mutex _m;
    std::condition_variable _not_full_cond;
    std::condition_variable _not_empty_cond;

    uint8_t *_array;
    bool     _empty{true};
    bool     _full {false};

public:
    virtual void push(uint8_t val) override;
    virtual bool pop(uint8_t& val) override;

    FixedMutexQueue(uint32_t size);
    virtual ~FixedMutexQueue() override;
};

#endif // FIXEDMUTEXQUEUE_H
