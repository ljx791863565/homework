#ifndef __LIST__H__
#define __LIST__H__

#include <iostream>
#include <stdlib.h>
#include <assert.h>
template <typename T>
class CNode 
{
public:
	CNode()	:data(0), next(NULL){}
	
	CNode(const T &initdata) : data(initdata),	next(NULL){}

	CNode(const T &initdata, CNode<T> *p) :	data(initdata), next(p){}

	~CNode()
	{
		data = 0;
		delete next;
	}
public:
	T data;		//数据域
	CNode<T> *next;	//指针域
};

template <typename T>
class CSList
{
public:
	CSList();
	CSList(const T &initdata);
	~CSList();

public:
	int		IsEmpty() const;
	int		GetCount() const;
	int		InsertBefore(const int pos, const T data);
	int		InsertAfter(const int pos, const T data);
	int		AddHead(const T data);
	int		AddTail(const T data);
	void	RemoveAt(const int pos);
	void	RemoveHead();
	void	RemoveTail();
	void	RemoveAll();
	T&		GetTail();
	T		GetTail() const;
	T&		GetHead();
	T		GetHead() const;
	T&		GetAt(const int pos);
	T		GetAt(const int pos) const;
	void	SetAt(const int pos, T data);
	int		Find(const T data) const;

protected:
	int m_nCount;	//节点个数
	CNode<T> *m_pNodeHead;
};



template <typename T>
inline CSList<T>::CSList()	: m_nCount(0), m_pNodeHead(NULL){}

template <typename T>
inline CSList<T>::CSList(const T& initdata)	 :	m_nCount(0), m_pNodeHead(NULL)
{
	AddHead(initdata);
}

template <typename T>
inline CSList<T>::~CSList()
{
	RemoveAll();
}

template <typename T>
inline int CSList<T>::IsEmpty() const
{	
	return 0 == m_nCount;
}

template <typename T>
inline int CSList<T>::AddHead(const T data)
{
	CNode<T> *pNewNode;
	pNewNode = new CNode<T>;
	if (NULL == pNewNode){
		return 0;
	}
	pNewNode->data = data;
	pNewNode->next = m_pNodeHead;

	m_pNodeHead = pNewNode;
	++m_nCount;

	return 1;
}

template <typename T>
inline int CSList<T>::AddTail(const T data)
{
	return InsertAfter(GetCount(), data);
}

template <typename T>
inline int CSList<T>::InsertBefore(const int pos, const T data)
{
	int i;
	int nRetPos;
	CNode<T> *pTmpNode1;
	CNode<T> *pTmpNode2;
	CNode<T> *pNewNode;

	pNewNode = new CNode<T>;
	if (NULL == pNewNode){
		nRetPos = 0;
		goto Exit0;
	}

	pNewNode->data = data;

	if (NULL == m_pNodeHead){
		pNewNode->next = NULL;
		m_pNodeHead = pNewNode;
		nRetPos = 1;
		goto Exit1;
	}

	assert(1 <= pos && pos <= m_nCount);

	if (1 == pos){
		pNewNode->next = m_pNodeHead;
		m_pNodeHead = pNewNode;
		nRetPos = 1;
		goto Exit1;
	}

	pTmpNode1 = m_pNodeHead;
	for (i=1; i<pos; ++i){
		pTmpNode2 = pTmpNode1;
		pTmpNode1 = pTmpNode1->next;
	}
	pNewNode->next = pTmpNode1;
	pTmpNode2->next = pNewNode;
	nRetPos = pos;

Exit1:
	++m_nCount;
Exit0:
	return nRetPos;
}

template <typename T>
inline int CSList<T>::InsertAfter(const int pos, const T data)
{
	int i, nRetPos;
	CNode<T> *pTmpNode;
	CNode<T> *pNewNode = new CNode<T>;
	if (NULL == pNewNode){
		nRetPos = 0;
		goto Exit0;
	}
	pNewNode->data = data;

	if (NULL == m_pNodeHead){
		pNewNode->next = NULL;
		m_pNodeHead = pNewNode;
		nRetPos = 1;
		goto Exit1;
	}
	assert(1 <= pos && pos <= m_nCount);

	pTmpNode = m_pNodeHead;
	for (i=1; i<pos; ++i){
		pTmpNode = pTmpNode->next;
	}
	pNewNode->next = pTmpNode->next;//pNewNode->next = NULL;
	pTmpNode->next = pNewNode;
	nRetPos = pos + 1;
	
Exit1:
	++m_nCount;
Exit0:
	return nRetPos;	//nRetPos = 0;
}

template <typename T>
inline int CSList<T>::GetCount() const
{
	return m_nCount;
}

template <typename T>
inline void CSList<T>::RemoveAt(const int pos)
{
	assert(1 <= pos && pos <= m_nCount);

	int i;
	CNode<T> *pTmpNode1, *pTmpNode2;
	pTmpNode1 = m_pNodeHead;

	if (1 == pos){
		m_pNodeHead = m_pNodeHead->next;
		goto Exit1;
	}
	for (i=1; i<pos; ++i){
		pTmpNode2 = pTmpNode1;
		pTmpNode1 = pTmpNode1->next;
	}
	pTmpNode2->next = pTmpNode1->next;

Exit1:
	delete pTmpNode1;
	--m_nCount;
}

template <typename T>
inline void CSList<T>::RemoveTail()
{
	assert(0 != m_nCount);
	RemoveAt(m_nCount);
}

template <typename T>
inline void CSList<T>::RemoveHead()
{
	assert(0 != m_nCount);
	RemoveAt(1);
}

template <typename T>
inline void CSList<T>::RemoveAll()
{
	int i, nCount;
	CNode<T> *pTmpNode;

	nCount = m_nCount;
	for (i=0; i<nCount; ++i){
		pTmpNode = m_pNodeHead->next;
		delete m_pNodeHead;
			m_pNodeHead = pTmpNode;
	}

	m_nCount = 0;
}

template <typename T>
inline T& CSList<T>::GetTail() 
{
	assert(0 != m_nCount);
	int i, nCount;
	CNode<T> *pTmpNode;
	pTmpNode = m_pNodeHead;
	nCount = m_nCount;

	for (i=0; i<nCount; ++i){
		pTmpNode = pTmpNode->next;
	}

	return pTmpNode->data;
}


template <typename T>
inline T CSList<T>::GetTail() const
{
	assert(0 != m_nCount);
	int i, nCount;
	CNode<T> *pTmpNode;
	pTmpNode = m_pNodeHead;
	nCount = m_nCount;

	for (i=0; i<nCount; ++i){
		pTmpNode = pTmpNode->next;
	}

	return pTmpNode->data;
}

template <typename T>
inline T& CSList<T>::GetHead()
{
	assert(0 != m_nCount);
	return m_pNodeHead->data;
}

template <typename T>
inline T CSList<T>::GetHead() const
{
	assert(0 != m_nCount);
	return m_pNodeHead->data;
}

template <typename T>
inline T& CSList<T>::GetAt(const int pos)
{
	assert(1 <= pos && pos <= m_nCount);

	int i;
	CNode<T> *pTmpNode = m_pNodeHead;

	for (i=1; i<pos; ++i){
		pTmpNode = pTmpNode->next;
	}
	return pTmpNode->data;
}

template <typename T>
inline T CSList<T>::GetAt(const int pos) const
{
	assert(1 <= pos && pos <= m_nCount);

	int i;
	CNode<T> *pTmpNode = m_pNodeHead;

	for (i=1; i<pos; ++i){
		pTmpNode = pTmpNode->next;
	}
	return pTmpNode->data;
}

template <typename T>
inline void CSList<T>::SetAt(const int pos, const T data)
{
	assert(1 <= pos && pos <= m_nCount);

	int i;
	int nCount = m_nCount;
	CNode<T> *pTmpNode = m_pNodeHead;

	for (i=1; i<nCount; ++i){
		pTmpNode = pTmpNode->next;
	}
	pTmpNode->data = data;
}

template <typename T>
inline int CSList<T>::Find(const T data) const
{
	int i;
	int nCount = m_nCount;
	CNode<T> *pTmpNode = m_pNodeHead;

	for (i=0; i<nCount; ++i){
		if (data == pTmpNode->data){
			return i+1;
		}
		pTmpNode = pTmpNode->next;
	}
}


#endif //__LIST__H__
