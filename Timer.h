/*
 * Timer.h
 *
 *  Created on: 11/ago/2012
 *      Author: stefano
 */

#ifndef __TIMER_H
#define __TIMER_H

#include "SupportDefs.h"

#include <map>
#include <string>

enum timer_type {
	TIMER_GLOBAL = 0
};


class Timer {
public:
	typedef uint32 (*timer_func)(uint32 interval, void* parameter);

	static bool Initialize();
	static Timer* Set(const char* name, uint32 delay);
	static Timer* Get(const char* name);

	bool Expired();
	void Rearm();

	static void Wait(uint32 delay);
	static void AddOneShotTimer(uint32 time, timer_func function, void* parameter);
	static uint32 Ticks();


private:
	Timer(uint32 delay);

	uint32 fDelay;
	uint32 fExpirationTime;

	typedef std::map<std::string, Timer*> timer_map;
	static timer_map sTimers;
};



#endif /* __TIMER_H */
