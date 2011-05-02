#pragma once

#include "platform.h"

#if !defined(NULL)
#   define NULL 0
#endif

#if defined(OS_POSIX)
#include <stdint.h>
#elif defined(OS_WIN)
#include "stdint.h"
#else
#error Include stdint.h
#endif

typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;

typedef float       f32;
typedef double      f64;

#include <string>
using std::string;
