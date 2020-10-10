#ifndef FIXEDATOMICQUEUE_H
#define FIXEDATOMICQUEUE_H

#include <iqueue.h>

class FixedAtomicQueue : IQueue
{
public:
    virtual bool pop(uint8_t &val) override;
    virtual void push(uint8_t val) override;

    FixedAtomicQueue();
    virtual ~FixedAtomicQueue() override;
};

#endif // FIXEDATOMICQUEUE_H
