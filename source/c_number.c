/*
 * Created by galaxyhuang in 2021.
 */

#include <utils_c/common.h>
#include <utils_c/number.h>
#include <stdbool.h>
#include <assert.h>

#define IMP_NUM_UNWRAPPER(type, TYPE)                                       \
  int64_t seq_num_unwrapper_on_##type(seq_num_unwrapper_t *self,           \
                                       type##_t value)                      \
  {                                                                         \
    if (INT64_MIN == self->last_value) {                                    \
      self->last_unwrapped = value;                                         \
    } else {                                                                \
      const static type##_t MAX_VALUE_DIST =                                \
          TYPE##_MAX / (type##_t)2 + (type##_t)1;                           \
      const type##_t last_value = (type##_t)self->last_value;               \
      bool is_ahead_or_at;                                                  \
                                                                            \
      self->last_unwrapped += (type##_t)(value - last_value);               \
                                                                            \
      if (value - last_value == MAX_VALUE_DIST) {                           \
        is_ahead_or_at = last_value < value;                                \
      } else {                                                              \
        is_ahead_or_at = (type##_t)(value - last_value) < MAX_VALUE_DIST;   \
      }                                                                     \
      if (!is_ahead_or_at) {                                                \
        const static int64_t backward_adjustment = (int64_t)TYPE##_MAX + 1; \
        self->last_unwrapped -= backward_adjustment;                        \
      }                                                                     \
    }                                                                       \
                                                                            \
    self->last_value = value;                                               \
    return self->last_unwrapped;                                            \
  }

IMP_NUM_UNWRAPPER(uint8, UINT8)

IMP_NUM_UNWRAPPER(uint16, UINT16)

IMP_NUM_UNWRAPPER(uint32, UINT32)

int64_t
seq_num_unwrapper_on_uint12(seq_num_unwrapper_t *self, uint16_t value)
{
  assert(value < 0x1000);

  if (INT64_MIN == self->last_value) {
    self->last_unwrapped = value;
  } else {
    const static uint16_t MAX_VALUE_DIST = 0x1000 / (uint16_t)2;

    const uint16_t last_value = (uint16_t)self->last_value;
    bool is_ahead_or_at;

    self->last_unwrapped += last_value <= value ? value - last_value
                                                : 0x1000 - (last_value - value);

    if (NUM_MIN_DIFF(value, last_value, 0x1000) == MAX_VALUE_DIST) {
      is_ahead_or_at = last_value < value;
    } else {
      is_ahead_or_at =
          NUM_FORWARD_DIFF(last_value, value, 0x1000) <= MAX_VALUE_DIST;
    }
    if (!is_ahead_or_at) {
      self->last_unwrapped -= (int64_t)0x1000;
    }
  }

  self->last_value = value;
  return self->last_unwrapped;
}

#undef IMP_NUM_UNWRAPPER
