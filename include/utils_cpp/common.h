/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#pragma once

#include <string>
#include <stdexcept>
#include <sstream>

#ifdef UNIX
#include <unistd.h>
#endif

#include "utils_cpp/macro_utils.h"

#ifndef ENABLE_CPP_EXCEPTION
#include "utils_cpp/log.h"
#endif

namespace utils
{
/**
 * return value
 */
enum class Ret {
#define UTILS_RET_ERROR 0x8000;

  OK = 0,  // OK
  NO,      // no

  E_ARG,       // parameter error
  E_ARG_NULL,  // nil parameter detected

  E_MEM,   // memory allocation error
  E_INIT,  // not properly initialized yet

  E_FILE_OP,   // file operation error
  E_FILE_CON,  // file content error

  E_UNSUP,         // unsupported type
  E_ALG,           // failed on algorithm
  E_BUF_OVERFLOW,  // buffer overflow

  E_IO_PENDING,  // operation could not be completed synchronously

  E_SOCKET_NOT_CONNECTED,  // socket is not currently connected

  E_GENERAL  // general error
};

std::string
to_s(const Ret value) noexcept;

constexpr bool
failed(const Ret ret) noexcept
{
  return ret > Ret::NO;
}

constexpr bool
succeed(const Ret ret) noexcept
{
  return !failed(ret);
}

#ifdef ENABLE_CPP_EXCEPTION

/**
 * exception
 */
class Excep : public std::exception
{
 public:
  Excep() = delete;

  Excep(const std::string &message)
      : msg{std::to_s(getpid()) + ": " + message}
  {
  }

  Excep(Ret ret) : msg{to_s(ret)} {}

  Excep(Ret ret, const std::string &message) : Excep(ret)
  {
    msg += ": ";
    msg += message;
  };

  Excep(const Excep &other) noexcept : msg{other.msg} {}

  const char *
  what() const _GLIBCXX_USE_NOEXCEPT override
  {
    return msg.c_str();
  };

 private:
  std::string msg;
};

#define UTILS_RAISE(msg)                                         \
  std::stringstream utilscpp_excep_located_s;                    \
  utilscpp_excep_located_s << __func__ << ": " << msg; \
  throw utils::Excep(utilscpp_excep_located_s.str());

#else
#define UTILS_RAISE(msg) lFatal(msg)
#endif

#define UTILS_RAISE_LOCATED UTILS_RAISE("in " << __func__)

#define UTILS_RAISE_IF(v)                 \
  if (v) {                                \
    UTILS_RAISE(#v " should not be ture") \
  }

#define UTILS_RAISE_UNLESS(v) UTILS_RAISE_IF(!(v))

#define UTILS_RAISE_VERB_IF(v, msg)                \
  if (v) {                                         \
    UTILS_RAISE(#v " should not be ture: " << msg) \
  }

#define UTILS_RAISE_VERB_UNLESS(v, msg) UTILS_RAISE_VERB_IF(!(v), msg)

/**
 * ulimit, enable core dump
 */
void
enable_core_dump() noexcept;

}  // namespace utils
