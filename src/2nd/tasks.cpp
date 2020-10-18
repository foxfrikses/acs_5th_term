#include "tasks.h"
#include "arraytask.h"
#include "queuetask.h"
#include <fstream>
#include <iostream>

const uint prNumCnt{3u};
const uint coNumCnt{3u};
const uint quSizCnt{3u};
const uint32_t TaskNum = 1024*1024*4;
const uint32_t ProducerNum[prNumCnt]{1, 2, 4 };
const uint32_t ConsumerNum[coNumCnt]{1, 2, 4 };
const uint32_t QueueSize  [quSizCnt]{1, 4, 16};

void saveResult( std::string str, std::string filename ){
    std::ofstream fout;
    fout.open(filename);
    if( fout.is_open() ){
        fout << str;
        fout.close();
    }
}

void showResult( std::string filename ){
    std::ifstream fin;
    fin.open(filename);
    if( fin.is_open() ){
        system("clear");
        std::cout << fin.rdbuf();
        fin.close();
        int c;
        while((c = getchar()) != '\n' && c != EOF);
        std::cin.get();
    }
}

void task_1_2() {
    std::string result;
    uint32_t nTsk = 1024*1024;
    ArrayTask arrTsk;
    result  = "Without sleeping";
    result += "\nNumTasks = " + std::to_string(nTsk);
    result += "\nNumThreads = {4, 8, 16, 32}\n\n";
    for( auto nThr = 4u; nThr <= 32u; nThr *= 2u ) {
        result += std::to_string(nThr) + " threads";
        uint64_t a{0u}, m{0u};
        arrTsk.setNumsTasksThreads(nTsk, nThr);
        if( arrTsk.run( ArrayTask::task::atomic ) )
            a = arrTsk.getDuration();
        if( arrTsk.run( ArrayTask::task::mutex ) )
            m = arrTsk.getDuration();
        result += "\n\tatomic - "
                  + std::to_string(a) +
                  " ms"
                  "\n\tmutex  - "
                  + std::to_string(m) +
                  " ms\n\n";
    }

    saveResult( result, "output_1_2.txt" );
    showResult( "output_1_2.txt" );
}

void task_1_3(){
    std::string result;
    uint32_t nTsk = 1024*1024;
    ArrayTask arrTsk;
    result  = "With sleeping";
    result += "\nNumTasks = " + std::to_string(nTsk);
    result += "\nNumThreads = {4, 8, 16, 32}\n\n";
    for( auto nThr = 4u; nThr <= 32u; nThr *= 2u ) {
        result += std::to_string(nThr) + " threads";
        uint64_t a{0u}, m{0u};
        arrTsk.setNumsTasksThreads(nTsk, nThr);
        if( arrTsk.run( ArrayTask::task::atomic_with_sleep ) )
            a = arrTsk.getDuration();
        if( arrTsk.run( ArrayTask::task::mutex_with_sleep ) )
            m = arrTsk.getDuration();
        result += "\n\tatomic - "
                  + std::to_string(a) +
                  " ms"
                  "\n\tmutex  - "
                  + std::to_string(m) +
                  " ms\n\n";
    }

    saveResult( result, "output_1_3.txt" );
    showResult( "output_1_3.txt" );
}

void task_2_1(){
    std::string result;
    QueueTask queTsk;
    result  = "Dynamic size mutex queue";
    result += "\nTaskNum = " + std::to_string(TaskNum);
    result += "\nProducerNum = {1, 2, 4}";
    result += "\nConsumerNum = {1, 2, 4}\n\n";
    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            result += std::to_string(ProducerNum[pr]) + " producers\n";
            result += std::to_string(ConsumerNum[co]) + " consumers\n";
            queTsk.setTaskNum(TaskNum);
            queTsk.setProducerNum(ProducerNum[pr]);
            queTsk.setConsumerNum(ConsumerNum[co]);
            if( queTsk.run(QueueTask::task::dynamic) ) {
                result += "\n\tEverything is OK";
                for( auto i = 0u; i < ProducerNum[pr]; ++i )
                    result += "\n\t\tProd " + std::to_string(i) + ": " +
                              std::to_string(queTsk.getDurations().produsers[i]) +
                              " ms";
                result += "\n";
                for( auto i = 0u; i < ConsumerNum[co]; ++i )
                    result += "\n\t\tConsum " + std::to_string(i) + ": " +
                              std::to_string(queTsk.getDurations().consumers[i]) +
                              " ms";
                result += "\n\n";
            }
            else
                result += "\n\tThere is an error\n\n";
        }
    }

    saveResult( result, "output_2_1.txt" );
    showResult( "output_2_1.txt" );
}

void task_2_2(){
    std::string result;
    QueueTask queTsk;
    result  = "Fixed size mutex queue";
    result += "\nTaskNum = " + std::to_string(TaskNum);
    result += "\nProducerNum = {1, 2, 4}";
    result += "\nConsumerNum = {1, 2, 4}\n\n";
    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            for( auto k = 0u; k < quSizCnt; ++k ){
                result += std::to_string(ProducerNum[pr]) + " producers\n";
                result += std::to_string(ConsumerNum[co]) + " consumers\n";
                result += std::to_string(QueueSize  [k]) + " elements in array\n";
                queTsk.setTaskNum(TaskNum);
                queTsk.setProducerNum(ProducerNum[pr]);
                queTsk.setConsumerNum(ConsumerNum[co]);
                if( queTsk.run(QueueTask::task::fixed_mutex, QueueSize[k]) ) {
                    result += "\n\tEverything is OK";
                    for( auto i = 0u; i < ProducerNum[pr]; ++i )
                        result += "\n\t\tProd " + std::to_string(i) + ": " +
                                  std::to_string(queTsk.getDurations().produsers[i]) +
                                  " ms";
                    result += "\n";
                    for( auto i = 0u; i < ConsumerNum[co]; ++i )
                        result += "\n\t\tConsum " + std::to_string(i) + ": " +
                                  std::to_string(queTsk.getDurations().consumers[i]) +
                                  " ms";
                    result += "\n\n";
                }
                else
                 result += "\n\tThere is an error\n\n";
            }
        }
    }

    saveResult( result, "output_2_2.txt" );
    showResult( "output_2_2.txt" );
}

void task_2_3(){
    std::string result;

    QueueTask queTsk;
    result  = "Fixed size atomic queue";
    result += "\nTaskNum = " + std::to_string(TaskNum);
    result += "\nProducerNum = {1, 2, 4}";
    result += "\nConsumerNum = {1, 2, 4}\n\n";
    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            for( auto k = 0u; k < quSizCnt; ++k ){
                result += std::to_string(ProducerNum[pr]) + " producers\n";
                result += std::to_string(ConsumerNum[co]) + " consumers\n";
                result += std::to_string(QueueSize  [k]) + " elements in array\n";
                queTsk.setTaskNum(TaskNum);
                queTsk.setProducerNum(ProducerNum[pr]);
                queTsk.setConsumerNum(ConsumerNum[co]);
                if( queTsk.run(QueueTask::task::fixed_atomic, QueueSize[k]) ) {
                    result += "\n\tEverything is OK";
                    for( auto i = 0u; i < ProducerNum[pr]; ++i )
                        result += "\n\t\tProd " + std::to_string(i) + ": " +
                                  std::to_string(queTsk.getDurations().produsers[i]) +
                                  " ms";
                    result += "\n";
                    for( auto i = 0u; i < ConsumerNum[co]; ++i )
                        result += "\n\t\tConsum " + std::to_string(i) + ": " +
                                  std::to_string(queTsk.getDurations().consumers[i]) +
                                  " ms";
                    result += "\n\n";
                }
                else
                 result += "\n\tThere is an error\n\n";
            }
        }
    }

    saveResult( result, "output_2_3.txt" );
    showResult( "output_2_3.txt" );
}

void result_1_2() { showResult( "output_1_2.txt" ); }
void result_1_3() { showResult( "output_1_3.txt" ); }
void result_2_1() { showResult( "output_2_1.txt" ); }
void result_2_2() { showResult( "output_2_2.txt" ); }
void result_2_3() { showResult( "output_2_3.txt" ); }
