/*
 * Created by xing in 2021.
 */

#ifndef UTILS_CC_C_BITSET_H
#define UTILS_CC_C_BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A C version of C++ bitset.
 */
typedef struct bitset_s bitset_t;

bitset_t *
bitset_new(size_t len);

void
bitset_del(bitset_t *bitset);

void
bitset_set_at(bitset_t *bitset, size_t index);

bool
bitset_get_at(bitset_t *bitset, size_t index);

void
bitset_clear(bitset_t *bitset, size_t from, size_t to);

void
bitset_clear_all(bitset_t *bitset);

#define BITSET_DECLARE_FOR(type)                                     \
  void type##_bitset_set_at(type##_t *bitset, size_t index_in_bits); \
                                                                     \
  bool type##_bitset_get_at(type##_t *bitset, size_t index_in_bits); \
                                                                     \
  void type##_bitset_clear(                                          \
      type##_t *bitset, size_t from_in_bits, size_t to_in_bits);     \
                                                                     \
  void type##_bitset_clear_all(type##_t *bitset);                    \
                                                                     \
  static inline void type##_bitset_init(type##_t *bitset)            \
  {                                                                  \
    type##_bitset_clear_all(bitset);                                 \
  }

BITSET_DECLARE_FOR(uint8)
BITSET_DECLARE_FOR(uint16)
BITSET_DECLARE_FOR(uint32)
BITSET_DECLARE_FOR(uint64)

#undef BITSET_DECLARE_FOR

#ifdef __cplusplus
}
#endif

#endif  // UTILS_CC_C_BITSET_H
