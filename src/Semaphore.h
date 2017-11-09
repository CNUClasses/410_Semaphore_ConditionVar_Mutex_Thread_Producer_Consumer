/*
 * Semaphore.h
 *
 *  Created on: Nov 8, 2017
 *      Author: keith
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	Semaphore(int cnt=1);
	virtual ~Semaphore();

	void wait();
	void signal();
	
private:
	volatile int count;
	std::mutex m;
	std::condition_variable cv;
};

#endif /* SEMAPHORE_H_ */
