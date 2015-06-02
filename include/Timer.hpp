#ifndef TIMER_HPP
#define TIMER_HPP

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
typedef size_t Count;
#endif
#include <iostream>

namespace FLAT
{

	typedef float Millisecond;

	//! Stopwatch to Measure time.
	class Timer
	{
	public:
		Timer();

		void start();

		void start(const std::string& label);

		friend std::ostream & operator << (std::ostream & lhs, Timer& rhs);

		Millisecond stop();

		Timer& 		   operator  =(const Timer &rhs);

		void reset();

		void add(Timer &timer);

		bool    _is_running;
		bool    _at_zero;
		std::string   _label;
	#ifdef WIN32
		DWORD   _start;
		DWORD   _elapsed_milliseconds;
	#else
		timeval _start;
		timeval _end;
		size_t  _elapsed_milliseconds;
	#endif
	};

    inline Millisecond Timer::stop()
    {
#ifdef WIN32
        _elapsed_milliseconds += GetTickCount() - _start;
#else
        gettimeofday(& _end, NULL);
        _elapsed_milliseconds += (_end.tv_sec * 1000 + _end.tv_usec / 1000)
            - (_start.tv_sec * 1000 + _start.tv_usec / 1000);
#endif
        _is_running = false;
        return (Millisecond) _elapsed_milliseconds;
    }

    inline void Timer::start()
    {
        _is_running = true;
#ifdef WIN32
        _start = GetTickCount();
#else
        gettimeofday(& _start, NULL);
#endif
        _at_zero = false;
    }
}
#endif
