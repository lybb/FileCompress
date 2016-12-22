#pragma once

#include<queue>
#include<stack>
#include<iostream>
using namespace std;
#include<assert.h>

template<class T>
struct BinaryTreeNode
{
	BinaryTreeNode(const T& x = T())
		:_data(x)
		,_left(NULL)
		,_right(NULL)
	{}

	T _data;		//数值域
	BinaryTreeNode<T>* _left;	//左子树
	BinaryTreeNode<T>* _right;	//右子树
};

template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree()
		:_root(NULL)
	{}
	BinaryTree(T* a,size_t size,const T& invalid)
	{
		size_t index = 0;
		_root = _CreatTree(a,size,index,invalid);
	}
	BinaryTree(const BinaryTree<T>& t)
	{
		_root = _Copy(t._root);
	}
	BinaryTree<T>& operator=(const BinaryTree<T>& t)
	{
		if (this != &t)
		{
			BinaryTree<T> tmp(t);	//拷贝构造
			std::swap(_root,tmp._root);
		}
	}
	~BinaryTree()
	{
		_Destory(_root);
	}

	void PrevOrder()		//前序
	{
		if (_root)
			_prevOrder(_root);
		cout<<endl;
	}
	void InOrder()		//中序
	{
		if (_root)
			_inOrder(_root);
		cout<<endl;
	}
	void PostOrder()	//后序
	{
		if(_root)
			_postOrder(_root);
		cout<<endl;
	}
	void LevelOrder()	//层序
	{
		_LevelOrder(_root);
	}
	void PrevOrder_NonR()
	{
		_PrevOrder_NonR(_root);
	}
	void InOrder_N0nR()
	{
		_InOrder_N0nR(_root);
	}
	void PostOrder_NonR()
	{
		_PostOrder_NonR(_root);
	}
	Node* Find(const T& x)
	{
		return _find(_root,x);
	}
	size_t Size()	//结点个数
	{
		return _Size(_root);
	}
	size_t Depth()	//深度
	{
		return _Depth(_root);
	}
	size_t LeafSize()	//叶子结点的个数
	{
		return _LeafSize(_root);
	}
	size_t GetKLevel(const int& k)		//第K层结点的个数
	{
		return _GetKLevel(_root,k);
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _CreatTree(T* a,size_t size,size_t& index,const T& invalid)
	{
		assert(a);
		Node* root = NULL;
		if((index<size) && a[index]!=invalid)
		{
			root = new Node(a[index]);		//构造根节点
			root->_left = _CreatTree(a,size,++index,invalid);	//左子树
			root->_right = _CreatTree(a,size,++index,invalid);	//右子树
		}
		return root;
	}
	Node* _Copy(Node* root)
	{
		if (root == NULL)
		{
			return NULL;
		}
		Node* newroot = NULL;
		if (root)
		{
			newroot = new Node(root->_data);
			newroot->_left = _Copy(root->_left);
			newroot->_right = _Copy(root->_right);
		}
		return newroot;
	}
	void _Destory(Node* root)
	{
		if (root == NULL)
			return;

		_Destory(root->_left);
		_Destory(root->_right);
		delete root;
	}

	void _prevOrder(Node* root)	//前---根、左、右---1 2 3 4 5 6
	{
		if(root == NULL)
			return ;
		Node* cur = root;
		if(cur)
		{
			cout<<cur->_data<<" ";
			_prevOrder(cur->_left);
			_prevOrder(cur->_right);
		}
	}
	void _inOrder(Node* root)		//中---左、根、右---3 2 4 1 6 5
	{
		if(root == NULL)
			return ;
		Node* cur = root;
		if (cur)
		{
			_inOrder(cur->_left);
			cout<<cur->_data<<" ";
			_inOrder(cur->_right);
		}
	}
	void _postOrder(Node* root)		//后---左、右、根---3 4 2 6 5 1
	{
		if (root == NULL)
			return ;
		Node* cur = root;
		if (cur)
		{
			_postOrder(cur->_left);
			_postOrder(cur->_right);
			cout<<cur->_data<<" ";
		}
	}
	void _LevelOrder(Node* root)
	{
		if (NULL == root)
			return ;
		queue<Node*> q;
		Node* cur = root;
		q.push(root);
		while (!q.empty())	//队列不为空说明有数据可以访问
		{
			Node* tmp = q.front();	//以根结点带动左右子树
			cout<<tmp->_data<<" ";
			if (tmp->_left)
				q.push(tmp->_left);
			if (tmp->_right)
				q.push(tmp->_right);
			q.pop();
		}
		cout<<endl;
	}

	void _PrevOrder_NonR(Node* root)	//非递归--前序
	{
		if(root == NULL)
			return ;
		Node* cur = root;
		stack<Node*> s;
		while (cur || !s.empty())		//cur != NULL说明仍然有子树可以被遍历
										//!s.empty()  说明栈不为空时可以有数据被访问
		{
			while (cur)
			{
				cout<<cur->_data<<" ";
				s.push(cur);
				Node* tmp = s.top();
				T x=tmp->_data;
				cur = cur->_left;
			}

			Node* top = s.top();
			s.pop();
			cur = top->_right;	//访问当前根节点的右子树
		}
		cout<<endl;
	}
	void _InOrder_N0nR(Node* root)		//非递归--中序
	{
		if(root == NULL)
			return ;
		Node* cur = root;
		stack<Node*> s;
		while(cur || !s.empty())
		{
			while(cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			Node* top = s.top();	//此时栈顶存的是当前子树的根节点
			cout<<top->_data<<" ";
			s.pop();
			cur = top->_right;		//将右子树作为左子树的子树来遍历
		}
		cout<<endl;
	}
	void _PostOrder_NonR(Node* root)	//非递归--后序
	{
		Node* cur = root;
		Node* pos = root;
		stack<Node*> s;
		while (cur || !s.empty())
		{
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			Node* top = s.top();

			if (top->_right == NULL || top->_right == pos)
				//top存的是当前的根节点，当top的右子树为空或者top的右子树为pos，说明右子树已经遍历过，
				//这时就可以访问当前的根节点了
			{
				cout<<top->_data<<" ";
				pos = top;
				s.pop();
			}
			else
			{
				cur = top->_right;
			}
		}
		cout<<endl;
	}
	Node* _find(Node* root,const T& data)
	{
		if (root == NULL)	//空树
			return NULL;
		if(root->_data == data)	//根节点的数据为要查找的数据
			return root;

		Node* ret = _find(root->_left,data);	//先遍历左子树
		if (ret == NULL)	//左子树已经遍历完并且没有找到，开始在右子树中查找
		{
			ret = _find(root->_right,data);
		}
		return ret;
	}

	//size_t _Size(Node* root)
	//{
	//	if (root == NULL)
	//		return NULL;

	//	size_t sz = 0;
	//	sz =  _Size(root->_left)+ _Size(root->_right);
	//	return sz+1;	//左子树 + 右子树 + 根节点
	//}
	size_t _Size(Node* root)		//每走一个节点，count++
	{
		if (root == NULL)
			return 0;
		static size_t count = 0;
		++count;
		if (root)
		{
			_Size(root->_left);
			_Size(root->_right);
		}
		return count;
	}
	size_t _Depth(Node* root)
	{
		size_t leftsz = 0;
		size_t rightsz = 0;
		if (NULL == root)
			return 0;
		else
		{
			leftsz += _Depth(root->_left);		//遍历左子树的深度
			rightsz += _Depth(root->_right);	//遍历右子树的深度
		}
		return leftsz>rightsz?leftsz+1:rightsz+1;	//左右中深度中最大的加上根节点并返回
	}
	size_t _LeafSize(Node* root)
	{
		if (NULL == root)
			return 0;
		Node* cur = root;
		static size_t sz = 0;
		if (cur)
		{
			if (cur->_left == NULL && cur->_right == NULL)
			{
				sz+=1;
			}
			else
			{
				_LeafSize(cur->_left);
				_LeafSize(cur->_right);
			}
		}
		return sz;
	}
	size_t _GetKLevel(Node* root,const int& k)
	{
		if (NULL == root)
			return 0;
		if (k == 1 && root)		//第一层，并且不为空树，只有一个根节点
			return 1;
		Node* cur = root;
		size_t sz = 0;
		if (cur)
 		{
			sz += _GetKLevel(cur->_left,k-1);
			sz += _GetKLevel(cur->_right,k-1);
		}
	}
protected:
	Node* _root;
};



void TestBinaryTree()
{
	int a1[10] = {1,2,3,'#','#',4,'#','#',5,6};
	int a2[15] = {1,2,'#',3,'#','#',4,5,'#',6,'#',7,'#','#',8};
	BinaryTree<int> t1(a1,10,'#');
	BinaryTree<int> T1(a2,15,'#');
	BinaryTree<int> t2(t1);
	BinaryTree<int> t3 = t1;
	t1.PrevOrder();			//1 2 3 4 5 6
	t1.PrevOrder_NonR();	//
	t1.InOrder();			//3 2 4 1 6 5
	t1.InOrder_N0nR();
	t1.PostOrder();			//3 4 6 2 5 1
	t1.PostOrder_NonR();

	T1.PrevOrder();	
	T1.PrevOrder_NonR();
	T1.InOrder();
	T1.InOrder_N0nR();
	T1.PostOrder();
	T1.PostOrder_NonR();
	cout<<"层序："<<endl;
	t1.LevelOrder();
	T1.LevelOrder();

	//BinaryTreeNode<int>* ret = t1.Find(6);
	//cout<<"Find(6):"<<ret->_data<<endl;
	//BinaryTreeNode<int>* ret2 = t1.Find(8);
	//cout<<"Find(8)"<<ret2<<endl;
	//cout<<"Size:"<<t1.Size()<<endl;
	//cout<<"Size:"<<T1.Size()<<endl;
	//cout<<"Depth:"<<t1.Depth()<<endl;
	//cout<<"Depth:"<<T1.Depth()<<endl;
	//cout<<"LeafSize:"<<t1.LeafSize()<<endl;
	//cout<<"LeafSize:"<<T1.LeafSize()<<endl;
	//cout<<"GetKlevelNode:"<<t1.GetKLevel(3)<<endl;
	//cout<<"GetKlevelNode:"<<T1.GetKLevel(4)<<endl;
}																																																																																																												