/**
  * @file cmsis_compiler.h - Compiler abstraction header
  */
#ifndef __CMSIS_COMPILER_H
#define __CMSIS_COMPILER_H

/* GCC compiler */
#define __WEAK        __attribute__((weak))
#define __PACKED      __attribute__((packed, aligned(1)))
#define __ALIGNED(x)  __attribute__((aligned(x)))
#define __STATIC_INLINE static inline
#define __FORCEINLINE __attribute__((always_inline)) static inline

#endif
