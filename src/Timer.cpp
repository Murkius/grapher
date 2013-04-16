#include "Timer.h"

using namespace std;

Timer::Timer() {
    QueryPerformanceFrequency(&qpFrequency);
}
void Timer::start() {
    QueryPerformanceCounter(&startQp);
}
void Timer::stop() {
    QueryPerformanceCounter(&stopQp);
}
double Timer::getTime() {
    return (stopQp.QuadPart - startQp.QuadPart)/(double(qpFrequency.QuadPart));
}


