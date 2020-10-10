#include "queuetask.h"

#include <thread>

bool QueueTask::run( task tsk, uint32_t queueSize )
{
    if( TaskNum == 0 ||
        ProducerNum == 0 ||
        ConsumerNum == 0 ||
        ( tsk != QueueTask::task::dynamic && !queueSize ) )
        return false;

    return false;
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
{

}

QueueTask::~QueueTask()
{

}

void QueueTask::consumerFunc()
{

}

void QueueTask::producerFunc()
{

}
