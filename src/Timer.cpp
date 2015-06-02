#include "Timer.hpp"

namespace FLAT
{

  Timer::Timer()
        : _is_running(false),
          _at_zero(true),
      _elapsed_milliseconds(0)
    {
    }

    void Timer::start(const std::string& label)
    {
        _label = label;
        start();
    }

    std::ostream & operator << (std::ostream & lhs, Timer& rhs)
    {
        if (rhs._is_running == true)
        {
#ifdef WIN32
          rhs._elapsed_milliseconds += GetTickCount() - rhs._start;
#else
            gettimeofday(& rhs._end, NULL);
            rhs._elapsed_milliseconds += (rhs._end.tv_sec * 1000 + rhs._end.tv_usec / 1000)
                - (rhs._start.tv_sec * 1000 + rhs._start.tv_usec / 1000);
#endif
        }
        if (rhs._label != "")
        {
            lhs<< rhs._label << ": "
                << (rhs._elapsed_milliseconds+0.0)/(1000.0);
        }
        else
        {
            lhs << (rhs._elapsed_milliseconds+0.0)/(1000.0);
        }
        return lhs;
    }
    void Timer::add(Timer& timer)
    {
      _elapsed_milliseconds += timer._elapsed_milliseconds;
    }

    void Timer::reset()
    {
        _at_zero = true;
        _is_running = false;
        _label = "";
        _elapsed_milliseconds = 0;
    }

    Timer& Timer::operator  =(const Timer &rhs)
    {
    _is_running = rhs._is_running;
    _at_zero = rhs._at_zero;
    _label = rhs._label;
  #ifdef WIN32
    _start = rhs._start;
    _elapsed_milliseconds = rhs._elapsed_milliseconds;
  #else
    _start = rhs._start;
    _end = rhs._end;
    _elapsed_milliseconds = rhs._elapsed_milliseconds;
  #endif
    return *this;
    }

}
