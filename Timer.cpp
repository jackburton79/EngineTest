/*
 * Timer.cpp
 *
 *  Created on: 11/ago/2012
 *      Author: stefano
 */


#include "Timer.h"

#include <sys/time.h>

#include <SDL.h>


Timer::timer_map Timer::sTimers;

Timer::Timer(uint32 delay)
	:
	fDelay(delay),
	fExpirationTime(Ticks() + delay)
{
}


/* static */
bool
Timer::Initialize()
{
	if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0)
		return false;
	return true;
}


/* static */
Timer*
Timer::Set(const char* name, uint32 delay)
{
	sTimers[name] = new Timer(delay);
	return sTimers[name];
}


/*static */
Timer*
Timer::Get(const char* name)
{
	timer_map::iterator i = sTimers.find(name);
	if (i == sTimers.end())
		return NULL;
	return i->second;
}


bool
Timer::Expired()
{
	return Ticks() >= fExpirationTime;
}


void
Timer::Rearm()
{
	fExpirationTime = Ticks() + fDelay;
}


/* static */
void
Timer::AddOneShotTimer(uint32 interval, timer_func func, void* parameter)
{
	SDL_AddTimer(interval, func, parameter);
}


/* static */
uint32
Timer::Ticks()
{
	return SDL_GetTicks();
}


/* static */
void
Timer::Wait(uint32 delay)
{
	SDL_Delay(delay);
}

