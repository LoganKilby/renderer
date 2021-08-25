/* date = May 22nd 2021 9:47 pm */

#ifndef QPC_H
#define QPC_H

#include "windows.h"

// Basic usage:
//   QPC_StartCounter();
//   long long Result = QPC_EndCounter();
//
//   printf("%lld\n", Result); print microseconds
//   printf("%Lf\n", Result / 1000.0l); print milliseconds

// Simple LIFO: EndCounter returns the time elapsed since the most recent call to StartCounter

#define MAX_COUNTER_CALLS 50
// MAX_COUNTER_CALLS defines how many StartCounter calls can be stored at any given time.
// You may change MAX_COUNTER_CALLS to any value you like.

void QPC_StartCounter(void);
long long QPC_EndCounter(void); // Returns microseconds since last StartCounter
void QPC_EndCounterPrint(char *Message); // Prints Your message followed by the time elapsed in ms

// ------------------------------------------------------------------------------------------------

struct qpc_counter
{
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER Frequency;
};

qpc_counter QPC_CounterQueue[MAX_COUNTER_CALLS] = {};
int QPC_CounterCount = 0;

void QPC_AddCounter(qpc_counter Counter)
{
    if(QPC_CounterCount < MAX_COUNTER_CALLS)
    {
        QPC_CounterQueue[QPC_CounterCount] = Counter;
        QPC_CounterCount++;
    }
}

void QPC_StartCounter(void)
{
    qpc_counter NewCounter = {};
    QueryPerformanceFrequency(&NewCounter.Frequency);
    QueryPerformanceCounter(&NewCounter.StartingTime);
    QPC_AddCounter(NewCounter);
}

long long QPC_EndCounter()
{
    long long Result = 0;
    
    if(QPC_CounterCount)
    {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        
        qpc_counter Counter = QPC_CounterQueue[QPC_CounterCount - 1];
        QPC_CounterQueue[QPC_CounterCount - 1] = {};
        QPC_CounterCount--;
        
        LARGE_INTEGER ElapsedMicroseconds = {};
        ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - Counter.StartingTime.QuadPart;
        Result = (ElapsedMicroseconds.QuadPart * 1000000) / Counter.Frequency.QuadPart;
    }
    
    return Result;
}

void QPC_EndCounterPrint(char *Message)
{
    if(QPC_CounterCount)
    {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        
        qpc_counter Counter = QPC_CounterQueue[QPC_CounterCount - 1];
        QPC_CounterQueue[QPC_CounterCount - 1] = {};
        QPC_CounterCount--;
        
        LARGE_INTEGER ElapsedMicroseconds = {};
        ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - Counter.StartingTime.QuadPart;
        long long Result = (ElapsedMicroseconds.QuadPart * 1000000) / Counter.Frequency.QuadPart;
        printf("%s %Lf ms\n", Message, Result / 1000.0l);
    }
    else
    {
        printf("QPC: No counter data to print.\n");
    }
}

#endif //QPC_H
