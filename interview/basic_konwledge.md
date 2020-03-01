# new/delete malloc/free区别
* 作用都是用于在堆上手动申请和释放内存
* malloc/free是c的标准库函数，而new/delete是c++中的运算符
* new会调用对象的构造函数，delete会调用对象的析构函数
* malloc有一簇函数 如malloc realloc calloc等应对不同需求，需要包含头文件malloc.h，而new直接调用就可以了
* new返回指定类型指针并且可以自动计算需要大小，而malloc函数原型返回的是void类型指针，需要我们计算需要大小并且手动转换成需要的类型
* 所以在面向过程的函数情景中，一般多用malloc 在涉及对象情景下，用new

# 
