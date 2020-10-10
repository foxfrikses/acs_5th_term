#ifndef FIXEDATOMICQUEUE_H
#define FIXEDATOMICQUEUE_H

#include <iqueue.h>

class FixedAtomicQueue : public IQueue
{
public:
    virtual bool pop(uint8_t &val) override;
    virtual void push(uint8_t val) override;

    FixedAtomicQueue(uint32_t);
    virtual ~FixedAtomicQueue() override;
};

#endif // FIXEDATOMICQUEUE_H
