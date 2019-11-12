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
		s_cnt.wait();	//controls how many we produce(counts down1)
		{
			lock_guard<mutex> lck(m);		//lock
			gCount++;
			cout << "Producer gCount=" << gCount << endl;
		}

		s_binary.signal();	//tell consumer to consume
	}

	//tell consumer we are done
	lock_guard<mutex> lck(m);
	bDone = true;
	cout << "Producer DONE!!" << endl;
}

void consumer(int id) {
	while(true) {
		s_binary.wait();	//wait for ready from producer
		{
			lock_guard<mutex> lck(m);

			//need to consume?
			if(gCount >0){
				gCount--;
				cout << "               Consumer gCount=" << gCount << endl;
			}

			s_cnt.signal();	//count up 1

			if(bDone==true && gCount ==0)
				break;	//here so we can properly signal those below
		}

	}
	lock_guard<mutex> lck(m);
	cout << "  Consumer: " << id << " exiting" << endl;
}
int main()
{
	cout << "The initial value of gCount is " << gCount << endl; //

	thread t_producer(producer, 10000);
	thread t_consumer1(consumer,1);

	t_producer.join();
	t_consumer1.join();
	
	cout <<endl<< "The final value of gCount is " << gCount << endl; //

    return 0;
}
