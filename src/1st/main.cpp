#include <fstream>

int main()
{
    int8_t  A[8];
    int8_t  B[8];
    int8_t  C[8];
    int16_t D[8];
    int16_t F[8];
    {
        int32_t temp{0};
        std::ifstream fin;
        fin.open("input.txt");
        if ( !fin.is_open() )
            return -1;

        for( int i = 0; i < 8; ++i ){
            fin >> temp;
            A[i] = temp;
        }
        for( int i = 0; i < 8; ++i ){
            fin >> temp;
            B[i] = temp;
        }
        for( int i = 0; i < 8; ++i ){
            fin >> temp;
            C[i] = temp;
        }
        for( int i = 0; i < 8; ++i ){
            fin >> temp;
            D[i] = temp;
        }
        fin.close();
    }

    // F[i]=A[i] * ( B[i] + C[i] ) - D[i] , i=1...8.
    // Реализованы операции с насыщением
    {
        __asm__ volatile
        (
            "movq      (%[B]),   %%mm0     "   "\n\t"
            "movq      (%[C]),   %%mm1     "   "\n\t"
            "paddsb    %%mm1,    %%mm0     "   "\n\t"   // mm0 = (B+C)[0;8)
            "movq      (%[A]),   %%mm1     "   "\n\t"   // mm1 = A[0;8)
            "pxor      %%mm2,    %%mm2     "   "\n\t"   // mm2 = 0x00
            "movq      %%mm0,    %%mm3     "   "\n\t"   // mm3 = (B+C)[0;8)
            "movq      %%mm1,    %%mm4     "   "\n\t"   // mm4 = A[0;8)
            "pcmpgtb   %%mm2,    %%mm3     "   "\n\t"   // mm3 = not sign(B+C)
            "pcmpgtb   %%mm2,    %%mm4     "   "\n\t"   // mm4 = not sign(A)
            "pcmpeqb   %%mm2,    %%mm2     "   "\n\t"   // mm2 = (-1 for _ in range(8))
            "pxor      %%mm2,    %%mm3     "   "\n\t"   // mm3 = sign(B+C)
            "pxor      %%mm2,    %%mm4     "   "\n\t"   // mm4 = sign(A)
            "movq      %%mm0,    %%mm2     "   "\n\t"   // mm2 = (B+C)[0;8)
            "punpcklbw %%mm3,    %%mm0     "   "\n\t"   // mm0 = (B+C)[0,4)
            "punpckhbw %%mm3,    %%mm2     "   "\n\t"   // mm2 = (B+C)[4,8)
            "movq      %%mm1,    %%mm3     "   "\n\t"   // mm1 = A[0;8)
            "punpcklbw %%mm4,    %%mm1     "   "\n\t"   // mm1 = A[0,4)
            "punpckhbw %%mm4,    %%mm3     "   "\n\t"   // mm3 = A[4,8)
            "pmullw    %%mm0,    %%mm1     "   "\n\t"   // mm1 = A(B+C)[0;4)
            "pmullw    %%mm3,    %%mm2     "   "\n\t"   // mm2 = A(B+C)[4;8)
            "movq      (%[D]),   %%mm0     "   "\n\t"   // mm0 = D[0;4)
            "movq      8(%[D]),  %%mm3     "   "\n\t"   // mm3 = D[4;8)
            "psubsw    %%mm0,    %%mm1     "   "\n\t"   // mm1 = (A(B+C)-D)[0;4)
            "psubsw    %%mm3,    %%mm2     "   "\n\t"   // mm2 = (A(B+C)-D)[4;8)
            "movq      %%mm1,    %[F]      "   "\n\t"   // mm1 -> F[0;4)
            "movq      %%mm2,    8%[F]     "   "\n\t"   // mm2 -> F[4;8)
            "emms                          "
            :[F]"=m"(F)
            :[A]"r"(A), [B]"r"(B), [C]"r"(C), [D]"r"(D)
            :"mm0", "mm1", "mm2", "mm3", "mm4", "memory"
        );
    }

    {
        std::ofstream fout;
        fout.open("output.txt");
        if ( !fout.is_open() )
            return -2;

        for( int i = 0; i < 8; ++i )
            fout << F[i] << '\n';
        fout.close();
    }

    {
        std::ofstream fout;
        fout.open("expected_output.txt"); // если не срабатывал механизм насыщения
        if ( !fout.is_open() )
            return -3;

        for( int i = 0; i < 8; ++i )
            fout << A[i]*((int)B[i] + C[i]) - D[i] << '\n';
        fout.close();
    }

    return 0;
}
