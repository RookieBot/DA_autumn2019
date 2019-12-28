#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <stdio.h>

std::map<int, bool> lengths;
int main() {
	int N, x;
	scanf("%d", &N);
	for (int i = 0; i < 2 * N - 1; i++) {
		scanf("%d", &x);
		lengths[x] = !lengths[x];
	}
	for (std::map<int, bool>::iterator it = lengths.begin(); it != lengths.end(); it++) {
		if (it->second) {
			printf("%d", it->first);
			break;
		}
	}
	return 0;
}


// the bit-wise XOR result of the lengths of N pairs of chopsticks should be zero (obviously);
// define n = length of the lost chopstick
// we just calculate the bit-wise XOR result of the lengths of 2*N-1 chopsticks k; 
// naturally n XOR k = 0 which means n = k;
// I read about this trick after many unsuccesful tries;
// the best I did on my own was O(nlog(n)), which only passed test cases 1~5 and 7; 
// reference: https://blog.csdn.net/dxyinme/article/details/75330210

//int main() {
//	int N, k;
//	int lost_length = 0;
//	scanf("%d", &N);
//	for (int i = 0; i < 2 * N - 1; i++) {
//		scanf("%d", &k);
//		lost_length = lost_length ^ k;
//	}
//	printf("%d", lost_length);
//	return 0;
//}

// O(nlogn)
//int partition(int* arr, int left, int right) {
//	int i = left - 1;
//	int x = *(arr + right);
//	for (int j = left; j < right; j++) {
//		if (*(arr + j) <= x) {
//			i += 1;
//			int temp = *(arr + j);
//			*(arr + j) = *(arr + i);
//			*(arr + i) = temp;
//		}
//	}
//	//exchange arr[i+1] with arr[right]
//	int temp = *(arr + i + 1);
//	*(arr + i + 1) = *(arr + right);
//	*(arr + right) = temp;
//	return i + 1;
//}
//
//int quickSort(int* arr, int left, int right) {
//	if (left < right) {
//		int q = partition(arr, left, right);
//		quickSort(arr, left, q - 1);
//		quickSort(arr, q + 1, right);
//	}
//	return 0;
//}
//
//int stickLengths[8000000];
//int main() {
//	int N, tmp;
//	scanf("%d", &N);
//	for (int i = 0; i < 2 * N - 1; i++) scanf("%d", &stickLengths[i]);
//	quickSort(stickLengths, 0, 2 * N - 2);
//	bool flag = 1;
//	bool mark = 0;
//	for (int i = 1; i < 2 * N - 1; i++) {
//		if (stickLengths[i] == stickLengths[i - 1]) {
//			flag = !flag;
//		}
//		else if (flag) {
//			printf("%d", stickLengths[i - 1]);
//			mark = 1;
//			break;
//		}
//		else flag = 1;
//	}
//	if(!mark) printf("%d", stickLengths[2 * N - 2]);
//	return 0;
//}

// the following: O(n^2)
//class stickLength {
//public:
//	static int num;
//	int length;
//	bool flag;
//	void set(int len) { length = len; flag = 1; stickLength::num++; }
//	void add() { flag = !flag; }
//};
//
//int stickLength::num = 0;
//stickLength stickLengths[4000000]; // 31250 KBytes
//
//int main()
//{
//	int N, tmp;
//	scanf("%d", &N);
//	for (int i = 0; i < 2 * N - 1; i++) {
//		scanf("%d", &tmp);
//		bool flag_found = false;
//		for (int j = 0; j < stickLength::num; j++) {
//			if (tmp == stickLengths[j].length) { stickLengths[j].add(); flag_found = true; }
//		}
//		if (!flag_found) stickLengths[stickLength::num].set(tmp);
//	}
//	for (int j = 0; j < stickLength::num; j++) {
//		if (stickLengths[j].flag) {
//			printf("%d", stickLengths[j].length);
//			break;
//		}
//	}
//
//	return 0;
//}