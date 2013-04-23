#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <sys/time.h>
 
class Timer {
    public:
        Timer();
        void start();
        void stop();
        double getTime();
        long getClockTime();
    private:
        clock_t startClock;
        clock_t stopClock;
        long previousClock;
    
        LARGE_INTEGER startQp;
        LARGE_INTEGER previous;
        LARGE_INTEGER stopQp;
        LARGE_INTEGER qpFrequency;
};
 
#endif
