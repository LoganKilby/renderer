/* date = June 27th 2021 8:32 pm */

#ifndef TYPES_H
#define TYPES_H

#define internal static 
#define local_persist static 
#define global_variable static

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#if ASSERTIONS_ENABLED
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#define AssertMsgBreak(Msg) {fprintf(stderr, Msg); Assert(0);}
#define AssertMsgBreakEx(Expression, Msg, MsgParam) \
{if(!(Expression)){fprintf(stderr, Msg, MsgParam); Assert(0);}}
#else
#define Assert(Expression)
#define AssertMsgBreak(Expression, Msg)
#define AssertMsgBreakEx(Msg, MsgParam)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define OffsetOf offsetof

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;

typedef float real32;
typedef double real64;

#endif //TYPES_H
