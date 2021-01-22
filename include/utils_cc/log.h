/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#pragma once

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <mutex>
#include <thread>

#define UCC_SEVERITY_DBG 3
#define UCC_SEVERITY_INF 2
#define UCC_SEVERITY_WAR 1
#define UCC_SEVERITY_ERR 0

#if NDEBUG
#define UCC_CURRENT_SEVERITY UCC_SEVERITY_INF
#else
#define UCC_CURRENT_SEVERITY UCC_SEVERITY_DBG
#endif

#define UCC_LOGGER(msg)                                   \
  {                                                       \
    std::lock_guard<std::mutex> no_overlap{ucc::log_mut}; \
    std::cout << msg << std::endl;                        \
  }
#define UCC_INIT_LOG  // UCC_LOGGER("Hello.")
#define UCC_INIT_LOG_WITH(file_name) \
  UCC_LOGGER("Use STD_COUT instead of " << file_name)
#define UCC_SHUTDOWN_LOG

// Debugging logger
#if UCC_CURRENT_SEVERITY >= UCC_SEVERITY_DBG
#define UCC_DBG_ONLY(act) \
  {                       \
    act;                  \
  }
#define lDbg(msg)                                                          \
  UCC_LOGGER("D " << std::this_thread::get_id() << " [" << __func__ << " " \
                  << __LINE__ << "] " << msg);
#define printDbg(fmt, ...) \
  print_log(               \
      UCC_SEVERITY_DBG, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__);
#else
#define lDbg(msg) ;
#define printDbg(fmt, ...) ;
#define UCC_DBG_ONLY(act) ;
#endif

// Informative logger
#if UCC_CURRENT_SEVERITY >= UCC_SEVERITY_INF

#define lInf(msg)                                                           \
  UCC_LOGGER("I " << std::this_thread::get_id() << " [" << __func__ << "] " \
                  << msg);

#define printInf(fmt, ...) \
  print_log(UCC_SEVERITY_INF, "[%s] " fmt, __func__, ##__VA_ARGS__);
#else
#define lInf(msg) ;
#define printInf(fmt, ...) ;
#endif

// Warning logger
#if UCC_CURRENT_SEVERITY >= UCC_SEVERITY_WAR

#define lWar(msg)                                                           \
  UCC_LOGGER("W " << std::this_thread::get_id() << " [" << __func__ << "] " \
                  << msg);

#define printWar(fmt, ...) \
  print_log(UCC_SEVERITY_WAR, "[%s] " fmt, __func__, ##__VA_ARGS__);
#else
#define lWar(msg) ;
#define printWar(fmt, ...) ;
#endif

// Erroneous logger
#define lErr(msg)                                                           \
  UCC_LOGGER("E " << std::this_thread::get_id() << " [" << __func__ << "] " \
                  << msg);

#define printErr(fmt, ...) \
  print_log(UCC_SEVERITY_ERR, "[%s] " fmt, __func__, ##__VA_ARGS__);

#define lFatal(msg)    \
  {                    \
    lErr(msg) exit(1); \
  }
#define printFatal(fmt, ...)              \
  {                                       \
    printErr(fmt, ##__VA_ARGS__) exit(1); \
  }

#define mDbg(msg) lDbg(get_name() << "| " << msg)
#define mInf(msg) lInf(get_name() << "| " << msg)
#define mWar(msg) lWar(get_name() << "| " << msg)
#define mErr(msg) lErr(get_name() << "| " << msg)
#define mFatal(msg) lFatal(get_name() << "| " << msg)

namespace ucc
{
constexpr size_t N_LOG_BYTES_MAX{512};

extern std::mutex log_mut;

void
print_log(int severity, const char *fmt, ...);

void
vprint_log(int severity, const char *fmt, va_list vp);

void
print_binary(const char *tag, const void *buf, size_t buf_len);

#define lBinary(buf, buf_len) utils::print_binary(__func__, buf, buf_len);

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
    UCC_LOGGER(s.str());
  }

 private:
  std::stringstream s{};
};

#define UCC_LAZY_LOG_VAR(logger, var) logger << #var << " ~ " << var << ", ";

}  // namespace ucc
