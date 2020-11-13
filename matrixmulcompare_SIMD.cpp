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

float** multi4kernel(float **c, float **a, float **b, int COL, int row, int col) {
	__m128 t00, t01, t02, t03, t10, t11, t12, t13,
		a0, a1, b0, b1, b2, b3;
	t00 =  t01 =  t02 =  t03 =  t10 =  t11 = t12 =  t13 = _mm_set1_ps(0);
	float *pb0(b[col]), *pb1(b[col + 1]), *pb2(b[col + 2]), *pb3(b[col + 3]), *pa0(a[0]), *pa1(a[1]), *endb0 = pb0 + COL;
	do {
		a0 = _mm_load_ps(pa0);
		a1 = _mm_load_ps(pa1);
		b0 = _mm_set1_ps(*(pb0++));
		b1 = _mm_set1_ps(*(pb1++));
		b2 = _mm_set1_ps(*(pb2++));
		b3 = _mm_set1_ps(*(pb3++)); 
		t00 = _mm_add_ss (t00, _mm_mul_ps(a0, b0));
		t01 = _mm_add_ss(t01, _mm_mul_ps(a0, b1));
		t02 = _mm_add_ss(t02, _mm_mul_ps(a0, b2));
		t03 = _mm_add_ss(t03, _mm_mul_ps(a0, b3));
		t10 = _mm_add_ss(t10, _mm_mul_ps(a1, b0));
		t11 = _mm_add_ss(t11, _mm_mul_ps(a1, b1));
		t12 = _mm_add_ss(t12, _mm_mul_ps(a1, b2));;
		t13 = _mm_add_ss(t13, _mm_mul_ps(a1, b3));;
		pa0 += 2;
		pa1 += 2;
	} while (pb0 != endb0);
	_mm_store_ps(&c[col][row], t00);
	_mm_store_ps(&c[col + 1][row], t01);
	_mm_store_ps(&c[col + 2][row], t02);
	_mm_store_ps(&c[col + 3][row], t03);
	_mm_store_ps(&c[col][row + 2], t10);
	_mm_store_ps(&c[col + 1][row + 2], t11);
	_mm_store_ps(&c[col + 2][row + 2], t12);
	_mm_store_ps(&c[col + 3][row + 2], t13);
	return c;
}
float** dotmultiplication(float **a, float **b, int q, int w, int e)
{
	float *ta[2];
	ta[0] = (float*)malloc(sizeof(float) * 2 * w);
	ta[1] = (float*)malloc(sizeof(float) * 2 * w);
	float **sum;
	sum = new float*[q];
	for (int i = 0; i < q; i++)
	{
		sum[i] = new float[w + 1];
	}
	int i(0), j(0), k, t;
	do {
		k = 0; i = 0;
		do {
			ta[0][k] = a[i][j];
			ta[1][k++] = a[i][j + 2];
			ta[0][k] = a[i][j + 1];
			ta[1][k++] = a[i++][j + 3];
		} while (i < w);
		i = 0;
		do {
			multi4kernel(sum, ta, b, e, j, i);
			i += 4;
		} while (i < e);
		j += 4;
	} while (j < q);
	free(ta[0]);
	free(ta[1]);
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
