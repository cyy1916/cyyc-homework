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
	int* hasdestroy;
public:
	matrix();
	matrix(int row, int line);
	matrix(int row, int line, float **data);
	matrix(const matrix& other);
	matrix(int row, int line, float num);
	matrix(int row, int line, int i, int j);
	matrix(int row);
	~matrix();
	matrix operator = (const matrix& other);
	matrix operator * (const matrix& A);
	matrix operator * (float c);
	matrix eletrans(int n, int row1, int row2);
	matrix eletrans(int n, int k, float rok);
	matrix eletrans(int n, int row1, int row2, int opp, float rok);
	friend matrix operator * (float real, const matrix& other);
	friend std::ostream& operator << (std::ostream& os, const matrix& other);
};
float** dotmultiplication(float **a, float **b, int i, int j, int k)
{
	float a1 = 0, b1 = 0, c1 = 0;
	float** sum;
	sum = new float*[i];
	for (int n = 0; n < i; n++)
	{
		sum[n] = new float[k] {0};
	}
	float s = 0;
#pragma omp parallel for
	for (int n = 0; n < i; n++)
	{
		for (int p = 0; p < j; p = p++)
		{
			a1 = a[n][p];
			for (int m = 0; m < k; m = m++)
			{
				b1 = b[p][m];
				sum[n][m] += a1 * b1;
			}
		}
	}
	return sum;
}

matrix::matrix()
{
	row = 0;
	line = 0;
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
	hasdestroy = new int[1]{1};
}
matrix::matrix(const matrix &other)
{
	hasdestroy = new int[1];
	if (hasdestroy[0] == 0)
	{
		delete[] data;
	}
	else
	{
		hasdestroy[0]--;
	}
	hasdestroy = other.hasdestroy;
	hasdestroy[0]++;
	row = other.row;
	line = other.line;
	data = other.data;
}
matrix::matrix(int row, int line, float **data)
{
	this->row = row;
	this->line = line;
	this->data = data;
	hasdestroy = new int[1]{1};
}
matrix::matrix(int row, int line, float num)
{
	this->row = row;
	this->line = line;
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[line] {0};
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < line; j++)
		{
			data[i][j] = num;
		}
	}
	hasdestroy = new int[1]{ 1 };
}
matrix::matrix(int row, int line, int i, int j)
{
	this->row = row;
	this->line = line;
	data = new float*[row];
	for (int p = 0; p < row; p++)
	{
		data[p] = new float[line] {0};
	}
	data[i - 1][j - 1] = 1;
	hasdestroy = new int[1]{ 1 };
}
matrix::matrix(int row)
{
	this->row = row;
	this->line = row;
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[row] {0};
	}

	for (int i = 0; i < row; i++)
	{
		data[i][i] = 1;
	}
	hasdestroy = new int[1]{ 1 };
}
matrix::~matrix()
{
	hasdestroy[0] --;
	if (hasdestroy[0] == 0)
	{
		delete[] data;
	}
}
matrix matrix::operator = (const matrix& other)
{
	if (this == &other)
	{
		return *this;
	}
	else
	{
		line = other.line;
		row = other.row;
		data = other.data;
		hasdestroy = other.hasdestroy;
		hasdestroy[0]++;
		return *this;
	}
}

matrix matrix::operator*(const matrix& other) 
{
	return matrix(row, other.line, dotmultiplication(data, other.data, row, line, other.line));
}

matrix matrix::operator*(float c)
{
	if (hasdestroy[0] == 1)
	{
#pragma omp parallel for
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < line; j++)
			{					
				data[i][j] *= c;
			}
		}
		return *this;
	}
	else
	{
		float **data1 = new float*[row];
		for (int p = 0; p < row; p++)
		{
			data1[p] = new float[line] {0};
		}
#pragma omp parallel for
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < line; j++)
			{
				data1[i][j] = data[i][j] * c;
			}
		}
		return matrix(row, line, data1);
	}
}

matrix operator*(float real, const matrix& other)
{
	
	if (other.hasdestroy[0] == 1)
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
	else
	{
		float **data1 = new float*[other.row];
		for (int p = 0; p < other.row; p++)
		{
			data1[p] = new float[other.line] {0};
		}
#pragma omp parallel for
		for (int i = 0; i < other.row; i++)
		{
			for (int j = 0; j < other.line; j++)
			{
				data1[i][j] = other.data[i][j] * real;
			}
		}
		return matrix(other.row, other.line, data1);
	}
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

matrix matrix::eletrans(int n, int row1, int row2)
{
	if (hasdestroy[0] == 1)
	{
		if (n == 1)
			{
				float * a = data[row1];
				data[row1] = data[row2];
				data[row2] = a;
			}
			else if(n == 0)
			{
				for (int j = 0; j < line; j++)
				{
					float a = data[row1][j];
					data[row1][j] = data[row2][j];
					data[row2][j] = a;
				}
			}
			return *this;
	}
	else
	{

	}
}

matrix matrix::eletrans(int n, int row1, float rok)
{
	if (n == 1)
	{
		for (int j = 0; j < line; j++)
		{
			data[row1][j] = data[row1][j] * rok;
		}
	}
	else if (n == 0)
	{
		for (int j = 0; j < row; j++)
		{
			data[j][row1] = data[j][row1] * rok;
		}
	}
	return *this;
}
matrix matrix::eletrans(int n, int row1, int row2, int opp, float rok)
{
	if (n == 1)
	{
		for (int j = 0; j < line; j++)
		{
			data[row2][j] = data[row2][j] + opp * data[row1][j] * rok;
		}
	}
	else if (n == 0)
	{
		for (int j = 0; j < row; j++)
		{
			data[j][row2] = data[j][row2] + opp * data[j][row1] * rok;
		}
	}	
	return *this;
}

