#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

void threadFun(int n)
{
	std::cout << "thread: " << n << std::endl;
}
int main()
{
	/*
	std::thread threads[5];

	std::cout<<"creates 5 threads" << std::endl;

	for (int i = 0; i< 5; i++){
		threads[i] = std::thread(threadFun, i+1);
	}

	for (auto &t : threads){
		t.join();
	}

	std::cout<<"all threads joind" << std::endl;
*/

	std::thread t(threadFun, 5);
	t.join();
	return 0;
}
