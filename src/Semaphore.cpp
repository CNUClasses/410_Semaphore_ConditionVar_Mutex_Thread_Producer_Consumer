/*
 * Semaphore.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: keith
 */

#include "Semaphore.h"
using namespace std;

Semaphore::Semaphore(int cnt) :
		count(cnt) {
}
Semaphore::~Semaphore() {
}

void Semaphore::wait() {
	unique_lock<mutex> mlk(m);
	while(count == 0)
		cv.wait(mlk);
	--count;
}
void Semaphore::signal() {
	unique_lock<mutex> mlk(m);
	++count;
    cv.notify_one();	
}

