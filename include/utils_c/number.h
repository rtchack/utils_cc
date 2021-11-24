/*
 * Created by xing in 2021.
 */

#ifndef UTILS_CC_C_NUMBER_H
#define UTILS_CC_C_NUMBER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef ABS
#define ABS(a)((a) > 0 ? (a) : -1 * (a))
#endif

#ifndef NUM_FORWARD_DIFF
#define NUM_FORWARD_DIFF(a, b, M) ((a) <= (b) ? (b) - (a) : (M) - ((a) - (b)))
#endif

#ifndef NUM_FORWARD_DIFF_U8
#define NUM_FORWARD_DIFF_U8(a, b) NUM_FORWARD_DIFF((a), (b), UINT8_MAX + 1)
#endif

#ifndef NUM_FORWARD_DIFF_U16
#define NUM_FORWARD_DIFF_U16(a, b) NUM_FORWARD_DIFF((a), (b), UINT16_MAX + 1)
#endif

#ifndef NUM_FORWARD_DIFF_U32
#define NUM_FORWARD_DIFF_U32(a, b) NUM_FORWARD_DIFF((a), (b), UINT32_MAX + 1)
#endif

#ifndef NUM_REVERSE_DIFF
#define NUM_REVERSE_DIFF(a, b, M) ((b) <= (a) ? (a) - (b) : (M) - ((b) - (a)))
#endif

#ifndef NUM_REVERSE_DIFF_U8
#define NUM_REVERSE_DIFF_U8(a, b) NUM_REVERSE_DIFF(a, b, UINT8_MAX + 1)
#endif

#ifndef NUM_REVERSE_DIFF_U16
#define NUM_REVERSE_DIFF_U16(a, b) NUM_REVERSE_DIFF(a, b, UINT16_MAX + 1)
#endif

#ifndef NUM_REVERSE_DIFF_U32
#define NUM_REVERSE_DIFF_U32(a, b) NUM_REVERSE_DIFF(a, b, UINT32_MAX + 1)
#endif

#ifndef NUM_MIN_DIFF
#define NUM_MIN_DIFF(a, b, M) MIN(NUM_FORWARD_DIFF((a),(b),(M)), NUM_REVERSE_DIFF((a),(b),(M)))
#endif

#ifndef NUM_MIN_DIFF_U8
#define NUM_MIN_DIFF_U8(a, b) NUM_MIN_DIFF((a), (b), UINT8_MAX + 1)
#endif

#ifndef NUM_MIN_DIFF_U16
#define NUM_MIN_DIFF_U16(a, b) NUM_MIN_DIFF((a), (b), UINT16_MAX + 1)
#endif

#ifndef NUM_MIN_DIFF_U32
#define NUM_MIN_DIFF_U32(a, b) NUM_MIN_DIFF((a), (b), UINT32_MAX + 1)
#endif

#ifndef CEIL
#define CEIL(a, b) (((a) + ((b) - 1) )/ (b))
#endif

typedef struct {
  int64_t last_value;
  int64_t last_unwrapped;
} seq_num_unwrapper_t;

static const seq_num_unwrapper_t SEQ_NUM_UNWRAPPER_INIT = {INT64_MIN, 0};

int64_t
seq_num_unwrapper_u8(seq_num_unwrapper_t *self, uint8_t value);

int64_t
seq_num_unwrapper_u12(seq_num_unwrapper_t *self, int16_t value);

int64_t
seq_num_unwrapper_u16(seq_num_unwrapper_t *self, uint16_t value);

int64_t
seq_num_unwrapper_u32(seq_num_unwrapper_t *self, uint32_t value);

#ifdef __cplusplus
};
#endif

#endif  // UTILS_CC_C_NUMBER_H
