/*
 * Created by galaxyhuang in 2021.
 */

#include <utils_c/bitset.h>
#include <utils_c/common.h>
#include <string.h>

bitset_t *
bitset_new(size_t len)
{
  const size_t n_bytes = (len + sizeof(uint8_t) - 1) / sizeof(uint8_t);
  bitset_t *self = malloc(sizeof(bitset_t) + n_bytes);
  if (!self) {
    return self;
  }

  self->len = len;
  self->n_bytes = n_bytes;
  self->value = (uint8_t *)(self + 1);
  memset(self->value, 0, self->len);

  return self;
}

void
bitset_del(bitset_t *self)
{
  free(self);
}

void
bitset_set_at(bitset_t *self, size_t index)
{
  const size_t i = index >> 3;
  const uint8_t m = (uint8_t)(index % 8);

  UC_DCHECK(index < self->len);
  if (index >= self->len) {
    return;
  }

  self->value[i] |= (1 << m);
}

bool
bitset_get_at(bitset_t *self, size_t index)
{
  const size_t i = index >> 3;
  const uint8_t m = (uint8_t)(index % 8);

  UC_DCHECK(index < self->len);
  if (index >= self->len) {
    return false;
  }

  return self->value[i] & (1 << m);
}

void
bitset_clear(bitset_t *self, size_t from, size_t to)
{
  UC_DCHECK(to < self->len);

  if (from == to) {
    const size_t i = from >> 3;
    const uint8_t m = (uint8_t)(to % 8);

    self->value[i] ^= (1 << m);
  } else if (from < to) {
    const size_t i_from = from >> 3;
    uint8_t m_from = (uint8_t)(from % 8);

    const size_t i_to = to >> 3;
    uint8_t m_to = (uint8_t)(to % 8);

    if (i_to == i_from) {
      while (m_from <= m_to) {
        self->value[i_from] ^= 1 << m_from;
        ++m_from;
      }
    } else {
      if (i_to > 1 && i_from < i_to - 1) {
        memset(self->value + i_from + 1, 0, i_to - i_from - 1);
      }
      while (m_from != 8) {
        self->value[i_from] ^= 1 << m_from;
        ++m_from;
      }
      while (m_to != UINT8_MAX) {
        self->value[i_to] ^= 1 << m_to;
        --m_to;
      }
    }
  } else {
    UC_DCHECK(false);
  }
}

void
bitset_clear_all(bitset_t *self)
{
  memset(self->value, 0, self->n_bytes);
}

void
inline_bitset_set_at(void *bitset, size_t index_in_bits)
{
  const size_t i = index_in_bits >> 3;
  const uint8_t m = (uint8_t)(index_in_bits % 8);
  ((uint8_t *)bitset)[i] |= (1 << m);
}

bool
inline_bitset_get_at(void *bitset, size_t index_in_bits)
{
  const size_t i = index_in_bits >> 3;
  const uint8_t m = (uint8_t)(index_in_bits % 8);
  return ((uint8_t *)bitset)[i] & (1 << m);
}

void
inline_bitset_clear(void *bitset, size_t from_in_bits, size_t to_in_bits)
{
  if (from_in_bits == to_in_bits) {
    const size_t i = from_in_bits >> 3;
    const uint8_t m = (uint8_t)(to_in_bits % 8);

    ((uint8_t *)bitset)[i] ^= (1 << m);
  } else if (from_in_bits < to_in_bits) {
    const size_t i_from = from_in_bits >> 3;
    uint8_t m_from = (uint8_t)(from_in_bits % 8);

    const size_t i_to = to_in_bits >> 3;
    uint8_t m_to = (uint8_t)(to_in_bits % 8);

    if (i_to == i_from) {
      while (m_from <= m_to) {
        ((uint8_t *)bitset)[i_from] ^= 1 << m_from;
        ++m_from;
      }
    } else {
      if (i_to > 1 && i_from < i_to - 1) {
        memset(((uint8_t *)bitset) + i_from + 1, 0, i_to - i_from - 1);
      }
      while (m_from != 8) {
        ((uint8_t *)bitset)[i_from] ^= 1 << m_from;
        ++m_from;
      }
      while (m_to != UINT8_MAX) {
        ((uint8_t *)bitset)[i_to] ^= 1 << m_to;
        --m_to;
      }
    }
  } else {
    UC_DCHECK(false);
  }
}

void
inline_bitset_clear_all(void *bitset, size_t n_bytes)
{
  memset(bitset, 0, n_bytes);
}
