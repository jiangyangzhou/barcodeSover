#ifndef SEQLIST_H_INCLUDED
#define SEQLIST_H_INCLUDED
#include<iostream>

using namespace std;

template <class T>
class seqList :public list<T>
{
	template <class U>
	friend seqList<U> operator+(const seqList<U> &s1, const seqList<U> &s2);
private:
	T *data;
	int len;
	int maxSize;
	void enLarge(int size) {
		T *p = data;
		maxSize = size;
		data = new T[maxSize];
		for (int i = 0; i<len; i++) {
			data[i] = p[i];
		}
		delete[] p;
	};
public:
	seqList(int initSize = 10);
	seqList(T *arr, int l);
	seqList(const seqList<T> &x);
	~seqList();
	void clear();
	int length()const { return len; }
	void insert(int i, const T &x);
	void remove(int i);
	int search(const T &x) const;
	T visit(int i) const;
	void traverse() const;
	void combine(const seqList &x);
};

template <class T>
seqList<T>::seqList(int initSize) {
	if (initSize<0)  cout << "Size Error!";
	else {
		data = new T[initSize];
		maxSize = initSize;
		len = 0;
	}
}

template <class T>
seqList<T>::seqList(T *arr, int l) {
	len = l;
	maxSize = len + 10;
	data = new T[maxSize];
	for (int i = 0; i<len; i++) {
		data[i] = arr[i];
	}
}

template <class T>
seqList<T>::seqList(const seqList<T> &x) {
	len = x.len;
	maxSize = x.maxSize;
	data = new T[maxSize];
	for (int i = 0; i<len; i++) {
		data[i] = x.data[i];
	}
}


template <class T>
seqList<T>::~seqList() {
	delete[] data;
	len = 0;
}


template <class T>
void seqList<T>::clear() {
	len = 0;
}


template <class T>
void seqList<T>::insert(int i, const T &x) {
	if (i<-1)  cout << "out of range!";
	else if (i == -1) {
		data[len] = x; len++; return;
	}
	else {
		if (i>len) enLarge(len + 10);
		for (int j = len; j>i; j--) {
			data[j] = data[j - 1];
		}
		data[i] = x;
		len++;
	}
}

template <class T>
void seqList<T>::remove(int i) {
	if (i >= len || i<0) cout << "indexError";
	else {
		for (int j = i - 1; j<len - 1; j++) {
			data[j] = data[j + 1];
		}
		len--;
	}
}

template <class T>
int seqList<T>::search(const T &x) const {
	for (int j = 0; j<len; j++) {
		if (data[j] == x) return j;
	}
	return -1;
}

template <class T>
T seqList<T>::visit(int i) const {
	if (i == -1) return data[len - 1];
	else if (i >= len || i<-1) cout << "indexError";
	else return data[i];
}

template <class T>
void seqList<T>::traverse()const {
	for (int i = 0; i<len; i++) {
		cout << data[i] << " ";
	}
	cout << endl;
}

template <class T>
void seqList<T>::combine(const seqList &x) {
	if (maxSize<len + x.len)
		enLarge(len + x.len + 10);
	for (int j = len; j<len + x.len; j++) {
		data[j] = x.data[j - len];
	}
	len += x.len;
}

template <class U>
seqList<U> operator+ (const seqList<U> &s1, const seqList<U> &s2) {
	seqList<U> news(s1);
	//cout<<s1.data[0]<<" "<<s1.data[1]<<endl;
	//cout<<news.data[0]<<" "<<news.data[1]<<endl;
	//return news;
	if (news.maxSize<s1.len + s2.len)
		news.enLarge(s1.len + s2.len + 10);
	for (int j = news.len; j<s1.len + s2.len; j++) {
		news.data[j] = s2.data[j - s1.len];
	}
	news.len = s1.len + s2.len;
	//cout<<s1.data[0]<<" "<<s1.data[1]<<endl;
	//cout<<news.data[0]<<" "<<news.data[1]<<endl;
	return news;
}



#endif // SEQLIST_H_INCLUDED
