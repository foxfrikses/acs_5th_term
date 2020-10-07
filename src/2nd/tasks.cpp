#include "tasks.h"
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

void task_1_2(){
    std::string result = "12";

    saveResult( result, "output_1_2.txt" );
    showResult( "output_1_2.txt" );
}

void task_1_3(){
    std::string result = "13";

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
