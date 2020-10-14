#include "omp.h"
#include <fstream>
#include <inttypes.h>
#include <chrono>

int main()
{
	uint32_t *A, *B;
	uint32_t count;
	uint32_t result = 0, expected_result = 0; 
	int i, num_threads;
	uint32_t exec_time, expected_exec_time;
	{
		std::ifstream fin("input.txt");
		fin >> count;
		A = new uint32_t[count];
		B = new uint32_t[count];
		for( i = 0; i < count; ++i )
			fin >> A[i];
		for( i = 0; i < count; ++i )
			fin >> B[i];
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		for(i = 0; i < count; ++i)
			expected_result += A[i] * B[count - i];

		auto finish_time = std::chrono::high_resolution_clock::now();
		expected_exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time).count();
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		#pragma omp parallel default(shared)
		{
			#pragma omp for private(i) schedule(static) reduction(+:result) nowait
			for(i = 0; i < count; ++i)
				result += A[i] * B[count - i];
			
			#pragma omp single nowait
			num_threads = omp_get_num_threads();
		
		}

		auto finish_time = std::chrono::high_resolution_clock::now();
		exec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time).count();
	}

	{
		std::ofstream fout("output.txt");
		fout << "result - " << result << "\nexec time - " << exec_time << "\nnumber of threads - " << num_threads;
		fout << "\n\nexpected result - " << expected_result << "\nexec time - " << expected_exec_time;
	}

	delete[] A;
	delete[] B;
}
