/**
 *  Created on: Mar 9, 2016
 *      Author: xing
 */

#include "utils_cc/log.h"
#include "utils_cc/macro_utils.h"

#include <stdarg.h>
#include <cstring>

namespace ucc
{
constexpr char MSG_TRUNCATED[] = "TRUNCATED!";

std::mutex log_mut{};

void
print_log(int severity, const char *fmt, ...)
{
  char msg[N_LOG_BYTES_MAX];

  va_list args;
  va_start(args, fmt);
  if (N_LOG_BYTES_MAX <= (size_t)vsnprintf(msg, N_LOG_BYTES_MAX, fmt, args)) {
    memcpy(msg + (N_LOG_BYTES_MAX - sizeof(MSG_TRUNCATED) - 1),
           MSG_TRUNCATED,
           sizeof(MSG_TRUNCATED));
  }
  va_end(args);

  switch (severity) {
  case UCC_SEVERITY_DBG: {
    UCC_LOGGER("D " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_INF: {
    UCC_LOGGER("I " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_WAR: {
    UCC_LOGGER("W " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_ERR: {
    UCC_LOGGER("E " << std::this_thread::get_id() << " " << msg);
    return;
  }
  default: UCC_LOGGER("U " << std::this_thread::get_id() << " " << msg);
  }
}

void
vprint_log(int severity, const char *fmt, va_list vp)
{
  char msg[N_LOG_BYTES_MAX];

  if (N_LOG_BYTES_MAX <= (size_t)vsnprintf(msg, N_LOG_BYTES_MAX, fmt, vp)) {
    memcpy(msg + (N_LOG_BYTES_MAX - sizeof(MSG_TRUNCATED) - 1),
           MSG_TRUNCATED,
           sizeof(MSG_TRUNCATED));
  }

  switch (severity) {
  case UCC_SEVERITY_DBG: {
    UCC_LOGGER("D " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_INF: {
    UCC_LOGGER("I " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_WAR: {
    UCC_LOGGER("W " << std::this_thread::get_id() << " " << msg);
    return;
  }
  case UCC_SEVERITY_ERR: {
    UCC_LOGGER("E " << std::this_thread::get_id() << " " << msg);
    return;
  }
  default: UCC_LOGGER("U " << std::this_thread::get_id() << " " << msg);
  }
}

void
print_binary(const char *tag, const void *buf, size_t buf_len)
{
  char msg[N_LOG_BYTES_MAX];
  auto *tmp_buf = (const unsigned char *)buf;
  size_t offset = 0;
  size_t i = 0;

  unless(tmp_buf) return;

#define LOG_BINARY_PUTN(fmt, ...)                                             \
  offset +=                                                                   \
      snprintf(msg + offset, (N_LOG_BYTES_MAX - offset), fmt, ##__VA_ARGS__); \
  if (N_LOG_BYTES_MAX <= offset) {                                            \
    memcpy(msg + (offset - sizeof(MSG_TRUNCATED) - 1),                        \
           MSG_TRUNCATED,                                                     \
           sizeof(MSG_TRUNCATED));                                            \
    goto print_point;                                                         \
  }

  LOG_BINARY_PUTN("[%s] {\n", tag)

  while (i < buf_len) {
    LOG_BINARY_PUTN("%02x", tmp_buf[i])
    unless((++i % 8)) { LOG_BINARY_PUTN("  ")}
    else { LOG_BINARY_PUTN(" ")}
    unless((i % 16)) { LOG_BINARY_PUTN("\n")}
  }

print_point:
  print_log(UCC_SEVERITY_INF, "%s}", msg);
}

}  // namespace ucc
