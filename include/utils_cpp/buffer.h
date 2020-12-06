/**
 * Created on: 6/4/16
 *     Author: xing
 */

#pragma once

#include <cstring>
#include <mutex>
#include <functional>

#include "utils_cpp/module.h"

namespace utils
{
/**
 * Buffer
 */
template <size_t CAP>
class Buffer
{
 public:
  Buffer() : w_ptr{data}, r_ptr{data}, end_ptr{data + CAP} {}

  Ret
  write(const uint8_t *src, size_t len) noexcept
  {
    if (len > end_ptr - w_ptr) {
      return Ret::E_BUF_OVERFLOW;
    }

    memcpy(w_ptr, src, len);
    w_ptr += len;
    return Ret::OK;
  }

  Ret
  write_at(const uint8_t *src, size_t len, size_t offset) noexcept
  {
    if (len > CAP - offset) {
      return Ret::E_BUF_OVERFLOW;
    }

    // Reset read pointer whenever we write with a specific offset.
    r_ptr = data;

    w_ptr = data + offset;
    memcpy(w_ptr, src, len);
    w_ptr += len;
    return Ret::OK;
  }

  Ret
  read(uint8_t *dst, size_t &len) noexcept
  {
    const auto act_len = std::min(len, get_remain_data_size());
    unless(act_len) { return Ret::E_ARG; }

    memcpy(dst, r_ptr, act_len);
    r_ptr += act_len;
    len = act_len;
    return Ret::OK;
  }

  Ret
  read_at(uint8_t *dst, size_t &len, size_t offset) noexcept
  {
    if (offset > get_total_data_size()) {
      return Ret::E_ARG;
    }

    r_ptr = data + offset;
    const auto act_len = std::min(len, get_remain_data_size());
    memcpy(dst, r_ptr, act_len);
    r_ptr += act_len;
    len = act_len;
    return Ret::OK;
  }

  void
  inc_offset(size_t n) noexcept
  {
    if (!r_ptr) {
      r_ptr = data + n;
    } else {
      r_ptr += n;
    }
  }

  size_t
  get_remain_data_size() const noexcept
  {
    return w_ptr - r_ptr;
  }

  size_t
  get_total_data_size() const noexcept
  {
    return w_ptr - data;
  }

  size_t
  get_capacity() const
  {
    return CAP;
  }

 private:
  uint8_t *w_ptr;    // write pointer
  uint8_t *r_ptr;    // read pointer
  uint8_t *end_ptr;  // end pointer

  static constexpr size_t prefix_len{sizeof(size_t) + sizeof(size_t)};

  uint8_t data[UTILS_ROUNDED(CAP)];
};

}  // namespace utils