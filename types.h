/* date = June 27th 2021 8:32 pm */

#ifndef TYPES_H
#define TYPES_H

#define local_persist static 
#define global_variable static

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#if ASSERTIONS_ENABLED
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define OffsetOf offsetof
#define ConvertToMegabytes(Bytes) (f32)Bytes / 1000000.0f

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t b32;

typedef float f32;
typedef double f64;

#endif //TYPES_H
