#ifndef __STACK__H__
#define __STACK__H__


#include "../List/list.h"
template <typename T>
class CStack : public CSList<T>
{
public:
	int push(T data);
	int pop(T *data = NULL);
	int top(T *data) const;
};

template <typename T>
inline int CStack<T>::push(T data)
{
	return this->AddTail(data);
}

template <typename T>
inline int CStack<T>::pop(T *data)
{
	if (this->IsEmpty()){
		return 0;
	}

	if (data){
		top(data);
	}

	this->RemoveTail();
	return 1;
}

template <typename T>
inline int CStack<T>::top(T *data) const
{
	assert(data);
	if (this->IsEmpty()){
		return 0;
	}
	*data = this->GetTail();
	return 1;
}

#endif //__STACK__H__
