#ifndef QUEUETASK_H
#define QUEUETASK_H

#include <inttypes.h>
#include "iqueue.h"

class QueueTask
{
public:
    enum class task : uint8_t {
        dynamic             = 0,
        fixed_mutex         = 1,
        fixed_atomic        = 2
    };

    bool run( task, uint32_t queueSize = 0u );
    void setProducerNum( uint32_t );
    void setConsumerNum( uint32_t );
    void setTaskNum( uint32_t );

    QueueTask();
    ~QueueTask();

protected:
    void consumerFunc();
    void producerFunc();

    uint32_t QueueSize{0};
    uint32_t ProducerNum{0};
    uint32_t ConsumerNum{0};
    uint32_t TaskNum{0};
    IQueue *queue{nullptr};
};

#endif // QUEUETASK_H
