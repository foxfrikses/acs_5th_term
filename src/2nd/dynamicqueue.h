#ifndef DYNAMICQUEUE_H
#define DYNAMICQUEUE_H

#include <deque>
#include <inttypes.h>
#include <mutex>

class DynamicQueue : protected std::deque<uint8_t>
{
protected:
    std::mutex _m;

public:
    void push(uint8_t val);
    bool pop(uint8_t& val);

    DynamicQueue();
};

#endif // DYNAMICQUEUE_H
