#ifndef MATH_H_
#define MATH_H_

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))
#define ABS(val) \
    ({__typeof__(val) a = (val); \
    (a > 0) ? a : -a;}) 
#endif /* MATH_H_ */
