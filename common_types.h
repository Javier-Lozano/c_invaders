#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#include <stdbool.h>

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef   signed char      i8;
typedef   signed short     i16;
typedef   signed int       i32;
typedef   signed long long i64;

typedef float  f32;
typedef double f64;

typedef struct game_context_st GameContext;

typedef void (*FuncScene)(GameContext *);


#endif // COMMON_TYPES_H_

