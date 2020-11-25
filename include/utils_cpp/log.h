/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#pragma once

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <mutex>

#define UTILS_SEVERITY_DBG 3
#define UTILS_SEVERITY_INF 2
#define UTILS_SEVERITY_WAR 1
#define UTILS_SEVERITY_ERR 0

#if NDEBUG
#define UTILS_CURRENT_SEVERITY UTILS_SEVERITY_INF
#else
#define UTILS_CURRENT_SEVERITY UTILS_SEVERITY_DBG
#endif

#define UTILS_LOGGER(msg)                                   \
  {                                                         \
    std::lock_guard<std::mutex> no_overlap{utils::log_mut}; \
    std::cout << msg << std::endl;                          \
  }
#define UTILS_INIT_LOG  // UTILS_LOGGER("Hello.")
#define UTILS_INIT_LOG_WITH(file_name) \
  UTILS_LOGGER("Use STD_COUT instead of " << file_name)
#define UTILS_SHUTDOWN_LOG

// Debugging logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_DBG
#define UTILS_DBG_ONLY(act) \
  {                         \
    act;                    \
  }
#define lDbg(msg) \
  UTILS_LOGGER("D [" << __FUNCTION__ << " " << __LINE__ << "] " << msg);
#define printDbg(fmt, ...)          \
  print_log(base::LogSeverity::DBG, \
            "[%s %d] " fmt,         \
            __FUNCTION__,           \
            __LINE__,               \
            ##__VA_ARGS__);
#else
#define lDbg(msg) ;
#define printDbg(fmt, ...) ;
#define UTILS_DBG_ONLY(act) ;
#endif

// Informative logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_INF

#define lInf(msg) UTILS_LOGGER("I [" << __FUNCTION__ << "] " << msg);

#define printInf(fmt, ...) \
  print_log(base::LogSeverity::INF, "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lInf(msg) ;
#define printInf(fmt, ...) ;
#endif

// Warning logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_WAR

#define lWar(msg) UTILS_LOGGER("W [" << __FUNCTION__ << "] " << msg);

#define printWar(fmt, ...) \
  print_log(base::LogSeverity::WAR, "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lWar(msg) ;
#define printWar(fmt, ...) ;
#endif

// Erroneous logger
#define lErr(msg) UTILS_LOGGER("E [" << __FUNCTION__ << "] " << msg);

#define printErr(fmt, ...) \
  print_log(base::LogSeverity::ERR, "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);

#define lFatal(msg)    \
  {                    \
    lErr(msg) exit(1); \
  }
#define printFatal(fmt, ...)              \
  {                                       \
    printErr(fmt, ##__VA_ARGS__) exit(1); \
  }

#define mDbg(msg) lDbg("[" << get_name() << "] " << msg)
#define mInf(msg) lInf("[" << get_name() << "] " << msg)
#define mWar(msg) lWar("[" << get_name() << "] " << msg)
#define mErr(msg) lErr("[" << get_name() << "] " << msg)
#define mFatal(msg) lFatal("[" << get_name() << "] " << msg)

namespace utils
{
constexpr size_t UTILS_MAX_LOG_MSG_LENGTH{512};

extern std::mutex log_mut;

enum class LogSeverity { DBG, INF, WAR, ERR };

void
print_log(LogSeverity severity, const char *fmt, ...);

void
print_binary(const char *tag, const void *buf, size_t buf_len);

#define lBinary(buf, buf_len) utils::print_binary(__FUNCTION__, buf, buf_len);

class LazyLogger
{
 public:
  LazyLogger()
  {
    s << "["
      << "] { ";
  }

  LazyLogger(const char *init_msg) { s << "[" << init_msg << "] { "; }

  inline std::ostream &
  operator<<(const std::string &msg)
  {
    return s << msg;
  }

  ~LazyLogger()
  {
    s << " }";
    UTILS_LOGGER(s.str());
  }

 private:
  std::stringstream s{};
};

#define UTILS_LAZY_LOG_VAR(logger, var) logger << #var << " ~ " << var << ", ";

}  // namespace utils
