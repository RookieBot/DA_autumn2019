#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

template <class T>
class TNode {
	template <class Y>
	friend class DlkList;
private:
	T stu_num;
	TNode<T>* left;
	TNode<T>* right;
public:
	TNode(T num):stu_num(num), left(NULL), right(NULL) {}
	void display() { printf("%d", stu_num); }
};

template <class T>
class DlkList {
public:
	DlkList() { head = NULL; }
	void clearList() { 
		TNode<T>* p = head;
		TNode<T>* q;
		while (p) {
			q = p;
			p = p->right;
			delete q;
			q = NULL;
		}
		head = NULL;
	}
	~DlkList() { clearList(); }
	TNode<T>* head;
	// inline bool findElem(T val, &TNode<T>* where);
	inline bool leftInsert(TNode<T>* ptr_in, TNode<T>* op);
	inline bool rightInsert(TNode<T>* ptr_in, TNode<T>* op);
	inline bool deleteNode(TNode<T>* ptr_op);
	inline bool initialInsert(TNode<T>* op);
	inline void printList();
};

template <class T>
bool DlkList<T>::leftInsert(TNode<T>* ptr_in, TNode<T>* ptr_op) {
	// ptr_in --student already in
	// op --student to be inserted
	//TNode<T>* ptr_op = new TNode<T>(op);
	if (ptr_in == head) {
		TNode<T>* tmp = head;
		head = ptr_op;
		ptr_op->right = tmp;
		ptr_op->left = NULL;
		tmp->left = ptr_op;
		return true;
	} 
	TNode<T>* p_left = ptr_in->left;
	p_left->right = ptr_op;
	ptr_op->left = p_left;
	ptr_op->right = ptr_in;
	ptr_in->left = ptr_op;
	return true;
}

template <class T>
bool DlkList<T>::rightInsert(TNode<T>* ptr_in, TNode<T>* ptr_op) {
	// ptr_in --student already in
	// op --student to be inserted
	//TNode<T>* ptr_op = new TNode<T>(op);
	if (ptr_in->right==NULL) {
		ptr_in->right = ptr_op;
		ptr_op->left = ptr_in;
		ptr_op->right = NULL;
		return true;
	}
	TNode<T>* p_right = ptr_in->right;
	p_right->left = ptr_op;
	ptr_op->right = p_right;
	ptr_in->right = ptr_op;
	ptr_op->left = ptr_in;
	return true;
}

template <class T>
bool DlkList<T>::deleteNode(TNode<T>* ptr_op) {
	/* delete ptr_op from linked list */
	if (head == NULL) return false;
	if (ptr_op == head) { // ptr_op is the first node in the list
		head = head->right;
		if(head != NULL) head->left = NULL; // if ptr_op is not the only node in the list
		delete ptr_op;
		ptr_op = NULL;
		return true;
	}
	else { // usual cases
		ptr_op->left->right = ptr_op->right;
		if(ptr_op->right != NULL ) ptr_op->right->left = ptr_op->left; // if ptr_op not the end
		delete ptr_op;
		ptr_op = NULL;
		return true;
	}
}

template <class T>
bool DlkList<T>::initialInsert(TNode<T>* ptr_op) {
	if (head != NULL) return false; // this function is useful only when the list is empty
	//TNode<T>* ptr_op = new TNode<T>(op);
	head = ptr_op;
	ptr_op->right = NULL;
	ptr_op->left = NULL;
}

template <class T>
void DlkList<T>::printList() {
	TNode<T>* p = head;
	while (p != NULL) {
		p->display();
		printf(" ");
		p = p->right;
	}
	printf("-1");
}
// use a list
TNode<int>* ptrs[500000];
DlkList<int> humanCentipede;

int main() {
	int N, M;
	scanf("%d", &N);
	scanf("%d", &M);
	int op; // operation: in or out
	int stu_op; // student to be inserted or removed
	int stu_in; // student in the queue for the new student to be placed next to(inserting only)
	int lor; // left or right (inserting only)
	for (int i = 0; i < N + M; i++) {
		scanf("%d", &op);
		scanf("%d", &stu_op);
		if (op) { // inserting
			scanf("%d", &stu_in);
			scanf("%d", &lor);
			if (stu_in == -1 && lor == -1) { // nobody in list; insert the first person
				TNode<int>* tmp = new TNode<int>(stu_op);
				humanCentipede.initialInsert(tmp);
				ptrs[stu_op] = tmp;
				tmp = NULL;
			}
			else if (lor == 1) { // right insert
				TNode<int>* tmp = new TNode<int>(stu_op);
				humanCentipede.rightInsert(ptrs[stu_in], tmp);
				ptrs[stu_op] = tmp;
				tmp = NULL;
			}
			else{ // left insert
				TNode<int>* tmp = new TNode<int>(stu_op);
				humanCentipede.leftInsert(ptrs[stu_in], tmp);
				ptrs[stu_op] = tmp;
				tmp = NULL;
			}
		}
		else { // removing
			humanCentipede.deleteNode(ptrs[stu_op]);
		}
	}
	humanCentipede.printList();
	/* test TNode an DlkList */
	//TNode<int>* t = new TNode<int>(32);
	//t->display();
	//delete t;
	//t = NULL;
	/*DlkList<int> lst;
	lst.initialInsert(1);
	TNode<int>* p = lst.head;
	lst.leftInsert(p, 2);
	lst.rightInsert(p, 3);
	lst.leftInsert(p, 4);
	lst.printList();
	lst.deleteNode(p);
	lst.printList();*/
	return 0;
}
