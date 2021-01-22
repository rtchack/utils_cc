/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#pragma once

#include <string>
#include <stdexcept>
#include <sstream>

#ifdef OS_UNIX
#include <unistd.h>
#endif

#include "utils_cc/macro_utils.h"

#ifndef ENABLE_CPP_EXCEPTION
#include "utils_cc/log.h"
#endif

namespace ucc
{
/**
 * return value
 */
enum class Ret {
#define UCC_RET_ERROR 0x8000;

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
to_s(Ret value) noexcept;

constexpr bool
failed(Ret ret) noexcept
{
  return ret > Ret::NO;
}

constexpr bool
succeed(Ret ret) noexcept
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

  Excep(const std::string &message) : msg{std::to_s(getpid()) + ": " + message}
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

#define UCC_RAISE(msg)                                 \
  std::stringstream utils_cc_excep_located_s;          \
  utils_cc_excep_located_s << __func__ << ": " << msg; \
  throw utils::Excep(utils_cc_excep_located_s.str());

#else
#define UCC_RAISE(msg) lFatal(msg)
#endif

#define UCC_RAISE_IF(v)                 \
  if (v) {                              \
    UCC_RAISE(#v " should not be ture") \
  }

#define UCC_RAISE_UNLESS(v) UCC_RAISE_IF(!(v))

#define UCC_RAISE_VERB_IF(v, msg)                \
  if (v) {                                       \
    UCC_RAISE(#v " should not be ture: " << msg) \
  }

#define UCC_RAISE_VERB_UNLESS(v, msg) UCC_RAISE_VERB_IF(!(v), msg)

#define NOT_IMPLEMENTED UCC_RAISE("NOT IMPLEMENTED");
#define UNREACHABLE UCC_RAISE("UNREACHABLE");

/**
 * ulimit, enable core dump
 */
void
enable_core_dump() noexcept;

}  // namespace ucc
