#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
 
class Timer {
    public:
        Timer();
        void start();
        void stop();
        double getTime();
    private:
        LARGE_INTEGER startQp;
        LARGE_INTEGER stopQp;
        LARGE_INTEGER qpFrequency;
};
 
#endif
