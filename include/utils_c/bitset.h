/*
 * Created by galaxyhuang in 2021.
 */

#ifndef UTILS_CPP_BITSET_H
#define UTILS_CPP_BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t *value;
  size_t len;
  size_t n_bytes;
} bitset_t;

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

void
inline_bitset_set_at(void *bitset, size_t index_in_bits);

bool
inline_bitset_get_at(void *bitset, size_t index_in_bits);

void
inline_bitset_clear(void *bitset, size_t from_in_bits, size_t to_in_bits);

void
inline_bitset_clear_all(void *bitset, size_t n_bytes);

#ifdef __cplusplus
}
#endif

#endif  // UTILS_CPP_BITSET_H
