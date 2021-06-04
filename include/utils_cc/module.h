/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#pragma once

#include <string>
#include <memory>

#include "utils_cc/common.h"
#include "utils_cc/log.h"
#include "utils_cc/string_utils.h"

namespace ucc
{
class Module : public StringAble
{
 public:
  Module() = default;

  explicit Module(std::string &&name) : name{std::move(name)} {}

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
    UNIMPLEMENTED
  }

 private:
  UCC_ACCESSOR(std::string, name){""};
};

}  // namespace ucc
