#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <immintrin.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include<vector>
#include <memory> 
#include<omp.h>
#include <thread>
#include<cblas.h>
using namespace std;
using namespace std::literals;
struct matrixs 
{
	int rownum;
	int linenum;
	float **ma;
};

float** multi4kernel(float **c, float **a, float **b, int ROW, int row, int col) {
	register float t0(0), t1(0), t2(0), t3(0), t4(0), t5(0), t6(0), t7(0),
		t8(0), t9(0), t10(0), t11(0), t12(0), t13(0), t14(0), t15(0);
	float *a0(a[0]), *a1(a[1]), *a2(a[2]), *a3(a[3]),
		*b0(b[col]), *b1(b[col + 1]), *b2(b[col + 2]), *b3(b[col + 3]), *end = b0 + ROW;
	do {
		t0 += *(a0)**(b0);
		t1 += *(a0)**(b1);
		t2 += *(a0)**(b2);
		t3 += *(a0++)**(b3);
		t4 += *(a1)**(b0);
		t5 += *(a1)**(b1);
		t6 += *(a1)**(b2);
		t7 += *(a1++)**(b3);
		t8 += *(a2)**(b0);
		t9 += *(a2)**(b1);
		t10 += *(a2)**(b2);
		t11 += *(a2++)**(b3);
		t12 += *(a3)**(b0++);
		t13 += *(a3)**(b1++);
		t14 += *(a3)**(b2++);
		t15 += *(a3++)**(b3++);
	} while (b0 != end);
	c[col][row] = t0;
	c[col + 1][row] = t1;
	c[col + 2][row] = t2;
	c[col + 3][row] = t3;
	c[col][row + 1] = t4;
	c[col + 1][row + 1] = t5;
	c[col + 2][row + 1] = t6;
	c[col + 3][row + 1] = t7;
	c[col][row + 2] = t8;
	c[col + 1][row + 2] = t9;
	c[col + 2][row + 2] = t10;
	c[col + 3][row + 2] = t11;
	c[col][row + 3] = t12;
	c[col + 1][row + 3] = t13;
	c[col + 2][row + 3] = t14;
	c[col + 3][row + 3] = t15;
	return c;
}
float** dotmultiplication(float **a, float **b, int q, int w, int e)
{
	float **sum;
	sum = new float*[q];
	for (int i = 0; i < q; i++)
	{
		sum[i] = new float[w + 1];
	}
	float *tr[4];
	int i(0), j(0);
	do {
		tr[i++] = (float*)malloc(sizeof(float)*w);
	} while (i < 4);
	do {
		i = 0;
		do {
			tr[0][i] = a[i][j];//packing过程，把行数据打包到连续空间
			tr[1][i] = a[i][j + 1];
			tr[2][i] = a[i][j + 2];
			tr[3][i] = a[i][j + 3];
		} while ((++i) < w);
		i = 0;
		do {
			multi4kernel(sum, tr, b, q, j, i);
			i += 4;
		} while (i < w);
		j += 4;
	} while (j < q);
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
		cout << "Incorrect matrix format" << endl;
	}
	else
	{
		a.linenum = n1;
		a.rownum = m1;
		b.linenum = n2;
		b.rownum = m2;
		a.ma = new float*[n1  + 1];
		for (int i = 0; i < n1; i++)
		{
			a.ma[i] = new float[m1 + 1];
		}
		b.ma = new float*[n2  + 1];
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
		sum.ma = dotmultiplication(a.ma, b.ma, n1, m1, m2);

		auto end1 = std::chrono::steady_clock::now();
		
		std::cout
			<< "Time of calculation: "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << "us ≈ "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms ≈ "
			<< std::chrono::duration_cast<std::chrono::seconds>(end1 - start1).count() << "s.\n";
		delete a.ma;
		delete b.ma;
		delete sum.ma;



	}

	return 0;
}
