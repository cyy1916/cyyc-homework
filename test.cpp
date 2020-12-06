#include <iostream>
#include "matrix.h"
using namespace std;
int main()
{
	auto now = std::chrono::system_clock::now();
	auto t_c = std::chrono::system_clock::to_time_t(now - 24h);
	matrix a(4, 4, 1);
	matrix b(4);
	matrix c;
	matrix d(4, 4, 1, 1);
	auto start1 = std::chrono::steady_clock::now();
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;
	cout << 2 * a << endl;
	cout << b * 2 << endl;
	cout << a - b << endl;
	cout << b + d << endl;
	cout << a.eletrans(1, 1, 2) << endl;
	cout << (d + b).eletrans(1, 1, 1, 2) << endl;
	cout << b.eletrans(1, 1, 2, 1, 2) << endl;
	auto end1 = std::chrono::steady_clock::now();
	std::cout
		<< "Time of calculation: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << "us ≈ "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms ≈ "
		<< std::chrono::duration_cast<std::chrono::seconds>(end1 - start1).count() << "s.\n";
}
