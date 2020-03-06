#ifndef __BINARY_SEARCH_TREE__H__
#define __BINARY_SEARCH_TREE__H__

#include "../binaryTree.h"
template <typename T>
class CBSTree : public CBTree<T>
{
private:
	CBTNode<T> *Find(const T &data, CBTNode<T> *p) const;
	CBTNode<T> *FindMin(CBTNode<T> *p) const;
	CBTNode<T> *FindMax(CBTNode<T> *p) const;
	CBTNode<T> *Insert(const T &data, CBTNode<T> *p);
	CBTNode<T> *Delete(const T &data, CBTNode<T> *p);

public:
	CBTNode<T> *Find(const T &data) const;
	CBTNode<T> *FindMin() const;
	CBTNode<T> *FindMax() const;
	CBTNode<T> *Insert(const T &data);
	CBTNode<T> *Delete(const T &data);
};

template <typename T>
inline CBTNode<T>* CBSTree<T>::Find(const T &data) const
{
	return Find(data, m_pNodeRoot);
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::Find(const T &data, CBTNode<T> *p) const
{
	if (NULL == p){
		return NULL;
	}
	if (data < p->data){
		return Find(data, p->left);
	}else if (data > p->data){
		return Find(data, p->right);
	}else{
		return p;
	}
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::FindMin() const
{
	return FindMin(m_pNodeRoot);
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::FindMin(CBTNode<T> *p) const
{
	if (NULL == p){
		return NULL;
	}else if(NULL == p->left){
		return p;
	}else {
		return FindMin(p->left);
	}
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::FindMax() const
{
	return FindMax(m_pNodeRoot);
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::FindMax(CBTNode<T> *p) const
{
	if (NULL == p){
		return NULL;
	}else if(NULL == p->right){
		return p;
	}else {
		return FindMin(p->right);
	}
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::Insert(const T &data)
{
	return Insert(data, m_pNodeRoot);
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::Insert(const T &data, CBTNode<T> *p)
{
	if (NULL == p){
		p = new CBTNode<T>;
		if (NULL == p){
			return NULL;
		}else{
			p->data = data;
			p->left = NULL;
			p->right = NULL;
			if (NULL == m_pNodeRoot){
				m_pNodeRoot = p;
				m_pNodeRoot->parent = NULL;
			}
		}
	}else if (data < p->data){
		p->left = Insert(data, p->left);
		if (p->left){
			p->left->parent = p;
		}
	}else if (data > p->data){
		p->right = Insert(data, p->right);
		if (p->right){
			p->right->parent = p;
		}
	}

	return p;
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::Delete(const T &data)
{
	return Delete(data, m_pNodeRoot);
}

template <typename T>
inline CBTNode<T>* CBSTree<T>::Delete(const T &data, CBTNode<T> *p)
{
	if (NULL == p){
		printf("Tree has no data\n");
	}else if (data < p->left){
		p->left = Delete(data, p->left);
	}else if (data > p->data){
		p->right = Delete(data, p->right);
	}else if (p->left && p->right) //find it and it has two children
	{
		CBTNode<T> *pTmp = FindMin(p->right);
		p->data = pTmp->data;
		p->right = Delete(p->data, p->right);
	}else{	//find it, and it has one or zero children
		CBTNode<T> *pTmp = p;
		if (NULL == p->left){
			p = p->right;
		}else if (NULL == p->right){
			p = p->left;
		}

		if (p){
			p->parent = pTmp->parent;
		}
		if (m_pNodeRoot == pTmp){
			m_pNodeRoot = p;
		}
		
		return pTmp;
	}
}
#endif //__BINARY_SEARCH_TREE__H__
