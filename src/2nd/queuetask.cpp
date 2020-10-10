#include "queuetask.h"

#include "dynamicqueue.h"
#include "fixedmutexqueue.h"
#include "fixedatomicqueue.h"
#include <thread>
#include <cstring>

bool QueueTask::run( task tsk, uint32_t queueSize )
{
    if( TaskNum == 0 ||
        ProducerNum == 0 ||
        ConsumerNum == 0 ||
        ( tsk != QueueTask::task::dynamic && !queueSize ) )
        return false;

    switch (tsk) {
    case QueueTask::task::dynamic:
        queue = new DynamicQueue();
        break;
    case QueueTask::task::fixed_mutex:
        queue = new FixedMutexQueue(queueSize);
        break;
    case QueueTask::task::fixed_atomic:
        queue = new FixedAtomicQueue(queueSize);
        break;
    }

    std::thread producerThreads[ProducerNum];
    std::thread consumerThreads[ConsumerNum];
    uint32_t consumerResults[ConsumerNum];
    std::memset( consumerResults, 0u, sizeof(uint32_t) );
    for( auto i = 0u; i < ProducerNum; ++i )
        producerThreads[i] = std::thread(&QueueTask::producerFunc,
                                         this);

    for( auto i = 0u; i < ConsumerNum; ++i )
        consumerThreads[i] = std::thread(&QueueTask::consumerFunc,
                                         this,
                                         std::ref(consumerResults[i]) );

    for( auto i = 0u; i < ProducerNum; ++i )
        producerThreads[i].join();

    for( auto i = 0u; i < ConsumerNum; ++i )
        consumerThreads[i].join();

    uint32_t res{0};
    for( auto i = 0u; i < ConsumerNum; ++i )
        res += consumerResults[i];

    delete queue;
    queue = nullptr;
    if( res == TaskNum*ProducerNum )
        return true;
    else return false;
}

void QueueTask::setProducerNum( uint32_t ProducerNum )
{
    this->ProducerNum = ProducerNum;
}

void QueueTask::setConsumerNum( uint32_t ConsumerNum )
{
    this->ConsumerNum = ConsumerNum;
}

void QueueTask::setTaskNum( uint32_t TaskNum )
{
    this->TaskNum = TaskNum;
}

QueueTask::QueueTask()
{}

QueueTask::~QueueTask()
{}

void QueueTask::consumerFunc( uint64_t &result )
{
    uint32_t localResult{0};
    uint8_t iterarionResult;
    while( queue->pop(iterarionResult) )
        localResult += iterarionResult;
}

void QueueTask::producerFunc()
{
    for( auto i = 0u; i < TaskNum; ++i )
        queue->push(1u);
}
