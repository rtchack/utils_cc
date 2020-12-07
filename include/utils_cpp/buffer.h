/**
 * Created on: 6/4/16
 *     Author: xing
 */

#pragma once

#include <cstring>
#include <mutex>
#include <functional>

#include "utils_cpp/module.h"

namespace utils {
/**
 * Buffer
 */
template<size_t CAP>
class Buffer {
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

  template<typename T>
  Ret
  write(T &&t) noexcept
  {
    T tmp = t;
    return write((uint8_t *)&tmp, sizeof(T));
  }

  Ret
  write(std::function<Ret(uint8_t * , size_t & )> on_buf)
  {
    auto len = (size_t)(end_ptr - w_ptr);
    const auto ret = on_buf(w_ptr, len);
    unless(ret == Ret::OK) {
      return ret;
    }

    w_ptr += len;
    return Ret::OK;
  }

  Ret
  write_at(const uint8_t *src, size_t len, size_t offset) noexcept
  {
    if (offset >= CAP) {
      return Ret::E_ARG;
    }

    const auto old_w_ptr = w_ptr;
    w_ptr = data + offset;

    const auto ret = write(src, len);
    if (ret != Ret::OK) {
      w_ptr = old_w_ptr;
    }

    return ret;
  }

  template<typename T>
  Ret
  write_at(T &&t, size_t offset) noexcept
  {
    T tmp = t;
    return write_at((uint8_t *)&tmp, sizeof(T), offset);
  }

  Ret
  write_at(std::function<Ret(uint8_t * , size_t & )> on_buf, size_t offset)
  {
    if (offset >= CAP) {
      return Ret::E_ARG;
    }

    const auto old_w_ptr = w_ptr;
    w_ptr = data + offset;

    const auto ret = write(on_buf);
    if (ret != Ret::OK) {
      w_ptr = old_w_ptr;
    }

    return ret;
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

  template<typename T>
  Ret
  read(T &t) noexcept
  {
    auto len = sizeof(T);
    if (get_remain_data_size() < len) {
      return Ret::E_ARG;
    }

    UTILS_RAISE_UNLESS(Ret::OK == read((uint8_t *)&t, len));
    return Ret::OK;
  }

  Ret
  read_at(uint8_t *dst, size_t &len, size_t offset) noexcept
  {
    const auto old_r_ptr = r_ptr;
    r_ptr = data + offset;

    if (r_ptr >= w_ptr) {
      return Ret::E_ARG;
    }

    const auto ret = read(dst, len);
    if (Ret::OK != ret) {
      r_ptr = old_r_ptr;
    }
    return ret;
  }

  template<typename T>
  Ret
  read_at(T &t, size_t offset) noexcept
  {
    auto len = sizeof(T);
    if (get_remain_data_size(offset) < len) {
      return Ret::E_ARG;
    }

    UTILS_RAISE_UNLESS(Ret::OK == read_at((uint8_t *)&t, len, offset));
    return Ret::OK;
  }

  size_t
  get_remain_data_size(size_t offset) const noexcept
  {
    const auto total = get_total_data_size();
    return total <= offset ? 0 : total - offset;
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
  uint8_t const *end_ptr;  // end pointer

  static constexpr size_t prefix_len{sizeof(size_t) + sizeof(size_t)};

  uint8_t data[UTILS_ROUNDED(CAP)];
};

}  // namespace utils