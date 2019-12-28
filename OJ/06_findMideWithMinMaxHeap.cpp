#define _CRT_SECURE_NO_WARNINGS_
#include <stdio.h>

int readNum() // øÏÀŸ∂¡»Î
{
	char ch = getchar();
	while (ch < '0' || ch > '9') ch = getchar();
	int v = 0;
	while (ch >= '0' && ch <= '9')
	{
		v = v * 10 + ch - '0';
		ch = getchar();
	}
	return v;
};

void heapPush(int* heap, int num, int &pos, bool min)
{
	int temp;
	heap[pos++] = num;
	int now = pos - 1;
	if (min) // maintain min heap
	{
		while (now > 0 && heap[now] < heap[(now - 1) / 2])
		{
			temp = heap[now];
			heap[now] = heap[(now - 1) / 2];
			heap[(now - 1) / 2] = temp;
			now = (now - 1) / 2;
		}
	}
	else // maintain max heap
	{
		while (now > 0 && heap[now] > heap[(now - 1) / 2])
		{
			temp = heap[now];
			heap[now] = heap[(now - 1) / 2];
			heap[(now - 1) / 2] = temp;
			now = (now - 1) / 2;
		}
	}
}

void heapPop(int* heap, int& num, int& pos, bool min)
{
	int temp;
	int now = 0;
	num = heap[0];
	heap[0] = heap[pos - 1];
	pos = pos - 1;
	if (min) // maintain min heap
	{
		while (now < pos)
		{
			if (2 * now + 1 >= pos) break; // now is in child-less
			else if (2*now+2>=pos) // now has one child
			{
				if (heap[now] > heap[2 * now + 1])
				{
					temp = heap[now];
					heap[now] = heap[2 * now + 1];
					heap[2 * now + 1] = temp;
				}
				break;
			}
			else if (2 * now + 2 < pos) // now has 2 children
			{
				// idx is the index of the lesser child 
				int idx = (heap[2 * now + 1] < heap[2 * now + 2]) ? 2 * now + 1 : 2 * now + 2;
				if (heap[now] > heap[idx])
				{
					temp = heap[now];
					heap[now] = heap[idx];
					heap[idx] = temp;
					now = idx; // update [now]
					continue;
				}
				else break; // the heap is already heapified 
			}
		}
	}
	else
	{
		while (now < pos)
		{
			if (2 * now + 1 >= pos) break; // now is in child-less
			else if (2 * now + 2 >= pos) // now has one child
			{
				if (heap[now] < heap[2 * now + 1])
				{
					temp = heap[now];
					heap[now] = heap[2 * now + 1];
					heap[2 * now + 1] = temp;
				}
				break;
			}
			else if (2 * now + 2 < pos) // now has 2 children
			{
				// idx is the index of the greater child 
				int idx = (heap[2 * now + 1] > heap[2 * now + 2]) ? 2 * now + 1 : 2 * now + 2;
				if (heap[now] < heap[idx])
				{
					temp = heap[now];
					heap[now] = heap[idx];
					heap[idx] = temp;
					now = idx; // update [now]
					continue;
				}
				else break; // the heap is already heapified 
			}
		}
	}
}

int maxHeap[150000];
int minHeap[150000];
float query[300000];

int maxpos = 0, minpos = 0;

int main()
{
	int N, M, tmp0, tmp1, b;
	N = readNum();
	M = readNum();
	for (int i = 0; i < N; ++i)
	{
		tmp0 = readNum();
		heapPush(maxHeap, tmp0, maxpos, false); 
		heapPop(maxHeap, tmp1, maxpos, false); 
		heapPush(minHeap, tmp1, minpos, true);
		if (minpos > maxpos) // give it back, so minpos==maxpos xor minpos==maxpos-1 always holds true
		{
			heapPop(minHeap, tmp1, minpos, true);
			heapPush(maxHeap, tmp1, maxpos, false);
		}

		if (i & 1) // i is odd, so the size of current data stream is even
		{
			query[i] = ((float)(maxHeap[0] + minHeap[0])) / 2;
		}
		else // i is even, so the size of current data stream is odd
		{
			query[i] = maxHeap[0];
		}
	}

	// query
	for (int j = 0; j < M; ++j)
	{
		b = readNum();
		float result = query[b];
		if (result - (float)(int)result == 0) // result is integer
		{
			printf("%d\n", (int)result);
		}
		else
		{
			printf("%.1f\n", result);
		}
	}

	//// test heap
	//heapPush(minHeap, 2, minpos, true);
	//heapPush(minHeap, 4, minpos, true);
	//heapPush(minHeap, 7, minpos, true);
	//heapPush(minHeap, 6, minpos, true);
	//heapPush(minHeap, 2, minpos, true);
	//heapPush(minHeap, 9, minpos, true);
	//heapPush(minHeap, 8, minpos, true);
	//heapPush(minHeap, 10, minpos, true);

	//heapPush(maxHeap, 2, maxpos, false);
	//heapPush(maxHeap, 4, maxpos, false);
	//heapPush(maxHeap, 7, maxpos, false);
	//heapPush(maxHeap, 6, maxpos, false);
	//heapPush(maxHeap, 2, maxpos, false);
	//heapPush(maxHeap, 9, maxpos, false);
	//heapPush(maxHeap, 8, maxpos, false);
	//heapPush(maxHeap, 10, maxpos, false);

	//printf("%d-%d\n", minpos, maxpos);
	//int mint, maxt;
	//for (int i = 0; i < 8; ++i)
	//{
	//	heapPop(minHeap, mint, minpos, true);
	//	heapPop(maxHeap, maxt, maxpos, false);
	//	printf("%d, min %d, max %d\n", i, mint, maxt);
	//}
	//printf("%d-%d\n", minpos, maxpos);

	return 0;
}