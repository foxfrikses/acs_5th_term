#ifndef IQUEUE_H
#define IQUEUE_H

#include <inttypes.h>

class IQueue{
public:
    virtual void push(uint8_t val) = 0;
    virtual bool pop(uint8_t& val) = 0;
    virtual ~IQueue(){};
};

#endif // IQUEUE_H
