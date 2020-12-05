#pragma once
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
using namespace std;
using namespace std::literals;
class matrix
{
private:
	int row;
	int line;
	float **data;
	bool hasdestroy;
public:
	matrix();
	matrix(int row, int line);
	matrix(int row, int line, float **data);
	matrix(int row, int line, float num);
	~matrix();
	void operator = (const matrix& other);
	void operator = (float real);
	matrix operator * (const matrix& A);
	matrix operator * (float c);
	friend matrix operator * (float real, const matrix& other);
	friend std::ostream& operator << (std::ostream& os, const matrix& other);
};
float** multi4kernel(float **c, float **a, float **b, int COL, int row, int col) {
	__m128 t00, t01, t02, t03, t10, t11, t12, t13,
		a0, a1, b0, b1, b2, b3;
	t00 = t01 = t02 = t03 = t10 = t11 = t12 = t13 = _mm_set1_ps(0);
	float *pb0(b[col]), *pb1(b[col + 1]), *pb2(b[col + 2]), *pb3(b[col + 3]), *pa0(a[0]), *pa1(a[1]), *endb0 = pb0 + COL;
	do {
		a0 = _mm_load_ps(pa0);
		a1 = _mm_load_ps(pa1);
		b0 = _mm_set1_ps(*(pb0++));
		b1 = _mm_set1_ps(*(pb1++));
		b2 = _mm_set1_ps(*(pb2++));
		b3 = _mm_set1_ps(*(pb3++));
		t00 = _mm_add_ps(t00, _mm_mul_ps(a0, b0));
		t01 = _mm_add_ps(t01, _mm_mul_ps(a0, b1));
		t02 = _mm_add_ps(t02, _mm_mul_ps(a0, b2));
		t03 = _mm_add_ps(t03, _mm_mul_ps(a0, b3));
		t10 = _mm_add_ps(t10, _mm_mul_ps(a1, b0));
		t11 = _mm_add_ps(t11, _mm_mul_ps(a1, b1));
		t12 = _mm_add_ps(t12, _mm_mul_ps(a1, b2));;
		t13 = _mm_add_ps(t13, _mm_mul_ps(a1, b3));;
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

matrix::matrix()
{
	row = 0;
	line = 0;
	hasdestroy = true;
}
matrix::matrix(int row, int line)
{
	this->row = row;
	this->line = line;
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[line] {0};
	}
	hasdestroy = false;
}
matrix::matrix(int row, int line, float **data)
{
	this->row = row;
	this->line = line;
	this->data = data;
	hasdestroy = true;
}
matrix::matrix(int row, int line, float num)
{
	this->row = row;
	this->line = line;
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[line] ;
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < line; j++)
		{
			data[i][j] = num;
		}
	}
	hasdestroy = false;
}
matrix::~matrix()
{
	if (!hasdestroy)
	{
		delete[] data;
		hasdestroy = true;
	}
}
void matrix::operator = (const matrix& other)
{
	line = other.line;
	row = other.row;
	data = other.data;
	hasdestroy = true;
}

void matrix::operator = (float real)
{
	line = 1;
	row = 1;
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[line] {real};
	}
	hasdestroy = false;
}
matrix matrix::operator*(const matrix& other) 
{
	return matrix(row, other.line, dotmultiplication(data, other.data, row, line, other.line));
}

matrix matrix::operator*(float c)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < line; j++)
		{
			data[i][j] *= c;
		}
	}
	return matrix(row, line, data);
}

matrix operator*(float real, const matrix& other)
{
	for (int i = 0; i < other.row; i++)
	{
		for (int j = 0; j < other.line; j++)
		{
			other.data[i][j] *= real;
		}
	}
	return other;
}

std::ostream& operator<< (std::ostream& os, const matrix& other)
{
	for (int i = 0; i < other.row; i++)
	{
		for (int j = 0; j < other.line; j++)
		{
			os << other.data[i][j] << " ";
		}
		os << endl;
	}
	return os;
}

