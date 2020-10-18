#ifndef FIXEDATOMICQUEUE_H
#define FIXEDATOMICQUEUE_H

#include <iqueue.h>
#include "semaphore.h"

#include <atomic>
#include <vector>

class FixedAtomicQueue : public IQueue
{
protected:
    std::vector<std::atomic<uint8_t>>*  _items;

    std::size_t _head; // push here
    std::size_t _tail; // pop here
    std::size_t _max_size;

    Semaphore _sem_num_to_push;
    Semaphore _sem_num_to_pop;

    std::atomic_flag _pop_running  = ATOMIC_FLAG_INIT;
    std::atomic_flag _push_running = ATOMIC_FLAG_INIT;
    std::atomic<bool> _is_disabled;

public:
    virtual bool pop(uint8_t &val) override;
    virtual void push(uint8_t val) override;

    FixedAtomicQueue(size_t size);
    virtual ~FixedAtomicQueue() override;
};

#endif // FIXEDATOMICQUEUE_H
