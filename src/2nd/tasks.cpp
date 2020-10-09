#include "tasks.h"
#include "arraytask.h"
#include <fstream>
#include <iostream>

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
    std::string result = "21";

    saveResult( result, "output_2_1.txt" );
    showResult( "output_2_1.txt" );
}

void task_2_2(){
    std::string result = "22";

    saveResult( result, "output_2_2.txt" );
    showResult( "output_2_2.txt" );
}

void task_2_3(){
    std::string result = "23";

    saveResult( result, "output_2_3.txt" );
    showResult( "output_2_3.txt" );
}

void result_1_2() { showResult( "output_1_2.txt" ); }
void result_1_3() { showResult( "output_1_3.txt" ); }
void result_2_1() { showResult( "output_2_1.txt" ); }
void result_2_2() { showResult( "output_2_2.txt" ); }
void result_2_3() { showResult( "output_2_3.txt" ); }
