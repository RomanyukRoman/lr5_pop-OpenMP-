#include<iostream>
#include"omp.h"
#include <tuple>

using namespace std;

const int rows = 10000;
const int cols = 10000;

long long arr[rows][cols];

void init_arr() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			arr[i][j] = rand() % 100;
		}
	}
}

long long get_sum(int num_threads) {

	long long sum = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum += arr[i][j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "Get_Sum " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
	return sum;
}

std::tuple<int, long long> get_min_sum_row(int num_threads){

	long long min = INT_MAX;
	int min_index = -1;
	double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < rows; i++)
	{
		long long min_sum_row = 0;
		for (int j = 0; j < cols; j++)
		{
			min_sum_row += arr[i][j];
		}
		if (min_sum_row < min)
		{
#pragma omp critical
			if (min_sum_row < min)
			{
				min = min_sum_row;
				min_index = i;
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "Minimum sum: " << min << " row with min sum" << min_index << " threads" << num_threads << "worked - " << t2 - t1 << " seconds" << endl;
	return std::make_tuple(min_index, min);

}

int main() {

	int num_threads = 32;
	int step = 2;
	omp_set_nested(1);
	init_arr();
	double t1 = omp_get_wtime();
	int min_index, min;
#pragma omp parallel sections
	{

#pragma omp section
	{
		for (int i = 1; i <= num_threads; i *= step) 
		{
			std::cout << "Sum in thread " << i << " = " << get_sum(i) << std::endl;
		}
	}
#pragma omp section
	{
		for (int i = 1; i <= num_threads; i *= step)
		{
			tie(min_index, min) = get_min_sum_row(num_threads);
			std::cout << "Row with min sum " << min_index << " Min sum: " << min << " in thread " << i << endl;
		}
	}
	}
		double t2 = omp_get_wtime();
		std::cout << "Total time - " << t2 - t1 << "sec" << std::endl;
		return 0;
}