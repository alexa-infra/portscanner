#pragma once

#if defined(_WIN32) || defined(__WIN32__)
#   define OS_WIN
#elif defined(__APPLE_CC__)
#   define OS_MAC
#elif defined(__linux__)
#   define OS_LINUX
#else
#   error Operating system is not supported
#endif

#if defined(OS_MAC) || defined(OS_LINUX)
#   define OS_POSIX
#endif

#if defined(__GNUC__)
#   define COMPILER_GCC
#elif defined(_MSC_VER)
#   define COMPILER_MSVC
#else
#   error Compiler is not supported
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64) || \
    defined(__alpha__) || defined(__ia64__)
#   define OS_ARCH_64
#else
#   define OS_ARCH_32
#endif
