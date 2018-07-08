#ifndef SINGLELINKLIST_H
#define SINGLELINKLIST_H
#include<iostream>
using namespace std;

template <class T>
class SingleLinkList
{
private:
	struct node {
		T data;
		node *next;
		node() :next(NULL) {}
		node(const T &x, node *n = NULL) {
			data = x;
			next = n;
		}
		node(node &n) {
			data = n.data;
			next = new node;
			next = n.next;
		}
		~node() {}
	};
	node *head;
	int size;
public:
	
	SingleLinkList() {   //构造函数
		this->size = 0;
		head = new node;
	}
	SingleLinkList(T *arr, int len) {  //由数组构造单链表
		size = 0;
		head = new node;
		int j = 0;
		cout << "len  " << len << endl;
		for (j = 0; j<len; j++) {
			this->insert(j + 1, arr[j]);
		}
	}
	SingleLinkList(const SingleLinkList<T> &l) {  //拷贝构造
		head = new node();
		size = 0;
		for (int j = 1; j <= l.size; j++) {
			this->insert(j, l.visit(j));
		}
	}
	SingleLinkList& operator=(const SingleLinkList &other) {
		if (this == &other)
			return *this;
		delete head;
		head = new node();
		size = 0;
		for (int j = 1; j <= other.size; j++) {
			this->insert(j, other.visit(j));
		}
		return *this;
	}

	void clear() {   //清空链表
		node *n = head;
		for (int i = size; i>0; i--)
			remove(i);
	}

	int length()const {   //获取单链表长度
		return size;
	}

	void insert(int i, const T &x){  //插入元素，从1开始
		if (i<0)    i = size - i;
		if (i>size + 1)  i = size;
		if (i == 0) i = 1;
		size++;
		node *n, *p;
		n = head;
		for (int j = 0; j<i - 1; j++) {
			n = n->next;
		}
		p = n->next;
		n->next = new node(x);
		n->next->next = p;
	}

	void remove(int i) {  //移去第i个元素
		if (i<0)   i = size - i;
		if (i>size)   i = size;
		size--;
		node *n, *p;
		n = head;
		for (int j = 0; j<i - 2; j++) {
			n = n->next;
		}
		p = n->next;
		n->next = p->next;
		delete p;
	}

	int search(T x)const {  //查找元素x,返回第一个元素位置
							//找不到返回-1
		int i = 0;
		node *n;
		n = head;
		while (n != NULL) {
			if (n->data == x)
				return i;
			n = n->next;
			i++;
		}
		return -1;
	}
	T visit(int i) const{  //获取第i个元素的值
		if(i<=0||i>size)   cout<<"out of range!,i="<<i<<endl;
		int j = 0;
		node *n;
		n = head;
		while (j<i) {
			j++;
			n = n->next;
		}
		return n->data;
	}
	void traverse() const{  //遍历链表
		node *n;
		n = head->next;
		while (n != NULL) {
			cout << (n->data) << ",";
			n = n->next;
		}
		cout << endl;
	}
	void reverse() {   //反转链表
		node *n = head->next, *p = head->next, *q = head->next;   //声明三个结点
		for (int i = 0; i<size; i++) {
			n = q;               //n=原来的n->next
			q = n->next;        //保存n->next
			n->next = p;          //next反转,p是n的上一个
			p = n;              //p更新为原来的p->next
		}
		head->next->next = NULL; // 第一个结点next置零
		head->next = n;     //头结点指向尾
	}



	~SingleLinkList() {
		clear();
	}

	T pop(int i) {  //获取第i个数，并删了它
		if (i<0)   i = size - i;
		if (i>size)   i = size;
		size--;
		node *n, *p;
		n = head;
		for (int j = 0; j<i - 2; j++) {
			n = n->next;
		}
		p = n->next;
		n->next = p->next;
		return p->data;

	}

protected:
	node* getnode(int i) {  //获取第i个结点
		node *n;
		int j = 0;
		n = head;
		while (j<i) {
			j++;
			n = n->next;
		}
		return n;
	}
};

#endif // SINGLELINKLIST_H
