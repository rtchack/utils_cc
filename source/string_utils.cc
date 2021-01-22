/**
 * Created by xing on 4/7/16.
 */

#include "utils_cc/string_utils.h"

namespace ucc
{
namespace str_util
{
void
upper_case(char *str) noexcept
{
  while (*str) {
    if (*str <= 122 && *str >= 97) {
      *str -= 32;
    } else if ('-' == *str) {
      *str = '_';
    }

    ++str;
  }
}

std::string
to_hex_str(uint8_t c) noexcept
{
  char str[4];
  snprintf(str, 4, "%02x", c);
  return str;
}

std::string
to_hex_str(const std::vector<uint8_t> &chars) noexcept
{
  std::string s;
  s.reserve((chars.size() + 1) << 2);
  for (auto c : chars) {
    s.append(to_hex_str(c));
  }
  return s;
}

std::string
to_hex_str(const std::vector<uint8_t> &chars,
           const std::string &delimiter) noexcept
{
  return to_hex_str(chars, delimiter, 1);
}

std::string
to_hex_str(const std::vector<uint8_t> &chars,
           const std::string &delimiter,
           uint16_t pace) noexcept
{
  std::string s;
  s.reserve((chars.size() + 1) << 2);

  auto sz = chars.size() - 1;
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
