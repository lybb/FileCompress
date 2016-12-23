#define  _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
using namespace std;
#include <string>
#include "Heap.h"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode()
	{}
	HuffmanTreeNode(const T& weight)
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_weight(weight)
	{}

	HuffmanTreeNode<T>* _left;			//左孩子
	HuffmanTreeNode<T>* _right;		//右孩子
	HuffmanTreeNode<T>* _parent;		//父亲结点
	T _weight;		//权值
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	{}
	HuffmanTree(T* a,size_t size,const T& invalid)
	{
		assert(a);
		struct NodeLess
		{
			bool operator()(Node* left,Node* right)const
			{
				return (left->_weight < right->_weight);
			}
		};

		Heap<Node* ,NodeLess> minheap;

		for (size_t i=0; i<size; ++i)	//建小堆，将最小的值放在堆顶
		{
			if (a[i] != invalid)
			{
				Node* node = new Node(a[i]);
				minheap.Push(node);
			}
		}

		//构建哈夫曼树
		while (minheap.Size() > 1)
		{
			Node* left = minheap.Top();		//分别取最小的两个值给left和right
			minheap.Pop();
			Node* right = minheap.Top();
			minheap.Pop();

			Node* parent = new Node(left->_weight + right->_weight);	//构建父节点

			parent->_left = left;	//使left和right指向父节点，父节点的左右分别指向left和right
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			minheap.Push(parent);
		}
		_root = minheap.Top();
		minheap.Pop();
	}

	~HuffmanTree()
	{
		_Destory(_root);
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:

	void _Destory(Node* root)
	{
		if (root == NULL)
			return ;

		Node* del = root;
		if (del)
		{
			_Destory(root->_left);
			_Destory(root->_right);
			delete del;
			del = NULL;
		}
	}
protected:
	Node* _root;
};

void TestHuffmanTree()
{
	int a[] = {1,2,3,4,5};
	size_t size = sizeof(a)/sizeof(a[0]);
	HuffmanTree<int> ht1(a,size,0);
}