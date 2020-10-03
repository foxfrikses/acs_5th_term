## First lab

I need to use assembly inline and MMX in C++ code.
My code must calculate _F = A * (B + C) - D_, where **A**, **B**, **C** type is *int8_t[8]*, and **D**, **F** type is *int16_t[8]*

What does the code do:

0. Read **A**, **B**, **C** and **D** from **input.txt**
0. Calculate _F_ using MMX and output it into **output.txt**
0. Calculate _F_ without MMX and output it into **expected_output.txt**

__input.txt__ (format example):  
>1 2 3 4 5 6 7 8  
>9 10 11 12 13 14 15 16  
>17 18 19 20 21 22 23 24  
>25 26 27 28 29 30 31 32

