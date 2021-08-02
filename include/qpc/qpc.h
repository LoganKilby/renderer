/* date = May 22nd 2021 9:47 pm */

#ifndef QPC_H
#define QPC_H

//#include "windows.h"
//#include "profileapi.h"

// Basic usage:
//   StartCounter();
//   long long Result = EndCounter(0);
// or,
//   long long Result;
//   EndCounter(&Result);
//
//   printf("%lld\n", Result);
//   printf("%lld\n", EndCounter(0));
//   printf("%Lf\n", EndCounter(0) / 1000.0l); milliseconds

// Multiple calls can be made to StartCounter before EndCounter. 

#define MAX_COUNTER_CALLS 50
// MAX_COUNTER_CALLS defines how many StartCounter calls can be stored at any given time.
// You may change MAX_COUNTER_CALLS to any value you like.

// Starts a timer. To end a timer, call EndCounter
void QPC_StartCounter(void);

// Returns time in microseconds since the last time you called StartCounter. 
long long QPC_EndCounter(long long *Dest);

struct qpc_counter
{
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER Frequency;
    int Active;
    int Index;
};

qpc_counter QPC_CounterQueue[MAX_COUNTER_CALLS] = {};
int QPC_CounterCount = 0;

static void UpdateCounterQueue()
{
    if(!QPC_CounterCount)
    {
        return;
    }
    
    QPC_CounterQueue[0].Active = 0;
    QPC_CounterCount--;
    
    int SwapCount = 0;
    for(int i = 0; i < MAX_COUNTER_CALLS; i++)
    {
        if(SwapCount == QPC_CounterCount)
        {
            break;
        }
        else
        {
            if(!QPC_CounterQueue[i].Active)
            {
                QPC_CounterQueue[i] = QPC_CounterQueue[i + 1];
                QPC_CounterQueue[i + 1] = {};
                SwapCount++;
            }
        }
    }
}

static void QPC_AddCounter(qpc_counter Counter)
{
    QPC_CounterQueue[QPC_CounterCount] = Counter;
    QPC_CounterQueue[QPC_CounterCount].Index = QPC_CounterCount;
    QPC_CounterCount++;
}

void QPC_StartCounter(void)
{
    qpc_counter NewCounter = {};
    QueryPerformanceFrequency(&NewCounter.Frequency);
    QueryPerformanceCounter(&NewCounter.StartingTime);
    NewCounter.Active = 1;
    QPC_AddCounter(NewCounter);
}

long long QPC_EndCounter(long long *Dest)
{
    long long Result;
    
    if(QPC_CounterCount)
    {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        
        qpc_counter Counter = QPC_CounterQueue[0];
        
        LARGE_INTEGER ElapsedMicroseconds = {};
        ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - Counter.StartingTime.QuadPart;
        Result = (ElapsedMicroseconds.QuadPart * 1000000) / Counter.Frequency.QuadPart;
        
        if(Dest)
        {
            *Dest = Result;
        }
        
        UpdateCounterQueue();
    }
    else
    {
        Result = 0;
    }
    
    return Result;
}

#endif //QPC_H
