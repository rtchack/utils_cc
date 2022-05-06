/*
 * Created by xing in 2021.
 */

#include <utils_c/common.h>
#include <utils_c/number.h>
#include <stdbool.h>
#include <assert.h>

#define IMP_NUM_UNWRAPPER(bits)                                                    \
int64_t seq_num_unwrapper_u##bits(seq_num_unwrapper_t *self, uint##bits##_t value) \
{                                                                                  \
  if (INT64_MIN == self->last_value) {                                             \
    self->last_unwrapped = value;                                                  \
  } else {                                                                         \
    const static int64_t num_of_values = (int64_t)1 + UINT##bits##_MAX;            \
    const static int64_t max_value_dist = ((int64_t)1 + UINT##bits##_MAX) /        \
                                          (int64_t)2;                              \
    const int64_t last_value = (int64_t)self->last_value;                          \
    const int64_t forward_diff = NUM_FORWARD_DIFF_U##bits(last_value, value);      \
    const int64_t reverse_diff = NUM_REVERSE_DIFF_U##bits(last_value, value);      \
                                                                                   \
    self->last_unwrapped += forward_diff;                                          \
                                                                                   \
    bool not_behind;                                                               \
    if (MIN(forward_diff, reverse_diff) == max_value_dist) {                       \
      not_behind = last_value < value;                                             \
    } else {                                                                       \
      not_behind = forward_diff <= max_value_dist;                                 \
    }                                                                              \
    if (!not_behind) {                                                             \
      self->last_unwrapped -= num_of_values;                                       \
    }                                                                              \
  }                                                                                \
                                                                                   \
  self->last_value = value;                                                        \
  return self->last_unwrapped;                                                     \
}

IMP_NUM_UNWRAPPER(8)

IMP_NUM_UNWRAPPER(16)

IMP_NUM_UNWRAPPER(32)

int64_t
seq_num_unwrapper_u12(seq_num_unwrapper_t *self, int16_t value)
{
  assert(value < 0x1000);

#define NUM_OF_UINT12 0x1000

  if (INT64_MIN == self->last_value) {
    self->last_unwrapped = value;
  } else {
    const static int16_t max_value_dist = NUM_OF_UINT12 / (int16_t)2;

    const int16_t last_value = (int16_t)self->last_value;

    const int16_t forward_diff = NUM_FORWARD_DIFF(last_value, value, NUM_OF_UINT12);
    const int16_t reverse_diff = NUM_REVERSE_DIFF(last_value, value, NUM_OF_UINT12);

    self->last_unwrapped += forward_diff;

    bool not_behind;

    if (MIN(forward_diff, reverse_diff) == max_value_dist) {
      not_behind = last_value < value;
    } else {
      not_behind = forward_diff <= max_value_dist;
    }

    if (!not_behind) {
      self->last_unwrapped -= NUM_OF_UINT12;
    }
  }

  self->last_value = value;
  return self->last_unwrapped;
}

#undef IMP_NUM_UNWRAPPER
