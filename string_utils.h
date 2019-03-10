/**
 * Created by xing on 4/7/16.
 */

#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <ostream>

#include "macro_utils.h"

namespace utils {

class StringAble {
 public:
  virtual std::string ToString() const = 0;

  friend std::ostream &operator<< (std::ostream &os, const StringAble &s)
  {
    return os << s.ToString ();
  }
};

inline std::string to_string(const StringAble &sa) {
  return sa.ToString();
}

namespace str_util {

void UperCase(char *str) noexcept;

std::string ToHexStr(uint8_t c) noexcept;

std::string ToHexStr(const std::vector<uint8_t> &uchars) noexcept;

std::string ToHexStr(const std::vector<uint8_t> &uchars,
                     const std::string &delimiter) noexcept;

std::string ToHexStr(const std::vector<uint8_t> &uchars,
                     const std::string &delimiter,
                     uint16_t pace) noexcept;

template<size_t N>
std::string ToHexStr(uint8_t (&chars)[N]) noexcept {
  std::string s;
  s.reserve((N + 1) << 2);
  for (auto c : chars) {
    s.append(ToHexStr(c));
  }
  return s;
}

template<size_t N>
std::string ToHexStr(uint8_t (&chars)[N],
                     const std::string &delimiter) noexcept {
  return ToHexStr(chars, delimiter, 1);
}

template<size_t N>
std::string ToHexStr(uint8_t (&chars)[N],
                     const std::string &delimiter,
                     uint16_t pace) noexcept {
  std::string s;
  s.reserve((N + 1) << 2);

  auto sz = N - 1;
  uint16_t step{0};
  for (size_t i{0}; i < sz; ++i) {
    s.append(ToHexStr(chars[i]));
    if (++step >= pace) {
      s.append(delimiter);
      step = 0;
    }
  }
  s.append(ToHexStr(chars[sz]));
  return s;
}

}
}
