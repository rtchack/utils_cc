/**
 * Created by xing on 4/7/16.
 */

#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <ostream>

#include "utils_cc/macro_utils.h"

namespace ucc
{
class StringAble {
 public:
  virtual std::string
  to_s() const = 0;

  friend std::ostream &
  operator<<(std::ostream &os, const StringAble &s)
  {
    return os << s.to_s();
  }
};

inline std::string
to_s(const StringAble &sa)
{
  return sa.to_s();
}

namespace str_util
{
void
upper_case(char *str) noexcept;

std::string
to_hex_str(uint8_t c) noexcept;

std::string
to_hex_str(const std::vector<uint8_t> &uchars) noexcept;

std::string
to_hex_str(const std::vector<uint8_t> &uchars,
           const std::string &delimiter) noexcept;

std::string
to_hex_str(const std::vector<uint8_t> &uchars,
           const std::string &delimiter,
           uint16_t pace) noexcept;

template<size_t N>
std::string to_hex_str(uint8_t (&chars)[N]) noexcept
{
  std::string s;
  s.reserve((N + 1) << 2);
  for (auto c: chars) {
    s.append(to_hex_str(c));
  }
  return s;
}

template<size_t N>
std::string
to_hex_str(uint8_t (&chars)[N], const std::string &delimiter) noexcept
{
  return to_hex_str(chars, delimiter, 1);
}

template<size_t N>
std::string
to_hex_str(uint8_t (&chars)[N],
           const std::string &delimiter,
           uint16_t pace) noexcept
{
  std::string s;
  s.reserve((N + 1) << 2);

  auto sz = N - 1;
  uint16_t step{0};
  for (size_t i{0}; i < sz; ++i) {
    s.append(to_hex_str(chars[i]));
    if (++step >= pace) {
      s.append(delimiter);
      step = 0;
    }
  }
  s.append(to_hex_str(chars[sz]));
  return s;
}

}  // namespace str_util
}  // namespace ucc
