#include "queuetask.h"

#include "dynamicqueue.h"
#include "fixedmutexqueue.h"
#include "fixedatomicqueue.h"
#include <thread>
#include <cstring>

bool QueueTask::run( task tsk, uint32_t queueSize )
{
    if( _taskNum == 0 ||
        _producerNum == 0 ||
        _consumerNum == 0 ||
        ( tsk != QueueTask::task::dynamic && !queueSize ) )
        return false;

    switch (tsk) {
    case QueueTask::task::dynamic:
        _queue = new DynamicQueue();
        break;
    case QueueTask::task::fixed_mutex:
        _queue = new FixedMutexQueue(queueSize);
        break;
    case QueueTask::task::fixed_atomic:
        _queue = new FixedAtomicQueue(queueSize);
        break;
    }

    std::thread producerThreads[_producerNum];
    std::thread consumerThreads[_consumerNum];
    uint32_t    consumerResults[_consumerNum];
    std::memset( consumerResults, 0u, sizeof(uint32_t) * _consumerNum );
    _durations.produsers.clear();
    _durations.consumers.clear();
    _durations.produsers.resize(_producerNum);
    _durations.consumers.resize(_consumerNum);

    for( auto i = 0u; i < _producerNum; ++i )
        producerThreads[i] = std::thread(&QueueTask::producerFunc,
                                         this,
                                         std::ref(_durations.produsers[i]));
    for( auto i = 0u; i < _consumerNum; ++i )
        consumerThreads[i] = std::thread(&QueueTask::consumerFunc,
                                         this,
                                         std::ref(_durations.consumers[i]),
                                         std::ref(consumerResults[i]) );
    for( auto i = 0u; i < _consumerNum; ++i )
        consumerThreads[i].join();
    for( auto i = 0u; i < _producerNum; ++i )
        producerThreads[i].join();

    uint32_t res{0};
    for( auto i = 0u; i < _consumerNum; ++i )
        res += consumerResults[i];

    delete _queue;
    _queue = nullptr;
    return (res == _taskNum*_producerNum);
}

void QueueTask::setProducerNum( uint32_t ProducerNum )
{
    _producerNum = ProducerNum;
}

void QueueTask::setConsumerNum( uint32_t ConsumerNum )
{
    _consumerNum = ConsumerNum;
}

void QueueTask::setTaskNum( uint32_t TaskNum )
{
    _taskNum = TaskNum;
}

QueueTask::QueueTask()
{}

void QueueTask::consumerFunc( uint32_t& duration, uint32_t &result )
{
    using namespace std::chrono;
    auto start{high_resolution_clock::now()};

    uint32_t localResult{0};
    uint8_t iterarionResult;
    while( _queue->pop(iterarionResult) )
        localResult += iterarionResult;
    result = localResult;

    auto end{high_resolution_clock::now()};
    duration = duration_cast<milliseconds>(end - start).count();
}

void QueueTask::producerFunc( uint32_t& duration )
{
    using namespace std::chrono;
    auto start{high_resolution_clock::now()};

    for( auto i = 0u; i < _taskNum; ++i )
        _queue->push(1u);

    auto end{high_resolution_clock::now()};
    duration = duration_cast<milliseconds>(end - start).count();
}

QueueTask::Durations& QueueTask::getDurations(){
   return _durations;
}
