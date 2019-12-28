#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <algorithm>
using std::vector;
using std::sort;
#define INFINITY 300001

struct adj {
	int end;
	int weight;
};

struct EDGE {
	int begin;
	int end;
	int weight;
};

bool heapInsert(EDGE* heap, const EDGE elem, const int heapMaxSize, int &heapSize)
{
	// min heap
	EDGE tmp;
	if (heapSize >= heapMaxSize) return false;
	heap[heapSize++] = elem;
	int i = heapSize-1; // the last element's index 
	while ( i>= 1 && heap[i].weight < heap[(i - 1) / 2].weight)
	{
		tmp = heap[i];
		heap[i] = heap[(i - 1) / 2];
		heap[(i - 1) / 2] = tmp;
		i = (i - 1) / 2;
	}
	return true;
}

bool heapPop(EDGE* heap, EDGE &obj, int &heapSize)
{
	if (heapSize == 0) return false;
	EDGE tmp;
	obj = heap[0];
	heap[0] = heap[heapSize - 1];
	heapSize--;
	int i = 0;

	bool condition1 = // current node only has left child
		(2 * i + 1 < heapSize && 2 * i + 2 >= heapSize && heap[i].weight > heap[2 * i + 1].weight);
	bool condition2 = // current node has left and right children
		(2*i+2 < heapSize && (heap[i].weight > heap[2*i+1].weight || heap[i].weight > heap[2*i+2].weight));
	// obviously condition1 != condition2
	while (condition1 || condition2)
	{
		if ((condition2 && heap[2 * i + 1].weight < heap[2 * i + 2].weight) || condition1)
		{
			//exchange(2i+1, i)
			tmp = heap[2 * i + 1];
			heap[2 * i + 1] = heap[i];
			heap[i] = tmp;
			i = 2 * i + 1;
		}
		else
		{
			tmp = heap[2 * i + 2];
			heap[2 * i + 2] = heap[i];
			heap[i] = tmp;
			i = 2 * i + 2;
		}
		// update while conditions
		condition1 = // current node only has left child
			(2 * i + 1 < heapSize && 2 * i + 2 >= heapSize && heap[i].weight > heap[2 * i + 1].weight);
		condition2 = // current node has left and right children
			(2 * i + 2 < heapSize && (heap[i].weight > heap[2 * i + 1].weight || heap[i].weight > heap[2 * i + 2].weight));
	}
	return true;
}

bool adjcmp(adj a, adj b)
{
	if (a.weight < b.weight) return true;
	return false;
}

int Dijkstra(vector<adj>* gg, int source, int Vcnt, int Ecnt, int dest)
{
	bool* inSPT = new bool[Vcnt]; // keep track of whether a vertex is already in SPT
	int v;
	unsigned i, j;
	int d;
	int edgeCount;
	//int d = INFINITY;
	EDGE* path = new EDGE[Ecnt];
	EDGE tmp0, tmp1, tmp2;
	int pathSize = 0;

	// initialize inSPT boolean array
	for (i = 0; i < Vcnt; i++)
	{
		inSPT[i] = false;
	}
	inSPT[source] = true;

	// initialize path (the non-SPT) in O(V)
	for (i = 0; i < gg[source].size(); ++i)
	{
		tmp0.begin = source;
		tmp0.end = gg[source][i].end;
		tmp0.weight = gg[source][i].weight;
		heapInsert(path, tmp0, Ecnt, pathSize);
	}

	// get V-1 edges
	edgeCount = 0;
	while (edgeCount < Vcnt - 1)
	{
		heapPop(path, tmp1, pathSize);
		if (inSPT[tmp1.end]) continue;
		if (tmp1.end == dest) { d = tmp1.weight; delete[] path; return d; }
		v = tmp1.end;
		inSPT[v] = true;
		for (j = 0; j < gg[v].size(); ++j)
		{
			/*if (tmp1.weight + getWeight(gg[tmp1.end], path[j].end) < path[j].weight)
			{
				path[j].begin = tmp1.end;
				path[j].weight = tmp1.weight + getWeight(gg[tmp1.end], path[j].end);
			}*/
			if (inSPT[gg[v][j].end]) continue; // if the vertex is already in SPT, it is irrelevant

			tmp2.begin = tmp1.end;
			tmp2.end = gg[v][j].end;
			tmp2.weight = tmp1.weight + gg[v][j].weight;
			heapInsert(path, tmp2, Ecnt, pathSize);
		}
	}
	/*delete[] path;
	return d;*/
}

int readNum() {   // ¿ìËÙ¶ÁÈë
	char ch = getchar();
	while (ch < '0' or ch > '9') ch = getchar();
	int v = 0;
	while (ch >= '0' and ch <= '9') {
		v = v * 10 + ch - '0';
		ch = getchar();
	}
	return v;
}

int main()
{
	const int V = readNum();
	const int E = readNum();
	const int Q = readNum();

	// set up the adjacency table
	vector<adj>* g = new vector<adj>[V];

	// temporary variables for storing what is read from input
	int begin, end, weight;
	adj tmp0, tmp1;

	// read numbers and construct adjacency table
	for (int i = 0; i < E; i++)
	{
		begin = readNum() - 1; // index of node starts with 0
		end = readNum() - 1; // index of node starts with 0
		weight = readNum();

		tmp0.weight = weight;
		tmp0.end = end;
		g[begin].push_back(tmp0);

		tmp1.weight = weight;
		tmp1.end = begin;
		g[end].push_back(tmp1); // undirected graph
	}

	// sort the adjacency lists so that every edge that is first read is the smallest
	// (in case of repeated edges) 
	for (int i = 0; i < V; i++)
	{
		sort(g[i].begin(), g[i].end(), adjcmp);
	}

	// query
	for (int j = 0; j < Q; j++)
	{
		int source = readNum() - 1;
		int dest = readNum() - 1;
		//printf("---sauce%d, dest%d\n", source, dest);
		int n = Dijkstra(g, source, V, E, dest);
		printf("%d\n", n);
	}

	return 0;
}

//int main()
//{
//	int size = 9;
//	int b[] = {1, 39, 8, 15, 79, 54, 26, 23, 93};
//	EDGE* heap = new EDGE[size];
//	EDGE* test = new EDGE[size];
//	EDGE tmp;
//	int testsize = 0;
//
//	for (int i = 0; i < size; ++i)
//	{
//		test[i].begin = i;
//		test[i].end = i + 1;
//		test[i].weight = b[i];
//		heapInsert(heap, test[i], size, testsize);
//	}
//
//	for (int i = 0; i < size; ++i)
//	{
//		printf("%d, ", heap[i].weight);
//	}
//	printf("\n");
//	/*EDGE prev;
//	heapPop(heap, prev, testsize);*/
//	for(int i=0;i < size ; ++i)
//	{
//		heapPop(heap, tmp, testsize);
//		printf("%d, ", tmp.weight);
//		/*if (tmp.weight < prev.weight) 
//		{
//			printf("i=%d, now%d:%d_____former%d:%d\n", i, tmp.begin, tmp.weight, prev.begin, prev.weight);
//		}
//		prev = tmp;*/
//	}
//	printf("ttt%d\n", testsize);
//	return 0;
//}