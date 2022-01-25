/* date = November 12th 2021 9:24 am */

#ifndef MACROS_H
#define MACROS_H

struct block_timer
{
    const char* function_name;
    LARGE_INTEGER timer_start;
    LARGE_INTEGER frequency;
	
	block_timer(const char *caller)
	{
        function_name = caller;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&timer_start);
    }
    
	~block_timer()
	{
		LARGE_INTEGER timer_end;
        QueryPerformanceCounter(&timer_end);
        timer_end.QuadPart = timer_end.QuadPart - timer_start.QuadPart;
        timer_end.QuadPart *= 1000000;
        timer_end.QuadPart /= frequency.QuadPart;
        
        fprintf(stderr, "%s: %.3Lf ms\n", function_name, timer_end.QuadPart / 1000.0l);
    }
};

#define TIMED_BLOCK block_timer timer##__LINE__(__FUNCTION__);

#endif //MACROS_H
