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
	
	SingleLinkList() {   //���캯��
		this->size = 0;
		head = new node;
	}
	SingleLinkList(T *arr, int len) {  //�����鹹�쵥����
		size = 0;
		head = new node;
		int j = 0;
		cout << "len  " << len << endl;
		for (j = 0; j<len; j++) {
			this->insert(j + 1, arr[j]);
		}
	}
	SingleLinkList(const SingleLinkList<T> &l) {  //��������
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

	void clear() {   //�������
		node *n = head;
		for (int i = size; i>0; i--)
			remove(i);
	}

	int length()const {   //��ȡ��������
		return size;
	}

	void insert(int i, const T &x){  //����Ԫ�أ���1��ʼ
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

	void remove(int i) {  //��ȥ��i��Ԫ��
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

	int search(T x)const {  //����Ԫ��x,���ص�һ��Ԫ��λ��
							//�Ҳ�������-1
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
	T visit(int i) const{  //��ȡ��i��Ԫ�ص�ֵ
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
	void traverse() const{  //��������
		node *n;
		n = head->next;
		while (n != NULL) {
			cout << (n->data) << ",";
			n = n->next;
		}
		cout << endl;
	}
	void reverse() {   //��ת����
		node *n = head->next, *p = head->next, *q = head->next;   //�����������
		for (int i = 0; i<size; i++) {
			n = q;               //n=ԭ����n->next
			q = n->next;        //����n->next
			n->next = p;          //next��ת,p��n����һ��
			p = n;              //p����Ϊԭ����p->next
		}
		head->next->next = NULL; // ��һ�����next����
		head->next = n;     //ͷ���ָ��β
	}



	~SingleLinkList() {
		clear();
	}

	T pop(int i) {  //��ȡ��i��������ɾ����
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
	node* getnode(int i) {  //��ȡ��i�����
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
