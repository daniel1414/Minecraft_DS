#ifndef _TIMER_H_
#define _TIMER_H_

#include <nds/timers.h>
#include "log.h"

class Timer
{
public:
    Timer(const char* function_name, u8 divider = TIMER_DIV_256, u8 timer = 3) : m_function_name(function_name), m_timer(timer)
    {
        TIMER_CR(timer) = TIMER_ENABLE | divider;
        m_start_ticks = timerTick(m_timer);
    }
    ~Timer()
    {
        u16 elapsed_ticks = timerTick(m_timer) - m_start_ticks;
        LOG("%s - ticks elapsed: %d", m_function_name, elapsed_ticks);
    }
private:
    const char* m_function_name;
    u8 m_timer;
    u16 m_start_ticks;
};

#endif