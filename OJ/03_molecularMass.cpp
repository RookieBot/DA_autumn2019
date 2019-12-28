#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cstring>
using namespace std;
//#include <time.h>

char str[4002];
int  pos = 0; // pos 记录栈当前位置
char ans[300] = "0";
char stack[4002][300];
char mass1[26][5] = { "0","0","120","0","0", "190","0","10","1270","0", "390","0","0","140","160", "310",
    "0","0","320","0", "0","0","0","0","890", "0" };
char mass2[26][26][5];

int multi[10][10];
int multc[10][10];

void str_plus(char* a, char* b, char* &out_rev) {
	//string a, b, out_rev; // out_rev, output in reversed order
	int i = strlen(a);
	int j = strlen(b);
	//char out_rev[4000];
	int outpos = 0;
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
		out_rev[outpos] = temp;
		outpos += 1;
	}
	if (i > 0) {
		for (int k = i - 1; k >= 0; k--) {
			temp = a[k] + carry;
			if (temp >= 58) {
				temp -= 10;
				carry = 1;
			}
			else carry = 0;
			out_rev[outpos] = temp;
			outpos += 1;
		}
		if (carry) {
			out_rev[outpos] = '1';
			outpos += 1;
		}
	}
	else if (j > 0) {
		for (int k = j - 1; k >= 0; k--) {
			temp = b[k] + carry;
			if (temp >= 58) {
				temp -= 10;
				carry = 1;
			}
			else carry = 0;
			out_rev[outpos] = temp;
			outpos += 1;
		}
		if (carry) {
			out_rev[outpos] = '1';
			outpos += 1;
		}
	}
	else if (carry) {
		out_rev[outpos] = '1';
		outpos += 1;
	}

	out_rev[outpos] = '\0'; // make it zero-terminated
	outpos += 1;

	//reverse the output string
	for (int i = 0; i < strlen(out_rev) / 2; i++) {
		char temp = out_rev[i];
		out_rev[i] = out_rev[strlen(out_rev) - i - 1];
		out_rev[strlen(out_rev) - i - 1] = temp;
	}

}

void str_mult(char* a, char* b, char* &out) {
	int i = strlen(a);
	int j = strlen(b);
	out[0] = '0';
	out[1] = '\0';

	while (j > 0) {
		j--;
		char* tmp = new char[300];
		char temp;
		int carry = 0;
		int tmppos = 0; // equals valid length of tmp
		i = strlen(a);
		while (i > 0) { // multi-digit * single-digit
			i--;
			temp = 48 + multi[a[i] - 48][b[j] - 48] + carry;
			if (temp >= 58) {
				carry = multc[a[i] - 48][b[j] - 48] + 1;
				tmp[tmppos] = temp - 10;
			}
			else {
				carry = multc[a[i] - 48][b[j] - 48];
				tmp[tmppos] = temp;
			}
			tmppos++;
		}
		if (carry > 0) {
			tmp[tmppos] = 48 + carry;
			tmppos++;
		}

		//reverse it no problem
		for (int ii = 0; ii < tmppos / 2; ii++) {
			char temp = tmp[ii];
			tmp[ii] = tmp[tmppos - ii - 1];
			tmp[tmppos - ii - 1] = temp;
		} // multi*single finished
		// add zeros
		for (int ii = 0; ii < strlen(b) - 1 - j; ii++) {
			tmp[tmppos] = '0';
			tmppos++;
		}
		tmp[tmppos] = '\0';
		tmppos++;
		char* tmpp = new char[300];
		str_plus(out, tmp, tmpp);
		strcpy(out, tmpp);
		delete[] tmp;
		tmp = NULL;
		delete[] tmpp;
		tmpp = NULL;
	}

}


char* mass(char a0, char a1 = '\0') {
	if (a1 == '\0') {
		int i1 = a0 - 65;
		return mass1[i1];
	}
	else {
		int i1 = a0 - 65;
		int i2 = a1 - 97;
		return mass2[i1][i2];
	}
}

void readint(int i, char* num, int& j)
{
	int numpos = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num[numpos] = str[i];
		numpos++;
		i++;
	}
	num[numpos] = '\0';
	j = i - 1;
}


int main() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			int temp = i * j;
			multi[i][j] = temp % 10;
			multc[i][j] = temp / 10;
		}
	}

	strcpy(mass2[7][4], "40"); //He
	strcpy(mass2[13][4], "200"); //Ne
	strcpy(mass2[13][0], "230"); //Na
	strcpy(mass2[12][6], "240"); //Mg
	strcpy(mass2[0][11], "270"); //Al
	strcpy(mass2[18][8], "280"); //Si
	strcpy(mass2[2][11], "355"); //Cl
	strcpy(mass2[0][17], "400"); //Ar
	strcpy(mass2[2][0], "400"); //Ca
	strcpy(mass2[19][8], "480"); //Ti
	strcpy(mass2[12][13], "550"); //Mn
	strcpy(mass2[5][4], "560"); //Fe
	strcpy(mass2[2][20], "635"); //Cu
	strcpy(mass2[25][13], "650"); //Zn
	strcpy(mass2[0][6], "1080"); //Ag
	strcpy(mass2[1][0], "1370"); //Ba
	strcpy(mass2[15][19], "1950"); //Pt
	strcpy(mass2[0][20], "1970"); //Au
	strcpy(mass2[7][6], "2010"); //Hg
	strcpy(mass2[15][14], "2090"); //Po

	str[0] = '(';
	scanf("%s", &str[1]);
	int  l = strlen(str);
	str[l] = ')';
	l += 1;
	int i = -1;
	while (1)
	{
		i = i + 1;
		if (i == l) break;
		if (str[i] == '(')
		{
			pos = pos + 1;
			strcpy(stack[pos], ans);
			strcpy(ans, "0");
			pos = pos + 1;
			strcpy(stack[pos], "-1");
			continue;
		}
		if (str[i] == ')')
		{
			pos = pos + 1;
			strcpy(stack[pos], ans);
			strcpy(ans, "0");
			char objcmp0[] = "-1";
			while (strcmp(stack[pos - 1],objcmp0)!=0)
			{
				char objcmp[] = "-2";
				if (pos > 3 && strcmp(stack[pos - 2],objcmp)==0)
				{
					char* tmp1 = new char[300];
					str_mult(stack[pos-1], stack[pos], tmp1);
					strcpy(stack[pos-2], tmp1);
					delete[] tmp1;
					tmp1 = NULL;
					pos -= 2;
				}
				else
				{
					//stack[pos - 1] = stack[pos] + stack[pos - 1];
					char* tmp1 = new char[300];
					str_plus(stack[pos], stack[pos - 1], tmp1);
					strcpy(stack[pos - 1], tmp1); // excetion thrown 
					pos--;
					delete[] tmp1;
					tmp1 = NULL;
				}
			}
			//stack[pos - 1] = stack[pos];
			strcpy(stack[pos - 1], stack[pos]);
			pos--;
			if (str[i + 1] == '_')
			{
				int j;
				char* num = new char[300];
				readint(i + 3, num, j);
				i = j + 1;
				//stack[pos] = stack[pos] * num;
				char* tmp1 = new char[300];
				str_mult(stack[pos], num, tmp1);
				strcpy(stack[pos], tmp1);
				delete[] num; num = NULL;
				delete[] tmp1; tmp1 = NULL;
			}
			continue;
		}
		if (str[i] == '~')
		{
			pos = pos + 1;
			strcpy(stack[pos], ans);
			strcpy(ans, "0");// ans = 0;
			pos = pos + 1;
			strcpy(stack[pos], "-2"); //stack[pos] = -2;
			if (str[i + 1] >= '0' && str[i + 1] <= '9')
			{
				int j;
				char* num = new char[300];
				readint(i + 1, num, j);
				i = j;
				pos = pos + 1;
				strcpy(stack[pos], num);//stack[pos] = num;
				delete[] num; num = NULL;
			}
			else
			{
				pos = pos + 1;
				strcpy(stack[pos], "1");//stack[pos] = 1;
			}
			continue;
		}
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
			{
				if (str[i + 2] == '_')
				{
					int j; // j是'}'
					char* num = new char[300];
					readint(i + 4, num, j);
					//ans = ans + num * mass(str[i], str[i + 1]);
					char* tmp1 = new char[300];
					char* tmp2 = new char[300];
					char* tmp3 = mass(str[i], str[i + 1]);
					str_mult(num, tmp3, tmp1);
					str_plus(tmp1, ans, tmp2);
					strcpy(ans, tmp2);
					i = j + 1;
					delete[] num;
					delete[] tmp1;
					delete[] tmp2;
					num = NULL;
					tmp1 = NULL;
					tmp2 = NULL;
					continue;
				}
				else
				{
					//ans = ans + mass(str[i], str[i + 1]);
					char* tmp1 = new char[300];
					str_plus(ans, mass(str[i], str[i + 1]), tmp1);
					strcpy(ans, tmp1);
					delete[] tmp1;
					tmp1 = NULL;
					continue;
				}
			}
			else
			{
				if (str[i + 1] == '_')
				{
					int j; // j是'}'
					char* num = new char[300];
					readint(i + 3, num, j);
					//ans = ans + num * mass(str[i]);
					char* tmp1 = new char[300]; 
					char* tmp2 = new char[300];
					char* tmp3 = mass(str[i]);
					str_mult(num, tmp3, tmp1);
					str_plus(tmp1, ans, tmp2);
					strcpy(ans, tmp2);
					i = j + 1;
					delete[] num;
					delete[] tmp1;
					delete[] tmp2;
					num = NULL;
					tmp1 = NULL;
					tmp2 = NULL;
					continue;
				}
				else
				{
					//ans += mass(str[i]);
					char* tmp1 = new char[300];
					str_plus(ans, mass(str[i]), tmp1);
					strcpy(ans, tmp1);
					delete[] tmp1;
					tmp1 = NULL;
					continue;
				}
			}
		}
	}

	//while (pos > 0)
	//{
	//	if (pos > 2 && stack[pos - 2] == -2)
	//	{
	//		stack[pos - 2] = stack[pos - 1] * stack[pos];
	//		pos -= 2;
	//	}
	//	else
	//	{
	//		stack[pos - 1] = stack[pos - 1] + stack[pos];
	//		pos--;
	//	}
	//}
	//long double tmp = (long long)stack[pos];
	/*if (stack[pos] == tmp) printf("%lld\n", (long long)stack[pos]);
	else printf("%.1lf\n", stack[pos]);*/
	int final_idx = strlen(stack[pos]) - 1;
	if (stack[pos][final_idx] == '0') {
		stack[pos][final_idx] = '\0';
		printf("%s", stack[pos]);
	}
	else {
		stack[pos][final_idx] = '.';
		stack[pos][final_idx+1] = '5';
		stack[pos][final_idx + 2] = '\0';
		printf("%s", stack[pos]);
	}
	return 0;
}