#ifndef QUEUETASK_H
#define QUEUETASK_H

#include <inttypes.h>
#include <vector>
#include "iqueue.h"

class QueueTask
{
public:
    enum class task : uint8_t {
        dynamic             = 0,
        fixed_mutex         = 1,
        fixed_atomic        = 2
    };
    struct Durations{
        std::vector<uint32_t> produsers;
        std::vector<uint32_t> consumers;
    };

    bool run( task, uint32_t queueSize = 0u );
    auto getDurations() -> Durations&;
    void setProducerNum( uint32_t );
    void setConsumerNum( uint32_t );
    void setTaskNum( uint32_t );

    QueueTask();

protected:
    void consumerFunc( uint32_t& duration, uint32_t &result );
    void producerFunc( uint32_t& duration );

    uint32_t  _queueSize{0};
    uint32_t  _producerNum{0};
    uint32_t  _consumerNum{0};
    uint32_t  _taskNum{0};
    IQueue   *_queue{nullptr};
    Durations _durations;
};

#endif // QUEUETASK_H
