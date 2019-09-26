#include <iostream>
#include <string>
using namespace std;
int main() {
	string a, b, out_rev; // out_rev, output in reversed order
	cin >> a >> b;
	int i = a.length();
	int j = b.length();
	char temp;
	bool carry = 0; 
	while (i > 0 && j > 0) {
		i--;
		j--;
		temp = a[i] + b[j] + carry - 48;
		if (temp >= 58) {
			carry = 1;
			temp -= 10;
		}
		else carry = 0;
		out_rev = out_rev + temp;
	}
	if (i > 0) {
		for (int k = i-1; k >= 0; k--) {
			temp = a[k] + carry;
			if (temp >= 58) {
				temp -= 10;
				carry = 1;
			}
			else carry = 0;
			out_rev = out_rev + temp;
		}
		if (carry)
			out_rev = out_rev + '1';
	}
	else if (j > 0) {
		for (int k = j-1; k >= 0; k--) {
			temp = b[k] + carry;
			if (temp >= 58) {
				temp -= 10;
				carry = 1;
			}
			else carry = 0;
			out_rev = out_rev + temp;
		}
		if (carry)
			out_rev = out_rev + '1';
	}
	else if(carry) out_rev = out_rev + '1';

	for (int ii = out_rev.length() - 1; ii >= 0; ii--) {
		cout << out_rev[ii];
	}

	return 0;
}