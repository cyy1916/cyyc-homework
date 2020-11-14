#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>
#include<vector>
#include <memory> 
#include<omp.h>
#include <thread>
using namespace std;
using namespace std::literals;
struct matrixs
{
	int rownum;
	int linenum;
	float **ma;
};

matrixs dotmultiplication(float **a, float **b, int i, int j, int k)
{
	matrixs sum;
	sum.linenum = i;
	sum.rownum = k;
	sum.ma = new float*[i];
	for (int n = 0; n < i; n++)
	{
		sum.ma[n] = new float[k];
	}
	float s = 0;
#pragma omp parallel for
	for (int n = 0; n < i; n++)
	{
		for (int p = 0; p < j; p++)
		{
			s = a[n][p];
			for (int m = 0; m < k; m++)
			{
				sum.ma[n][m] += s * b[p][m];
			}
		}
	}
	return sum;
}

float* data(int n)
{
	float *a = new float[n + 1]{ 0 };
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() + rand() / float(RAND_MAX);
	}
	return a;
}

int main(int argc, const char * argv[])
{
	auto now = std::chrono::system_clock::now();
	auto t_c = std::chrono::system_clock::to_time_t(now - 24h);
	matrixs a;
	matrixs b;
	int n1 = 0;
	int m1 = 0;
	int n2 = 0;
	int m2 = 0;
	matrixs sum;
	cin >> n1;
	cin >> m1;
	cin >> n2;
	cin >> m2;
	if (m1 != n2)
	{
		cout << "矩阵格式有误" << endl;
	}
	else
	{
		a.linenum = n1;
		a.rownum = m1;
		b.linenum = n2;
		b.rownum = m2;
		a.ma = new float*[n1 + 1];
		for (int i = 0; i < n1; i++)
		{
			a.ma[i] = new float[m1 + 1];
		}
		b.ma = new float*[n2 + 1];
		for (int i = 0; i < n2; i++)
		{
			b.ma[i] = new float[m2 + 1];
		}
		for (int i = 0; i < n1; i++)
		{
			srand((unsigned int)(time(NULL)));
			a.ma[i] = data(m1);
		}
		for (int i = 0; i < n2; i++)
		{
			srand((unsigned int)(time(NULL)));
			b.ma[i] = data(m2);
		}
		auto start1 = std::chrono::steady_clock::now();

		sum = dotmultiplication(a.ma, b.ma, n1, m1, m2);
		auto end1 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(ikj)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end1 - start1).count() << "s.\n";
		
		
	}


}
