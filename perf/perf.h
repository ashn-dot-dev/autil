#ifndef PERF_H_INCLUDED
#define PERF_H_INCLUDED

#include <time.h>

static double
elapsed_sec(clock_t start, clock_t end)
{
    return ((double)end - (double)start) / CLOCKS_PER_SEC;
}

#endif // PERF_H_INCLUDED
