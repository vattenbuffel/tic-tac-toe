#ifndef TIMING_H_
#define TIMING_H_

#include <stdint.h>
#include <time.h>

#define TIMING_START clock_t __start__ = clock();

#define TIMING_END_S ((clock() - __start__) / ((double)CLOCKS_PER_SEC) )
#define TIMING_END_MS (TIMING_END_S * 1000.f)
    



#endif /* TIMING_H_ */