/*
    Pagalbinë programëlë parodanti (árodanti?) kuris ið bûdø (gettimeofday, clock ar QueryPerformanceCounter)
    yra tiksliausias t.y. labiausiai tinkamas kodo vykdymui sugaiðtam laikui matuoti
*/

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <windows.h>

#define NUMBER 30

int main (void) {
    //------------------------- get time of day part ----------------------
    struct timeval tv[NUMBER];
    int count[NUMBER], i, diff;

    gettimeofday (&tv[0], NULL);

    for (i = 1; i < NUMBER; i++) {
        gettimeofday (&tv[i], NULL);
        count[i] = 1;
        while ((tv[i].tv_sec == tv[i-1].tv_sec) &&
            (tv[i].tv_usec == tv[i-1].tv_usec))
        {
            gettimeofday (&tv[i], NULL);
            count[i]++;
        }
    }

    printf ("%2d: secs = %d, usecs = %6d\n", 0, tv[0].tv_sec, tv[0].tv_usec);
    for (i = 1; i < NUMBER; i++) {
        diff = (tv[i].tv_sec - tv[i-1].tv_sec) * 1000000;
        diff += tv[i].tv_usec - tv[i-1].tv_usec;

        printf ("%2d: secs = %d, usecs = %6d, count = %5d, diff = %d microseconds\n",
            i, tv[i].tv_sec, tv[i].tv_usec, count[i], diff);
    }
    
    //------------------------ clock part --------------------------------------
    clock_t cv[NUMBER];
    //int count[NUMBER], i, diff;
    std::cout << "\n\nCLOCKS_PER_SEC:" << CLOCKS_PER_SEC << "\n";
    cv[0] = clock();

    for (i = 1; i < NUMBER; i++) {
        cv[i] = clock();
        count[i] = 1;
        while ((cv[i] == cv[i-1]))
        {
            cv[i] = clock();
            count[i]++;
        }
    }

    printf ("%2d: clicks = %d, \n", 0, cv[0]);
    for (i = 1; i < NUMBER; i++) {
        diff = (cv[i] - cv[i-1]) * 1000;
        
        printf ("%2d: clicks = %d, count = %5d, diff=%d microseconds\n",
            i, cv[i], count[i], diff);
    }
    
    //-------------------------- query performance ---------------------------------
    std::cout << "\nQuery performance \n";
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
    LARGE_INTEGER frequency;
    
    QueryPerformanceFrequency(&frequency);
    
    
    LARGE_INTEGER qpv[NUMBER];
    //int count[NUMBER], i, diff;
    double diff2;
    QueryPerformanceCounter(&qpv[0]);

    for (i = 1; i < NUMBER; i++) {
        QueryPerformanceCounter(&qpv[i]);
        count[i] = 1;
        while ((qpv[i].QuadPart == qpv[i-1].QuadPart))
        {
            QueryPerformanceCounter(&qpv[i]);
            count[i]++;
        }
    }

    //printf ("%2d: counter value = %d, \n", 0, qpv[0]);
    for (i = 1; i < NUMBER; i++) {
        diff2 = (qpv[i].QuadPart - qpv[i-1].QuadPart)/(double(frequency.QuadPart)/1000000);
        
        printf ("%2d: count = %d, diff=%.12f microseconds\n",
            i, count[i], diff2);
    }
    
    // QueryPerformanceCounter(&start);
    // ::QueryPerformanceCounter(&stop);
    // std::cout << "QueryPerformance: " << ((stop.QuadPart - start.QuadPart)) << "; Frequency: " << frequency.QuadPart << "\n";
    // std::cout << "QP: " << double(stop.QuadPart-start.QuadPart)/(double(frequency.QuadPart)/1000) << "\n";
    
    
    int a;
    std::cin >> a;
    return 0;
}
