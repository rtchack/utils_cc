/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#pragma once

#include <string>
#include <memory>

#include "utils_cpp/common.h"
#include "utils_cpp/log.h"
#include "utils_cpp/string_utils.h"

namespace utils
{
class Module : public StringAble
{
 public:
  Module() = default;

  explicit Module(const std::string &name) : name{name} {}

  virtual ~Module() = default;

  virtual void
  inspect() const
  {
    mInf(to_s())
  }

  virtual void
  put_stat() const
  {
    mInf(to_s())
  }

  virtual std::string
  to_s() const
  {
    NOT_IMPLEMENTED
  }

 private:
  UTILS_ACCESSOR(std::string, name){""};
};

}  // namespace utils
