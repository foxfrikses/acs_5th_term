#include "tasks.h"
#include "arraytask.h"
#include "queuetask.h"
#include <fstream>
#include <iostream>

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


void result_1_2() { showResult( "output_1_2.txt" ); }
void result_1_3() { showResult( "output_1_3.txt" ); }
void result_2_1() { showResult( "output_2_1.txt" ); }
void result_2_2() { showResult( "output_2_2.txt" ); }
void result_2_3() { showResult( "output_2_3.txt" ); }

void saveResult( std::string str, std::string filename ){
    std::ofstream fout;
    fout.open(filename);
    if( fout.is_open() ){
        fout << str;
        fout.close();
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
        arrTsk.set_num_tasks(nTsk);
        arrTsk.set_num_threads(nThr);
        if( arrTsk.run( ArrayTask::task::atomic ) )
            a = arrTsk.get_duration();
        if( arrTsk.run( ArrayTask::task::mutex ) )
            m = arrTsk.get_duration();
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
    uint32_t nTsk = 1024 * 1024;
    ArrayTask arrTsk;
    result  = "With sleeping";
    result += "\nNumTasks = " + std::to_string( nTsk );
    result += "\nNumThreads = {4, 8, 16, 32}\n\n";
    for( auto nThr = 4u; nThr <= 32u; nThr *= 2u ) {
        result += std::to_string( nThr ) + " threads";
        uint64_t a{ 0u }, m{ 0u };
        arrTsk.set_num_tasks( nTsk );
        arrTsk.set_num_threads( nThr );
        if( arrTsk.run( ArrayTask::task::atomic_with_sleep ) )
            a = arrTsk.get_duration();
        if( arrTsk.run( ArrayTask::task::mutex_with_sleep ) )
            m = arrTsk.get_duration();
        result += "\n\tatomic - "
                  + std::to_string( a ) +
                  " ms"
                  "\n\tmutex  - "
                  + std::to_string( m ) +
                  " ms\n\n";
    }

    saveResult( result, "output_1_3.txt" );
    showResult( "output_1_3.txt" );
}


const uint prNumCnt{ 3u };
const uint coNumCnt{ 3u };
const uint quSizCnt{ 3u };
const uint32_t TaskNum = 4 * 1024 * 1024;
const uint32_t ProducerNum[prNumCnt]{ 1, 2, 4  };
const uint32_t ConsumerNum[coNumCnt]{ 1, 2, 4  };
const uint32_t QueueSize  [quSizCnt]{ 1, 4, 16 };

template <size_t SIZE>
void saveResult( std::string str[coNumCnt+1][prNumCnt+1][SIZE], std::string* table_names, std::string filename ){
    std::ofstream fout;
    fout.open(filename);
    if( fout.is_open() ){
        fout << "NumTasks = " + std::to_string(TaskNum) << "\n\n";
        for( size_t table = 0; table < SIZE; ++table ){
            fout << table_names[table] << '\n';
            fout << "Consumers\t\t";
            fout << "Produsers\n";
            for( size_t i = 0; i <= coNumCnt; ++i)
            {
                fout << str[i][0][table] << "\t\t";
                for( size_t j = 1; j < prNumCnt; ++j)
                    fout << str[i][j][table] << '\t';
                fout << str[i][prNumCnt][table];
                fout << '\n';
            }
            fout << '\n';
        }
        fout.close();
    }
}

void task_2_1(){
    std::string result[coNumCnt+1][prNumCnt+1][1];
    for( size_t i = 1; i <= coNumCnt; ++i )
        result[i][0][0] = std::to_string(ConsumerNum[i - 1]);
    for( size_t i = 1; i <= prNumCnt; ++i )
        result[0][i][0] = std::to_string(ProducerNum[i - 1]);

    QueueTask queTsk;


    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            queTsk.setTaskNum(TaskNum);
            queTsk.setProducerNum(ProducerNum[pr]);
            queTsk.setConsumerNum(ConsumerNum[co]);

            if( queTsk.run(QueueTask::task::dynamic) ) {
                uint32_t max_elem = 0u;
                for( size_t i = 0; i < ProducerNum[pr]; ++i )
                    if( queTsk.getDurations().produsers[i] > max_elem )
                        max_elem = queTsk.getDurations().produsers[i];

                for( size_t i = 0; i < ConsumerNum[co]; ++i )
                    if( queTsk.getDurations().consumers[i] > max_elem )
                        max_elem = queTsk.getDurations().consumers[i];


                result[co + 1][pr + 1][0] = std::to_string(max_elem);
            }
            else
            {
                result[co + 1][pr + 1][0] = "error";
            }
        }
    }
    std::string table_names[1];
    table_names[0] = "Dynamic queue";

    saveResult( result, table_names, "output_2_1.txt" );
    showResult( "output_2_1.txt" );
}

void task_2_2(){
    std::string result[coNumCnt+1][prNumCnt+1][quSizCnt];
    for( size_t k = 0; k < quSizCnt; ++k )
    {
        for( size_t i = 1; i <= coNumCnt; ++i )
            result[i][0][k] = std::to_string(ConsumerNum[i - 1]);
        for( size_t i = 1; i <= prNumCnt; ++i )
            result[0][i][k] = std::to_string(ProducerNum[i - 1]);
    }

    QueueTask queTsk;


    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            for( auto k = 0u; k < quSizCnt; ++k ){

                queTsk.setTaskNum(TaskNum);
                queTsk.setProducerNum(ProducerNum[pr]);
                queTsk.setConsumerNum(ConsumerNum[co]);

                if( queTsk.run(QueueTask::task::fixed_mutex, QueueSize[k]) ) {
                    uint32_t max_elem = 0u;
                    for( size_t i = 0; i < ProducerNum[pr]; ++i )
                        if( queTsk.getDurations().produsers[i] > max_elem )
                            max_elem = queTsk.getDurations().produsers[i];

                    for( size_t i = 0; i < ConsumerNum[co]; ++i )
                        if( queTsk.getDurations().consumers[i] > max_elem )
                            max_elem = queTsk.getDurations().consumers[i];


                    result[co + 1][pr + 1][k] = std::to_string(max_elem);
                }
                else
                {
                    result[co + 1][pr + 1][k] = "error";
                }
            }
        }
    }
    std::string table_names[quSizCnt];
    for( size_t i = 0; i < quSizCnt; ++i )
        table_names[i] = "Mutex queue with size " + std::to_string(QueueSize[i]);

    saveResult( result, table_names, "output_2_2.txt" );
    showResult( "output_2_2.txt" );
}

void task_2_3(){
    std::string result[coNumCnt+1][prNumCnt+1][quSizCnt];
    for( size_t k = 0; k < quSizCnt; ++k )
    {
        for( size_t i = 1; i <= coNumCnt; ++i )
            result[i][0][k] = std::to_string(ConsumerNum[i - 1]);
        for( size_t i = 1; i <= prNumCnt; ++i )
            result[0][i][k] = std::to_string(ProducerNum[i - 1]);
    }

    QueueTask queTsk;


    for( auto pr = 0u; pr < prNumCnt; ++pr ) {
        for( auto co = 0u; co < coNumCnt; ++co ){
            for( auto k = 0u; k < quSizCnt; ++k ){

                queTsk.setTaskNum(TaskNum);
                queTsk.setProducerNum(ProducerNum[pr]);
                queTsk.setConsumerNum(ConsumerNum[co]);

                if( queTsk.run(QueueTask::task::fixed_atomic, QueueSize[k]) ) {
                    uint32_t max_elem = 0u;
                    for( size_t i = 0; i < ProducerNum[pr]; ++i )
                        if( queTsk.getDurations().produsers[i] > max_elem )
                            max_elem = queTsk.getDurations().produsers[i];

                    for( size_t i = 0; i < ConsumerNum[co]; ++i )
                        if( queTsk.getDurations().consumers[i] > max_elem )
                            max_elem = queTsk.getDurations().consumers[i];


                    result[co + 1][pr + 1][k] = std::to_string(max_elem);
                }
                else
                {
                    result[co + 1][pr + 1][k] = "error";
                }
            }
        }
    }

    std::string table_names[quSizCnt];
    for( size_t i = 0; i < quSizCnt; ++i )
        table_names[i] = "Atomic queue with size " + std::to_string(QueueSize[i]);

    saveResult( result, table_names, "output_2_3.txt" );
    showResult( "output_2_3.txt" );
}
