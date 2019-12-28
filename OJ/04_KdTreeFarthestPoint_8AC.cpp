#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <algorithm>
#define type unsigned long
using std::nth_element;

struct DataTuple {
	type val[2];
	int idx;
};

class Node {
public:
	//Node(DataTuple* p) : ptr(p), left(NULL), right(NULL), parent(NULL) {}
	//Node* left;
	//Node* right;
	//Node* parent;
	type lb[2]; // lower bounds
	type ub[2]; // upper bounds 
	int cd;
	DataTuple* ptr; // maybe a pointer is enough
	Node(DataTuple* p, type l[], type u[], int d)
	{
		ptr = p;
		lb[0] = l[0];
		lb[1] = l[1];
		ub[0] = u[0];
		ub[1] = u[1];
		cd = d;
	}
	//Node(DataTuple* p, int cd, type left, type right, type low, type up) : ptr(p), cd(cd), l(left), r(right), low(low), up(up) {}
};


// stack and other global variables
struct mystack {
	int begin;
	int end;
	int node; // index for a node in tree 
	type lb[2];
	type ub[2];
};
mystack stack[5000]; // stack for building the tree
int top = -1; // stack pointer


int stackf[5000]; // stack for searching the tree
int top1 = -1; // stackf pointer

struct grt {
	type dist;
	int idx;
};
grt great[1000];
DataTuple data[50000]; // storing original background points
Node* tree[150000]; // kd-tree
//int ans[20000];
//int anstop = 0;

type xmax = 0, xmin = 0, ymax = 0, ymin = 0;


class my_compare {
public:
	int cd; //sorting order
	my_compare(const int& d) : cd(d) {}
	bool operator() (const DataTuple& a, const DataTuple& b) const {
		// return value true means a < b
		if (a.val[cd] == b.val[cd]) { return a.idx > b.idx; } // sort by index if the 2 are equal
		return a.val[cd] < b.val[cd];
	}
};



type findmax(type a, type b, type c, type d)
{
	type aa = a > b ? a : b;
	type cc = c > d ? c : d;
	return aa > cc ? aa : cc;
}

type eucDist(type x1, type y1, type x2, type y2)
{
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void maintainGreat(grt great[], int order, type dist, int idx)
{
	if (dist <= great[0].dist)
	{
		if (dist < great[0].dist) return;
		else if (idx >= great[0].idx) return;
	}
	great[0].dist = dist;
	great[0].idx = idx;
	for (int i = 0; i < order - 1; i++)
	{
		if (great[i].dist > great[i + 1].dist || (great[i].dist == great[i + 1].dist && great[i].idx < great[i + 1].idx))
		{
			type tmp = great[i + 1].dist;
			int tmp1 = great[i + 1].idx;
			great[i + 1].dist = great[i].dist;
			great[i + 1].idx = great[i].idx;
			great[i].dist = tmp;
			great[i].idx = tmp1;
		}
		else break;
	}
}

bool ycompare(DataTuple a, DataTuple b)
{
	// return value true means a < b
	if (a.val[1] == b.val[1]) { return a.idx < b.idx; } // sort by index if the 2 are equal
	return a.val[1] < b.val[1];
}

bool xcompare(DataTuple a, DataTuple b)
{
	// return value true means a < b
	if (a.val[0] == b.val[0]) { return a.idx < b.idx; } // sort by index if the 2 are equal
	return a.val[0] < b.val[0];
}


void createKdTree(int size)
{
	int now;
	int cd = 0; // cutting demension
	int begin, end;
	type lb[2];
	type ub[2];
	//tree[0] = new Node(0, lb, ub, 0);
	// push the root into stack
	top++;
	stack[top].begin = 0;
	stack[top].end = size;
	stack[top].node = 0;
	stack[top].lb[0] = xmin;
	stack[top].lb[1] = ymin;
	stack[top].ub[0] = xmax;
	stack[top].ub[1] = ymax;
	//// process the root
	//nth_element(data, data + (size - 1) / 2, data + size, my_compare(cd));
	//tree[0] = new Node(data + (size - 1) / 2);
	//

	//// push the bigger side of data into stack
	//top++;
	//stack[top].begin = (size + 1) / 2;
	//stack[top].end = size;
	//stack[top].node = 1; // NULL for now
	//// push the smaller side of data into stack
	//top++;
	//stack[top].begin = 0;
	//stack[top].end = (size - 1) / 2;
	//stack[top].node = 2; // NULL for now

	while (top != -1)
	{
		// pop stack
		begin = stack[top].begin;
		end = stack[top].end;
		now = stack[top].node;
		lb[0] = stack[top].lb[0];
		lb[1] = stack[top].lb[1];
		ub[0] = stack[top].ub[0];
		ub[1] = stack[top].ub[1];
		top--;

		// find the median, finally the right subtree would definately not be empty
		nth_element(data + begin, data + (begin + end - 1) / 2, data + end, my_compare(cd));
		//if(cd==0) nth_element(data + begin, data + (begin + end - 1) / 2, data + end, xcompare);
		//else nth_element(data + begin, data + (begin + end - 1) / 2, data + end, ycompare);
		tree[now] = new Node(data + (begin + end - 1) / 2, lb, ub, cd); // node to be added;

		int od = cd == 0 ? 1 : 0;

		if ((begin + end + 1) / 2 != end) // still have greater data to process
		{
			// push stack
			top++;
			stack[top].begin = (begin + end + 1) / 2;
			stack[top].end = end;
			stack[top].node = 2 * now + 2; // right child
			stack[top].lb[cd] = tree[now]->ptr->val[cd];
			stack[top].lb[od] = lb[od];
			stack[top].ub[cd] = ub[cd];
			stack[top].ub[od] = ub[od];
		}

		if ((begin + end - 1) / 2 != begin) // still have smaller data to process
		{
			// push stack
			top++;
			stack[top].begin = begin;
			stack[top].end = (begin + end - 1) / 2;
			stack[top].node = 2 * now + 1; // left child
			stack[top].lb[cd] = lb[cd];
			stack[top].lb[od] = lb[od];
			stack[top].ub[cd] = tree[now]->ptr->val[cd];
			stack[top].ub[od] = ub[od];
		}
		cd = od; // update cd
	}
}

void findInTree(type obj[], int order)
{
	top1 = -1;
	int now;
	type lb[2]; // lower bound for the area in search
	type ub[2]; // upper bound for the area in search
	int cd;
	//grt* great = new grt[order]; // an <order> amount of greatest distances, ascending
	// initialize great
	for (int i = 0; i < order; i++)
	{
		great[i].dist = 0;
		great[i].idx = 50001;
	}

	// push the root
	top1++;
	stackf[top1] = 0;

	while (top1 != -1)
	{
		// pop stack
		now = stackf[top1];
		top1--;

		type dist = eucDist(tree[now]->ptr->val[0], tree[now]->ptr->val[1], obj[0], obj[1]);
		maintainGreat(great, order, dist, tree[now]->ptr->idx);
		cd = tree[now]->cd;
		lb[0] = tree[now]->lb[0];
		lb[1] = tree[now]->lb[1];
		ub[0] = tree[now]->ub[0];
		ub[1] = tree[now]->ub[1];
		int od = (cd == 0 ? 1 : 0);

		// see if left child can possibly update great[]:
		type leftdist1 = eucDist(obj[cd], obj[od], lb[cd], lb[od]);
		type leftdist2 = eucDist(obj[cd], obj[od], tree[now]->ptr->val[cd], lb[od]);
		type leftdist3 = eucDist(obj[cd], obj[od], lb[cd], ub[od]);
		type leftdist4 = eucDist(obj[cd], obj[od], tree[now]->ptr->val[cd], ub[od]);
		type ldist_max = findmax(leftdist1, leftdist2, leftdist3, leftdist4);

		// see if right child can possibly update great[]:
		type rightdist1 = eucDist(obj[cd], obj[od], tree[now]->ptr->val[cd], lb[od]);
		type rightdist2 = eucDist(obj[cd], obj[od], ub[cd], lb[od]);
		type rightdist3 = eucDist(obj[cd], obj[od], tree[now]->ptr->val[cd], ub[od]);
		type rightdist4 = eucDist(obj[cd], obj[od], ub[cd], ub[od]);
		type rdist_max = findmax(rightdist1, rightdist2, rightdist3, rightdist4);

		if (ldist_max > rdist_max) {
			// push right first
			if (rdist_max >= great[0].dist)
			{
				if (tree[2 * now + 2] != NULL)
				{
					// push right child into stackf
					top1++;
					stackf[top1] = 2 * now + 2;
				}
			}
			if (ldist_max >= great[0].dist)
			{
				if (tree[2 * now + 1] != NULL)
				{
					// push left child into stackf
					top1++;
					stackf[top1] = 2 * now + 1;
				}
			}
		}
		else {
			if (ldist_max >= great[0].dist)
			{
				if (tree[2 * now + 1] != NULL)
				{
					// push left child into stackf
					top1++;
					stackf[top1] = 2 * now + 1;
				}
			}
			if (rdist_max >= great[0].dist)
			{
				if (tree[2 * now + 2] != NULL)
				{
					// push right child into stackf
					top1++;
					stackf[top1] = 2 * now + 2;
				}
			}
		}

	}
	//if (ldist_max >= great[0].dist)
	//{
	//	if (tree[2 * now + 1] != NULL)
	//	{
	//		// push left child into stackf
	//		top1++;
	//		stackf[top1] = 2 * now + 1;
	//	}
	//}

	//
	//if (rdist_max >= great[0].dist)
	//{
	//	if (tree[2 * now + 2] != NULL)
	//	{
	//		// push right child into stackf
	//		top1++;
	//		stackf[top1] = 2 * now + 2;
	//	}
	//}

	//if (obj[cd] <= tree[now]->ptr->val[cd])
	//{
	//	type tmp = lb[cd];
	//	lb[cd] = tree[now]->ptr->val[cd];
	//	type dist_ul = eucDist(obj[0], obj[1], lb[0], ub[1]);
	//	type dist_ur = eucDist(obj[0], obj[1], ub[0], ub[1]);
	//	type dist_ll = eucDist(obj[0], obj[1], lb[0], lb[1]);
	//	type dist_lr = eucDist(obj[0], obj[1], ub[0], lb[1]);

	//	type dist_max = findmax(dist_ul, dist_ur, dist_ll, dist_lr);
	//	if (dist_max <= great[0]) // no need to look further at the right side
	//	{

	//		// push left child
	//		top1++;
	//		stackf[top1].node = 2 * now + 1;
	//		stackf[top1].lower[cd] = tmp;
	//		stackf[top1].lower[cd == 0 ? 1 : 0] = lb[cd == 0 ? 1 : 0];
	//		stackf[top1].upper[cd] = lb[cd];
	//		stackf[top1].upper[cd == 0 ? 1 : 0] = ub[cd == 0 ? 1 : 0];
	//		continue;
	//	}
	//	else ()
	//}

	printf("%d\n", great[0].idx);
	//ans[anstop++] = great[0].idx;
	//delete[] great;
	//great = NULL;
}

void setTuple(DataTuple& dt, type x, type y, int idx)
{
	dt.val[0] = x;
	dt.val[1] = y;
	dt.idx = idx;
}

type readNum() // ¿ìËÙ¶ÁÈë

{
	char ch = getchar();
	while (ch < '0' || ch > '9') ch = getchar();
	type v = 0;
	while (ch >= '0' && ch <= '9') {
		v = v * 10 + ch - '0';
		ch = getchar();

	}
	return v;

};

int main()
{
	// take input data
	int m, n;
	type x, y;
	n = readNum();
	m = readNum();
	for (int i = 0; i < n; i++)
	{
		x = readNum();
		y = readNum();
		setTuple(data[i], x, y, i + 1);
		if (x > xmax) xmax = x;
		else if (x < xmin) xmin = x;
		if (y > ymax) ymax = y;
		else if (y < ymin) ymin = y;
	}
	createKdTree(n);
	for (int j = 0; j < m; j++)
	{
		type co[2];
		co[0] = readNum();
		co[1] = readNum();
		int ord = readNum();
		findInTree(co, ord);
	}
	/*for (int i = 0; i < anstop; i++)
	{
		printf("%d\n", ans[i]);
	}*/
	return 0;
}