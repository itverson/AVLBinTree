#include<iostream>
using namespace std;
template<class K, class V>
struct AVLBinTreeNode
{
	AVLBinTreeNode<K, V> *_pLeft;
	AVLBinTreeNode<K, V>* _pRight;
	AVLBinTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	int _bf;
	AVLBinTreeNode(const K& key, const V& value)
		:_pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _bf(0){}
};
template<class K, class V>
class AVLBinTree
{
public:
	typedef AVLBinTreeNode<K, V> Node;
	typedef Node* PNode;
private:
	PNode _pRoot;
public:
	AVLBinTree()
		:_pRoot(NULL){}
	bool Insert(const K& key, const V& value)
	{
		if (_pRoot == NULL)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		PNode pParent = NULL;
		PNode cur = _pRoot;
		while (cur)//找位置
		{
			if (cur->_key == key)
				return false;
			else if (cur->_key > key)
			{
				pParent = cur;
				cur = cur->_pLeft;
			}
			else
			{
				pParent = cur;
				cur = cur->_pRight;
			}
		}
		PNode pNewNode = new Node(key, key);
		if (pParent->_key > key)
		{
			pParent->_pLeft = pNewNode;
			pNewNode->_pParent = pParent;
		}
		else
		{
			pParent->_pRight = pNewNode;
			pNewNode->_pParent = pParent;
		}
		//插入以后要开始调整
		while (pParent)
		{
			//首先计算父结点平衡因子
			if (pParent->_pLeft == pNewNode)//插入在当前结点的左边则bf--
			{
				pParent->_bf--;
			}
			else
			{
				pParent->_bf++;
			}
			//根据父结点平衡因子判断是否需要调整
			if (pParent->_bf == 0)//平衡
			{
				return true;
			}
			else  if (pParent->_bf == 1 || pParent->_bf == -1)//也是平衡,往回找判断
			{
				pNewNode = pParent;
				pParent = pParent->_pParent;
			}
			else if (pParent->_bf == 2 || pParent->_bf == -2)
			{
				if (pParent->_bf == 2)//左边的结点比较多
				{
					if (pNewNode->_bf == 1)
					{
						_RotateL(pParent);//
						return true;
					}
					else
					{
						_RotateRL(pParent);//右左旋转
						return true;
					}
				}
				if (pParent->_bf == -2)
				{
					if (pNewNode->_bf == -1)
					{
						_RotateR(pParent);
						return true;
					}
					else
					{
						_RotateLR(pParent);
						return true;
					}
				}
			}
		}
		return true;
	}
	void _RotateL(PNode pParent)//左旋
	{
		PNode SubR = pParent->_pRight;
		PNode SubRL = SubR->_pLeft;
		pParent->_pRight = SubRL;
		if (SubRL)
		{
			SubRL->_pParent = pParent;
		}
		PNode ppNode = pParent->_pParent;
		SubR->_pLeft = pParent;
		pParent->_pParent = SubR;
		if (ppNode == NULL)
		{
			_pRoot = SubR;
			SubR->_pParent = NULL;
		}
		else
		{
			if (ppNode->_pLeft == pParent)
			{
				ppNode->_pLeft = SubR;
			}
			else
			{
				ppNode->_pRight = SubR;
			}
			SubR->_pParent = ppNode;
		}
		pParent->_bf = SubR->_bf = 0;
	}
	void _RotateR(PNode pParent)
	{
		PNode SubL = pParent->_pLeft;
		PNode SubLR = SubL->_pRight;
		pParent->_pLeft = SubLR;
		if (SubLR)
		{
			SubLR->_pParent = pParent;
		}
		PNode ppNode = pParent->_pParent;
		SubL->_pRight = pParent;
		pParent->_pParent = SubL;
		if (ppNode == NULL)
		{
			_pRoot = SubL;
			SubL->_pParent = NULL;
		}
		else
		{
			if (ppNode->_pLeft == pParent)
			{
				ppNode->_pLeft = SubL;
				SubL->_pParent = ppNode;
			}
			else
			{
				ppNode->_pRight = SubL;
				SubL->_pParent = ppNode;
			}
		}
		pParent->_bf = SubL->_bf = 0;
	}
	void _RotateLR(PNode pParent)
	{
		PNode SubL = pParent->_pLeft;
		PNode SubLR = SubL->_pRight;
		int bf = SubLR->_bf;
		_RotateL(SubL);
		_RotateR(pParent);
		if (bf == 0)//如果bf是0等于插入结点就是SubLR,这个时候所有旋转的bf都是0
		{
			SubL->_bf = pParent->_bf = 0;
		}
		else if (bf == -1)//在 bf的左子树树插入新结点
		{
			SubL->_bf = -1;
			pParent->_bf = 0;
		}
		else//新结点的右子树树插入结点
		{
			SubL->_bf = 0;
			pParent->_bf = -1;
		}
		SubLR->_bf = 0;
	}
	void _RotateRL(PNode pParent)
	{
		PNode SubR = pParent->_pRight;
		PNode SubRL = SubR->_pLeft;
		int bf = SubRL->_bf;
		_RotateR(SubR);
		_RotateL(pParent);
		if (bf == 0)//如果bf是0等于插入结点就是SubLR,这个时候所有旋转的bf都是0
		{
			SubR->_bf = pParent->_bf = 0;
		}
		else if (bf == -1)//在 bf的左子树树插入新结点
		{
			SubR->_bf = 0;
			pParent->_bf = -1;
		}
		else//新结点的右子树树插入结点
		{
			SubR->_bf = -1;
			pParent->_bf = 0;
		}
		SubRL->_bf = 0;
	}
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	bool IsBalance_one()//判断是否为平衡二叉树
	{
		return _IsBalance_one(_pRoot);
	}
	bool IsBalance_two()
	{
		int depth;
		return _IsBalance_two(_pRoot, depth);
	}
	int Height()//求树的高度
	{
		return _Height(_pRoot);
	}
private:
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	bool _IsBalance_one(PNode pRoot)
	{
		if (pRoot == NULL)
		{
			return true;
		}
		int height = abs(_Height(pRoot->_pRight) - _Height(pRoot->_pLeft));
		return (height < 2) && _IsBalance_one(pRoot->_pLeft) && _IsBalance_one(pRoot->_pRight);
	}
	bool _IsBalance_two(PNode pRoot, int &depth)//自下向上的方法
	{
		if (NULL == pRoot)
		{
			depth = 0;
			return true;
		}
		int leftDepth, rightDepth;
		if (_IsBalance_two(pRoot->_pLeft, leftDepth) == false)
		{
			return false;
		}
		if (_IsBalance_two(pRoot->_pRight, rightDepth) == false)
		{
			return false;
		}
		if (rightDepth - leftDepth != pRoot->_bf)
		{
			cout << "bf异常" << pRoot->_bf << endl;
		}
		depth = leftDepth > rightDepth ? (leftDepth + 1) : (rightDepth + 1);
		return true;
	}
	int _Height(PNode pRoot)
	{
		if (NULL == pRoot)
		{
			return 0;
		}
		int lheight = _Height(pRoot->_pLeft) + 1;//求出左子树的高度
		int rheight = _Height(pRoot->_pRight) + 1;//求出右子树的高度
		return lheight > rheight ? lheight : rheight;
	}

};
int main()
{
	int a[] = { 18, 14, 20, 12, 16, 15 };
	AVLBinTree< int, int> bst;
	for (int i = 0; i < sizeof(a) / sizeof(*a); ++i)
	{
		bst.Insert(a[i], i);
	}
	bst.InOrder();
	cout << bst.Height();
	cout << bst.IsBalance_one();
	cout << bst.IsBalance_two();
	system("pause");
	return 0;
}