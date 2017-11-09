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

int gCount 	= 0;
Semaphore 	s_cnt(1);		//1=how many the producer can produce at a time before it waits
Semaphore 	s_m(1);			//mutual exclusion
Semaphore 	s_binary(0);	//binary semaphore, 0=blocked, 1 = go ahead,start off signaled, consumer will have to wait until producer produces

void producer(int i) {
	for (int j = 0; j < i; j++) {
		s_cnt.wait();	//controls how many we produce(counts down1)
		s_m.wait();		//lock

		//produce something
		gCount++;
		
		cout << "Producer gCount=" << gCount << endl;

		s_m.signal();		//unlock
		s_binary.signal();	//tell consumer to consume
	}
}


void consumer(int i) {

	for (int j = 0; j < i; j++) {
		s_binary.wait();	//wait for ready from producer		
		s_m.wait();			//lock

		//consume something
		gCount--;
		cout << "               Consumer gCount=" << gCount << endl;

		s_m.signal();	//unlock
		s_cnt.signal();		//count up 1
	}
}
int main()
{
	cout << "The initial value of gCount is " << gCount << endl; //

	thread t_consumer(consumer, 100);
	thread t_producer(producer, 100);

	t_producer.join();
	t_consumer.join();
	
	cout <<endl<< "The final value of gCount is " << gCount << endl; //

    return 0;
}
