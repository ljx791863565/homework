# 线性结构特点：
* 存在唯一一个被称为"第一个"的数据元素
* 存在唯一一个被称为"最后一个"的数据元素
* 除了第一个元素外 集合的每一个元素都只有一个前驱
* 除了最后一个元素外 集合的每一个元素都只有一个后续

#线性表 linear list 
> n个数据元素的有限序列

{ a1, ..., ai-1, ai, ai+1, ..., an};
称 ai-1 为ai的前驱元素 ai+1 为ai的后继元素 
当n为0时 称表为空表

ADT List{
	数据对象：
	数据关系：
	基本操作：
	InitList(&L){
		//构造一个空的线性表L
	}

	DestoryList(&L){
		//线性变已经存在下 销毁线性表L
	}

	ClearList(&L){
		//线性表L已经存在 将L初始化为空表
	}

	ListEmpty(&L){
		//线性表L已经存在 若L为空表 返回TRUE 否则返回FALSE
	}

	ListLength(L){
		//线性表L已经存在 返回L元素的个数
	}

	GetElem(L, i, &e){
		//L已经存在  1 <= i <= ListLength(L)
		//返回L中第i个元素 保存在e地址中
	}


}ADT List
