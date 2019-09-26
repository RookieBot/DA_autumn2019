#include <iostream>
using namespace std;
// TLE version
/*unsigned long int fib(int n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	if (n == 2) return 1;
	// F(2n+1) = F(n+1) ^ 2 + F(n) ^ 2  
	// F(2n) = F(n+1) ^ 2 - F(n-1) ^ 2
	unsigned long int result, a, b;	
	a = fib(n / 2 + 1);
	if (n % 2) { // n is odd
		b = fib(n / 2); 
		result = a * a + b * b;
	}
	else { // n is even
		b = fib(n / 2 - 1);
		result = a * a - b * b;
	}
	if (result >= 9973) result -= 9973;
	return result;
}*/


class matrix {
public:
	friend matrix mat_multiply(matrix, matrix);
	friend matrix mat_power(matrix, int);
	matrix() : element00(0), element01(0), element10(0), element11(0) {}
	matrix(const matrix& A): element00(A.element00), element01(A.element01), element10(A.element10), element11(A.element11) {}
	matrix(unsigned long long int a[2][2]) : element00(a[0][0]), element01(a[0][1]), element10(a[1][0]), element11(a[1][1]) {}
	unsigned long long int element00;
	unsigned long long int element01;
	unsigned long long int element10;
	unsigned long long int element11;

};
matrix mat_multiply(matrix A, matrix B) { // AB
	matrix result;
	result.element00 = (A.element00 * B.element00 + A.element01 * B.element10) % 9973;
	result.element01 = (A.element00 * B.element01 + A.element01 * B.element11) % 9973;
	result.element10 = (A.element10 * B.element00 + A.element11 * B.element10) % 9973;
	result.element11 = (A.element10 * B.element01 + A.element11 * B.element11) % 9973;
	return result;
}

matrix mat_power(matrix A, int n) {
	if (n == 1) return A;
	matrix tmp;
	tmp = mat_power(A, n / 2);
	if ((n & 1) == 1) return mat_multiply(mat_multiply(tmp, tmp), A);
	else return mat_multiply(tmp, tmp);
}


unsigned long int fib(int n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	if (n == 2) return 1;
	unsigned long long int tmp[2][2] = { 0,1,1,1 };
	matrix starter(tmp);
	matrix result = mat_power(starter, n - 1);
	return result.element11;
}

int main() {
	int num;
	cin >> num;
	int inputs[19];
	for (int i = 0; i < num; i++) {
		cin >> inputs[i];
	}
	for (int i = 0; i < num; i++) {
		cout << fib(inputs[i])%9973 << endl;
	}
		//cout << 48 << '\t' << fib(48) << endl;

	return 0;
}