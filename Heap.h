#pragma once

#include<iostream>
using namespace std;
#include <vector>
#include <assert.h>

template<class T>
struct Less
{
	bool operator()(const T& left,const T& right)
	{
		return left < right;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& left,const T& right)
	{
		return left > right;
	}
};

//堆
template<class T,class Compare = Less<T>>	//默认为大堆
class Heap
{
public:
	Heap()
	{}
	Heap(T* a,int size)
	{
		assert(a);
		_heap.reserve(size);
		for(int i=0; i<size; ++i)
		{
			_heap.push_back(a[i]);
		}
		//建堆
		for (int j=(size-2)/2; j>=0; --j)	//从下向上，找到第一个非叶子结点
		{
			AdjustDown(j);
		}
	}
	void AdjustDown(int root)	//向下调整
	{
		int parent = root;
		size_t child = parent * 2 + 1;
		while (child < _heap.size())
		{
			Compare com;
			//选左右孩子中较大的一个
			/*if ((child+1) < _heap.size()
				&& _heap[child+1] > _heap[child])*/
			size_t sz = _heap.size();
			if ((child+1) < sz
				&& com(_heap[child+1],_heap[child]))
			{
				++child;
			}
			//选父节点与孩子节点中较大的一个
			/*if (_heap[child] > _heap[parent])*/
			if (com(_heap[child],_heap[parent]))
			{
				swap(_heap[child],_heap[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	void Push(const T& x)
	{
		_heap.push_back(x);			//在最后插入一个数字
		AdjustUp(_heap.size()-1);	//从最后插入数字的位置向上调整
	}
	void Pop()
	{
		assert(!_heap.empty());
		swap(_heap[0],_heap[_heap.size()-1]);	//交换堆顶与最后一个元素
		_heap.pop_back();	//删除最后一个元素(删除了堆顶的元素)
		AdjustDown(0);		//从第一个数向下调整
	}
	bool Empty()
	{
		return _heap.empty();
	}
	size_t Size()
	{
		size_t sz = _heap.size();
		return sz;
	}
	const T& Top()
	{
		return _heap[0];
	}
	void Display()
	{
		for (size_t i=0; i<Size(); ++i)
		{
			cout<<_heap[i]<<" ";
		}
		cout<<endl;
	}
protected:
	
	void AdjustUp(int child)		//向上调整
	{
		int parent = (child-1)/2;
		while(child > 0)
		{
			Compare com;
			//if (_heap[child] > _heap[parent])	//孩子结点大于父节点就交换
			if(com(_heap[child],_heap[parent]))
			{
				swap(_heap[child],_heap[parent]);
				child = parent;
				parent = (child-1)/2;
			}
			else
				break;
		}
	}
protected:
	vector<T> _heap;
};


void _AdjustDown(int* a,int root,int size)	//向下调整
{
	int parent = root;
	int child = parent * 2 + 1;
	while (child < size)
	{
		if ((child+1) < size
			&& a[child+1] > a[child])	//当右子树存在并且右子树大于左子树
		{
			++child;
		}
		if (a[child] > a[parent])
		{
			swap(a[child],a[parent]);
			parent = child;
			child = 2*parent +1;
		}
		else
			break;
	}
}
void HeapSort(int* a,int size)
{
	//建堆
	for (int i=(size-1)/2; i>=0; --i)
	{
		_AdjustDown(a,i,size);
	}
	
	//排序
	int index = size-1;
	for (int i=0; i<size; ++i)
	{
		swap(a[0],a[index]);	//交换堆顶与最后一个数-------将最大（小）的数据排到最后
		_AdjustDown(a,0,index);	//从堆顶开始重新向下调整
		--index;
	}
	for (int j=0; j<size; ++j)
	{
		cout<<a[j]<<" ";
	}
	cout<<endl;
}



//优先级队列
template<class T,class Compare = Greater<T>>
class PriorityQueue
{
public:
	PriorityQueue(T* a,int size)
		:_pq(a,size)
	{}
	void Push(const T& x)
	{
		_pq.Push(x);
	}
	void Pop()
	{
		_pq.Pop();
	}
	const T& Top()
	{
		return _pq.Top();
	}
	void Display()
	{
		_pq.Display();
	}
protected:
	Heap<T,Compare> _pq;
};


//void TestPriorityQueue()
//{
//	int a[] = { 10, 16, 18, 12, 11, 13, 15, 17, 14, 19 }; 
//	int sz = sizeof(a)/sizeof(a[0]);
//	PriorityQueue<int,Greater<int>> pq1(a,sz);
//	cout<<"大堆： ";
//	pq1.Display();
//	cout<<"Top: "<<pq1.Top()<<endl;
//
//	pq1.Push(30);
//	cout<<"Push(30)： ";
//	pq1.Display();
//
//	pq1.Pop();
//	cout<<"Pop()： ";
//	pq1.Display();
//}


void TestHeap()
{
	int a[] = { 20, 16, 19, 18, 12, 11}; 
	int sz = sizeof(a)/sizeof(a[0]);
	//Heap<int,Greater<int>> hp1(a,sz);
	//cout<<"大堆： ";
	//hp1.Display();
	Heap<int,Less<int>> hp2(a,sz);
	cout<<"小堆： ";
	hp2.Display();
	hp2.Push(3);
	hp2.Display();
	//HeapSort(a,sz);
	

	//cout<<"Empty: "<<hp1.Empty()<<endl;
	//cout<<"Size: "<<hp1.Size()<<endl;
	//cout<<"Top: "<<hp1.Top()<<endl;

	//hp1.Push(30);
	//cout<<"Push(30)： ";
	//hp1.Display();

	//hp1.Pop();
	//cout<<"Pop()： ";
	//hp1.Display();

	//Heap<int,Less<int>> hp2(a,sz);
	//cout<<"小堆： ";
	//hp2.Display();

	cout<<endl;
}
//void TestPriorityQueue()
//{
//	int a[] = { 10, 16, 18, 12, 11, 13, 15, 17, 14, 19 }; 
//	int sz = sizeof(a)/sizeof(a[0]);
//	PriorityQueue<int,Greater<int>> pq1(a,sz);
//	cout<<"大堆： ";
//	pq1.Display();
//	cout<<"Top: "<<pq1.Top()<<endl;
//
//	pq1.Push(30);
//	cout<<"Push(30)： ";
//	pq1.Display();
//
//	pq1.Pop();
//	cout<<"Pop()： ";
//	pq1.Display();
//
//	Heap<int,Less<int>> pq2(a,sz);
//	cout<<"小堆： ";
//	pq2.Display();
//}