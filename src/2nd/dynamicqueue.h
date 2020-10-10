#ifndef DYNAMICQUEUE_H
#define DYNAMICQUEUE_H

#include <deque>
#include <inttypes.h>
#include <mutex>

#include "iqueue.h"

class DynamicQueue : protected std::deque<uint8_t>, IQueue
{
protected:
    std::mutex _m;

public:
    virtual void push(uint8_t val) override;
    virtual bool pop(uint8_t& val) override;

    DynamicQueue();
};

#endif // DYNAMICQUEUE_H
