/*
 * Created by galaxyhuang in 2021.
 */

#ifndef UTILS_CPP_COMMON_H
#define UTILS_CPP_COMMON_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
#define UC_DCHECK(act) assert(act);
#else
#define UC_DCHECK(act)
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define UC_UNUSED(v) (void)v;
#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))
#endif

#define UC_ROUND_UNIT 16U
#define UC_ROUND(size, round) (((size) + (round)-1) & (~((round)-1)))
#define UC_ROUNDED(size) (UC_ROUND((size), UC_ROUND_UNIT))

#define UC_ARRAYSZ(array) (sizeof(array) / sizeof(array[0]))

#ifdef __cplusplus
};
#endif

#endif  // UTILS_CPP_COMMON_H
