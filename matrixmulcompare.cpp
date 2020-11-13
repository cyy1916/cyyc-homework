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
matrixs dotmultiplication2(float **a, float **b, int i, int j, int k)
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

	for (int p = 0; p < j; p++)
	{
		for (int n = 0; n < i; n++)
		{
			for (int m = 0; m < k; m++)
			{
				sum.ma[n][m] += b[p][m] * a[n][p];
			}
		}
	}
	return sum;
}
matrixs dotmultiplication3(float **a, float **b, int i, int j, int k)
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

	for (int m = 0; m < k; m++)
	{
		for (int n = 0; n < i; n++)
		{

			for (int p = 0; p < j; p++)
			{
				sum.ma[n][m] += a[n][p] * b[p][m];
			}
		}
	}
	return sum;
}
matrixs dotmultiplication4(float **a, float **b, int i, int j, int k)
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

	for (int n = 0; n < i; n++)
	{
		for (int m = 0; m < k; m++)
		{
			for (int p = 0; p < j; p++)
			{
				sum.ma[n][m] += a[n][p] * b[p][m];
			}
		}
	}
	return sum;
}
matrixs dotmultiplication5(float **a, float **b, int i, int j, int k)
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

	for (int p = 0; p < j; p++)
	{
		for (int m = 0; m < k; m++)
		{
			s = b[p][m];
			for (int n = 0; n < i; n++)
			{
				sum.ma[n][m] += s * a[n][p];
			}
		}
	}
	return sum;
}
matrixs dotmultiplication6(float **a, float **b, int i, int j, int k)
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

	for (int m = 0; m < k; m++)
	{
		for (int p = 0; p < j; p++)
		{
			s = b[p][m];
			for (int n = 0; n < i; n++)
			{
				sum.ma[n][m] += s * a[n][p];
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
		auto start2 = std::chrono::steady_clock::now();

		sum = dotmultiplication2(a.ma, b.ma, n1, m1, m2);
		auto end2 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(kij)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end2 - start2).count() << "s.\n";
		auto start3 = std::chrono::steady_clock::now();

		sum = dotmultiplication3(a.ma, b.ma, n1, m1, m2);
		auto end3 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(jik)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end3 - start3).count() << "s.\n";
		auto start4 = std::chrono::steady_clock::now();

		sum = dotmultiplication4(a.ma, b.ma, n1, m1, m2);
		auto end4 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(ijk)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end4 - start4).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end4 - start4).count() << "s.\n";
		auto start5 = std::chrono::steady_clock::now();

		sum = dotmultiplication5(a.ma, b.ma, n1, m1, m2);
		auto end5 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(kji)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end5 - start5).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end5 - start5).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end5 - start5).count() << "s.\n";
		auto start6 = std::chrono::steady_clock::now();

		sum = dotmultiplication6(a.ma, b.ma, n1, m1, m2);
		auto end6 = std::chrono::steady_clock::now();
		std::cout
			<< "计算时间(jki)：  "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end6 - start6).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end6 - start6).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end6 - start6).count() << "s.\n";
	}


}
