/*
 * Created by xing in 2021.
 */

#include <utils_c/bitset.h>
#include <utils_c/common.h>
#include <string.h>

void
inline_bitset_set(void *self, size_t index_in_bits)
{
  const size_t i = index_in_bits >> 3;
  const int m = (int)(index_in_bits % 8);
  ((uint8_t *)self)[i] |= (1 << m);
}

bool
inline_bitset_get(const void *self, size_t index_in_bits)
{
  const size_t i = index_in_bits >> 3;
  const int m = (int)(index_in_bits % 8);
  return ((uint8_t *)self)[i] & (1 << m);
}

void
inline_bitset_clear(void *self, size_t from_in_bits, size_t to_in_bits)
{
  if (from_in_bits == to_in_bits) {
    const size_t i = from_in_bits >> 3;
    const int m = (int)(to_in_bits % 8);

    ((uint8_t *)self)[i] ^= (1 << m);
  } else if (from_in_bits < to_in_bits) {
    const size_t i_from = from_in_bits >> 3;
    int m_from = (int)(from_in_bits % 8);

    const size_t i_to = to_in_bits >> 3;
    int m_to = (int)(to_in_bits % 8);

    if (i_to == i_from) {
      while (m_from <= m_to) {
        ((uint8_t *)self)[i_from] ^= 1 << m_from;
        ++m_from;
      }
    } else {
      if (i_to > 1 && i_from < i_to - 1) {
        memset(((uint8_t *)self) + i_from + 1, 0, i_to - i_from - 1);
      }
      while (m_from < 8) {
        ((uint8_t *)self)[i_from] ^= 1 << m_from;
        ++m_from;
      }
      while (m_to >= 0) {
        ((uint8_t *)self)[i_to] ^= 1 << m_to;
        --m_to;
      }
    }
  } else {
    UC_DCHECK(false);
  }
}

void
inline_bitset_clear_all(void *self, size_t n_bytes)
{
  memset(self, 0, n_bytes);
}

bitset_t *
bitset_new(size_t len)
{
  const size_t n_bytes = (len + (1 << 3) - 1) >> 3;
  bitset_t *self = malloc(sizeof(bitset_t) + n_bytes);
  if (!self) {
    return self;
  }

  UC_DCHECK(len);

  self->count = 0;
  self->is_count_valid = true;
  self->len = len;
  self->n_bytes = n_bytes;
  self->value = (uint8_t *)(self + 1);
  memset(self->value, 0, self->n_bytes);

  return self;
}

void
bitset_attach(bitset_t *self, uint8_t *value, size_t n_bytes)
{
  UC_DCHECK(value);
  UC_DCHECK(n_bytes);

  self->count = 0;
  self->is_count_valid = true;
  self->len = n_bytes << 3;
  self->n_bytes = n_bytes;
  self->value = value;
  memset(self->value, 0, self->n_bytes);
}

void
bitset_del(bitset_t *self)
{
  free(self);
}

void
bitset_set(bitset_t *bitset, size_t index)
{
  const size_t i = index >> 3;
  const int m = (int)(index % 8);

  UC_DCHECK(index < bitset->len);
  if (index >= bitset->len) {
    return;
  }

  if (bitset->is_count_valid) {
    if (!(bitset->value[i] & (1 << m))) {
      ++bitset->count;
    }
  }

  bitset->value[i] |= (1 << m);
}

bool
bitset_get(const bitset_t *bitset, size_t index)
{
  UC_DCHECK(index < bitset->len);
  if (index >= bitset->len) {
    return false;
  }
  return inline_bitset_get(bitset->value, index);
}

bool
bitset_any(bitset_t *self)
{
  if (self->is_count_valid) {
    return (bool)self->count;
  }

  {
    size_t n = 0;
    while (n != self->n_bytes) {
      if (self->value[n] > 0) {
        return true;
      }
      ++n;
    }
  }

  return false;
}

bool
bitset_all(bitset_t *self)
{
  if (self->is_count_valid) {
    return self->count == self->len;
  }

  {
    size_t n = 0;
    const size_t n_to = self->len % 8 ? self->n_bytes - 1 : self->n_bytes;

    while (n != n_to) {
      if (self->value[n] != 0xff) {
        return false;
      }
      ++n;
    }

    if (n != self->n_bytes) {
      UC_DCHECK(n == self->n_bytes - 1);
      const uint8_t v = self->value[n];
      int m = (int)(self->len % 8);
      while (m-- > 0) {
        if (!(v & (1 << m))) {
          return false;
        }
      }
    }
  }

  return true;
}

size_t
bitset_num(bitset_t *self)
{
  if (!self->is_count_valid) {
    size_t i;
    size_t i_max;
    uint8_t v;

    int m = (int)(self->len % 8);

    self->count = 0;

    if (m) {
      i_max = self->n_bytes - 1;
      v = self->value[i_max];

      while (m-- > 0) {
        if (v & (1 << m)) {
          ++self->count;
        }
      }

    } else {
      i_max = self->n_bytes;
    }

    for (i = 0; i != i_max; ++i) {
      v = self->value[i];
      m = 0;

      while (m < 8) {
        if (v & (1 << m)) {
          ++self->count;
        }
        ++m;
      }
    }

    self->is_count_valid = true;
  }

  return self->count;
}

void
bitset_clear(bitset_t *self, size_t from, size_t to)
{
  UC_DCHECK(to < self->len);
  if (to >= self->len) {
    return;
  }
  inline_bitset_clear(self->value, from, to);
  self->is_count_valid = false;
}

void
bitset_clear_all(bitset_t *self)
{
  memset(self->value, 0, self->n_bytes);
  self->count = 0;
  self->is_count_valid = true;
}

#define BITSET_IMP_FOR(type)                                  \
  void type##_bitset_set(type##_t *self, size_t index)        \
  {                                                           \
    UC_DCHECK(index < sizeof(type##_t) << 3);                 \
    if (index >= sizeof(type##_t) << 3) {                     \
      return;                                                 \
    }                                                         \
    inline_bitset_set(self, index);                           \
  }                                                           \
                                                              \
  bool type##_bitset_get(const type##_t *self, size_t index)  \
  {                                                           \
    UC_DCHECK(index < sizeof(type##_t) << 3);                 \
    if (index >= sizeof(type##_t) << 3) {                     \
      return false;                                           \
    }                                                         \
    return inline_bitset_get(self, index);                    \
  }                                                           \
                                                              \
  void type##_bitset_clear(                                   \
      type##_t *self, size_t from_in_bits, size_t to_in_bits) \
  {                                                           \
    UC_DCHECK(to_in_bits < sizeof(type##_t) << 3);            \
    if (to_in_bits >= sizeof(type##_t) << 3) {                \
      return;                                                 \
    }                                                         \
    inline_bitset_clear(self, from_in_bits, to_in_bits);      \
  }                                                           \
                                                              \
  void type##_bitset_clear_all(type##_t *self) { *self = (type##_t)0; }

BITSET_IMP_FOR(uint8)
BITSET_IMP_FOR(uint16)
BITSET_IMP_FOR(uint32)
BITSET_IMP_FOR(uint64)

#undef BITSET_IMP_FOR
