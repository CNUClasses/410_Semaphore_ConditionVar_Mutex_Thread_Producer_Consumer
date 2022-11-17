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
#include <atomic>
#include "Semaphore.h"

using namespace std;

bool bDone = false;	//used by producer to indicate we are done
atomic<int> gCount(0);

Semaphore 	s_cnt(3);		//1=how many the producer can produce at a time before it waits
mutex m;
mutex mcout;
condition_variable cv;

void log(string s){
	lock_guard<mutex> lck(mcout);
	cout << s << endl;
}
	
void producer(int i) {
	for (int j = 0; j < i; j++) {	
		s_cnt.wait();
		{
			gCount++;
			log("Producer, produced 1, gCount="+ std::to_string( gCount) );
		}
		cv.notify_all();
	}
		
	//tell consumer we are done
	{
		lock_guard<mutex> lck(m);
		bDone = true;
		log("Producer DONE!!");
	}
	cv.notify_all();
}

void consumer(int id) {

	while(true) {
		//wait until there is work
		unique_lock<mutex> lck(m);
		while(gCount==0 && !bDone)
			cv.wait(lck);
		
		//need to consume?
		if(gCount >0){
			gCount--;
			
			log("               Consumer, consumed 1, gCount="+ std::to_string( gCount));
			s_cnt.signal();
		}
			
		if(bDone==true && gCount ==0){
			log("  Consumer:" + std::to_string(id)+" Done");
			break;
		}
	}
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

//Semaphore 	s_binary(0);	//binary semaphore, 0=blocked, 1 or more = go ahead,start off signaled, consumer will have to wait until producer produces

//condition?  (gCount==0 and !bDone)
//consumer-the thread that waits
//unique_lock<mutex> lck(m);
//while(gCount==0 && !bDone)
//	cv.wait(lck);
//:
//producer-this is the thread that controls
//{
//	unique_lock<mutex> lck(m);
//	condition=false;
//}
//	cv.notify_all();
//
