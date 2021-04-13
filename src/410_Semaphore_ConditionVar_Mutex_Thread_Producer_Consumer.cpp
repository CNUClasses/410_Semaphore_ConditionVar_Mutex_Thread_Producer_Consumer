//============================================================================
// Name        : 410_Semaphore_ConditionVar_Mutex_Thread_Producer_Consumer.cpp
// Author      : 
// Version     :
// Copyright   : Steal this code!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include "Semaphore.h"

using namespace std;

bool bDone = false;	//used by producer to indicate we are done
int gCount 	= 0;

Semaphore 	s_cnt(3);		//1=how many the producer can produce at a time before it waits
mutex m;					//mutual exclusion
Semaphore 	s_binary(0);	//binary semaphore, 0=blocked, 1 or more = go ahead,start off signaled, consumer will have to wait until producer produces

void producer(int i) {
	for (int j = 0; j < i; j++) {
		s_cnt.wait();
		{
			lock_guard<mutex> lck(m);
			gCount++;
			cout << "Producer gCount=" << gCount << endl;
		}
		s_binary.signal();
	}

	//tell consumer we are done
	lock_guard<mutex> lck(m);
	bDone = true;
	cout << "Producer DONE!!" << endl;
}

void consumer(int id) {
	while(true) {
		{
			s_binary.wait();
			//need to consume?
			{
				lock_guard<mutex> lck(m);
				if(gCount >0){
					gCount--;
					cout << "               Consumer " <<id<<"; gCount=" << gCount << endl;
					s_cnt.signal();

				}
			}

			{
				lock_guard<mutex> lck(m);
				if(bDone==true && gCount ==0)
					break;	//here so we can properly signal those below
			}
		}

	}
	cout << "  Consumer: " << id << " exiting" << endl;
}
int main()
{
	cout << "The initial value of gCount is " << gCount << endl; //

	thread t_producer(producer, 10000);
	thread t_consumer1(consumer,1);
	thread t_consumer2(consumer,2);
	thread t_consumer3(consumer,3);

	t_producer.join();
	t_consumer1.join();
	t_consumer2.join();
	t_consumer3.join();
	
	cout <<endl<< "The final value of gCount is " << gCount << endl; //

    return 0;
}
