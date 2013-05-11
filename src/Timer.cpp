#include "Timer.h"

using namespace std;

Timer::Timer() {
    QueryPerformanceFrequency(&qpFrequency);
    previous.QuadPart = 0;
    previousClock = 0;
}
void Timer::start() {
    QueryPerformanceCounter(&startQp);
    startClock = clock();
}
void Timer::stop() {
    QueryPerformanceCounter(&stopQp);
    stopClock = clock();
   
    previous.QuadPart += (stopQp.QuadPart - startQp.QuadPart);
    previousClock += stopClock - startClock;
}
void Timer::reset() {
    previous.QuadPart = 0;
    previousClock = 0;
}
double Timer::getTime() {
    return previous.QuadPart/(double(qpFrequency.QuadPart)/1000);
}
long Timer::getClockTime() {
    return previousClock;
}


