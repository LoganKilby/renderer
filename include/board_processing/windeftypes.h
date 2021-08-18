/* date = August 16th 2021 10:52 am */

#ifndef WINDEFTYPES_H
#define WINDEFTYPES_H

typedef unsigned char BYTE;
typedef long LONG;

#ifdef UNICODE
typedef WCHAR TCHAR;
#else
typedef char TCHAR;
#endif

typedef short VARIANT_BOOL;
typedef int BOOL;
typedef double DATE;
#endif //WINDEFTYPES_H
