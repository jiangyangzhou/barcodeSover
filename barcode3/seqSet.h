#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED
#include <iostream>
using namespace std;

template <class T>
class seqSet {
	//ģ������Ԫ������ģ��Ҫ��������
	template <typename U>
	friend seqSet<U> operator* (const seqSet<U> &a, const seqSet<U> &b);  //��
	template <typename U>
	friend seqSet<U> operator+ (const seqSet<U> &a, seqSet<U> &b);  //��
	template <typename U>
	friend seqSet<U> operator- (const seqSet<U> &a, const seqSet<U> &b);  //��
private:
	T *elem;
	int maxSize;
	int currentSize;
	void doubleSize();

public:

	seqSet(int Msize = 20);
	seqSet(const seqSet<T> &a);
	~seqSet() { delete[] elem; };
	seqSet &operator=(const seqSet<T> &a);
	const int Get_Size() { return currentSize; }
	bool exist(const T &x)const;
	bool insert(const T &x);
	bool erase(const T &x);
	T visit(int i) { return elem[i]; }
	void display();
	T get_minelem();

};

template <class T>
seqSet<T>::seqSet(int Msize) {
	currentSize = 0;
	maxSize = Msize;
	elem = new T[maxSize];
}

template <class T>
seqSet<T>::seqSet(const seqSet<T> &a) {
	currentSize = a.currentSize;
	maxSize = a.maxSize;
	elem = new T[maxSize];
	for (int i = 0; i<currentSize; i++)
	{
		elem[i] = a.elem[i];
	}
}

template <class T>
seqSet<T>& seqSet<T>::operator=(const seqSet<T> &a) {    //��ֵ�������
	maxSize = a.maxSize;
	currentSize = a.currentSize;
	T * temp = elem;
	delete[] temp;
	elem = new T[maxSize];
	for (int i = 0; i<maxSize; i++) {
		elem[i] = a.elem[i];
	}
	return *this;

}

template <class T>
void seqSet<T>::doubleSize() {     //����ռ�
	maxSize *= 2;
	T *temp = elem;
	elem = new T[maxSize];
	for (int i = 0; i<currentSize; i++) {
		elem[i] = temp[i];
	}
	delete[] temp;
}

template <class T>
bool seqSet<T>::exist(const T &x)const {
	for (int i = 0; i<currentSize; i++) {
		if (elem[i] == x) return true;
	}
	return false;
}

template <class T>
bool seqSet<T>::insert(const T &x) {
	if (exist(x)) return false;
	if (maxSize == currentSize)  doubleSize();
	elem[currentSize] = x;
	currentSize++;
	return true;
}

template <class T>
bool seqSet<T>::erase(const T &x) {
	bool flag = false;
	for (int i = 0; i<currentSize; i++) {
		if (elem[i] == x) {
			flag = true;
			currentSize--;
			for (int j = i; j<maxSize; j++)
			{
				elem[j] = elem[j + 1];
			}
			break;
		}
	}
	return flag;
}

template <class T>
void seqSet<T>::display() {
	for (int i = 0; i<currentSize; i++) {
		cout << elem[i] << " ";
	}
	cout << endl;


}

template <class T>
T seqSet<T>::get_minelem() {
	T mina = elem[0];
	for (int i = 0; i < currentSize; i++)
		if (elem[i] < mina)
			mina = elem[i];
	
	return mina;
}

template <class T>
seqSet<T> operator* (const seqSet<T> &a, const seqSet<T> &b)  //��
{
	seqSet<T> newSet(a.currentSize);
	for (int i = 0; i<a.currentSize; i++) {
		if (b.exist(a.elem[i]))
			newSet.insert(a.elem[i]);
	}
	return newSet;
}

template <class T>
seqSet<T> operator+ (const seqSet<T> &a, seqSet<T> &b) //��
{
	seqSet<T> newSet(a);
	for (int i = 0; i< b.currentSize; i++) {
		if (!a.exist(b.elem[i]))
			newSet.insert(b.elem[i]);
	}
	return newSet;

}

template <class T>
seqSet<T> operator- (const seqSet<T> &a, const seqSet<T> &b)  //��
{
	seqSet<T> newSet(a.currentSize);
	for (int i = 0; i<a.currentSize; i++) {
		if (!b.exist(a.elem[i]))
			newSet.insert(a.elem[i]);
	}
	return newSet;
}


#endif // SET_H_INCLUDED
