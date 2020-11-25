/**
 *  Created on: Mar 9, 2016
 *      Author: xing
 */

#include "utils_cpp/log.h"
#include "utils_cpp/macro_utils.h"

#include <stdarg.h>

namespace utils
{
#define LOG_TRUNCATED_NOTIFY_MSG " TRUNCATED!"

constexpr size_t TRUNCAT_NOTIFY_MSG_LEN{16};
constexpr size_t REAL_BASE_MAX_LOG_MSG_LENGTH{UTILS_MAX_LOG_MSG_LENGTH -
                                              TRUNCAT_NOTIFY_MSG_LEN};
constexpr size_t REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1{
    REAL_BASE_MAX_LOG_MSG_LENGTH - 1};

std::mutex log_mut{};

void
print_log(int severity, const char *fmt, ...)
{
  char msg[UTILS_MAX_LOG_MSG_LENGTH];
  va_list args;
  va_start(args, fmt);
  if (REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 <=
      (size_t)vsnprintf(msg, REAL_BASE_MAX_LOG_MSG_LENGTH, fmt, args)) {
    snprintf(msg + REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 - 1,
             TRUNCAT_NOTIFY_MSG_LEN,
             LOG_TRUNCATED_NOTIFY_MSG);
  }
  va_end(args);

  switch (severity) {
    case UTILS_SEVERITY_DBG:
      UTILS_LOGGER("D " << msg);
      return;
    case UTILS_SEVERITY_INF:
      UTILS_LOGGER("I " << msg);
      return;
    case UTILS_SEVERITY_WAR:
      UTILS_LOGGER("W " << msg);
      return;
    case UTILS_SEVERITY_ERR:
      UTILS_LOGGER("E " << msg);
      return;
  }
}

void
print_binary(const char *tag, const void *buf, size_t buf_len)
{
  char msg[UTILS_MAX_LOG_MSG_LENGTH];
  auto *tmp_buf = (const unsigned char *)buf;
  size_t index = 0;
  size_t i = 0;

  unless(tmp_buf) return;

#define LOG_BINARY_PUTN(fmt, ...)                                   \
  index += snprintf(msg + index,                                    \
                    (REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 - index), \
                    fmt,                                            \
                    ##__VA_ARGS__);                                 \
  if (REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 <= index) {              \
    snprintf(msg + index, 24, "...");                               \
    goto print_point;                                               \
  }

  LOG_BINARY_PUTN("[%s] {\n", tag)

  while (i < buf_len) {
    LOG_BINARY_PUTN("%02x", tmp_buf[i])
    unless((++i % 8)) { LOG_BINARY_PUTN("  ") }
    else {LOG_BINARY_PUTN(" ")} unless((i % 16)) { LOG_BINARY_PUTN("\n") }
  }

print_point:
  print_log(UTILS_SEVERITY_INF, "%s}", msg);
}

}  // namespace utils
