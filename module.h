/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#pragma once

#include <string>
#include <memory>

#include "common.h"
#include "log.h"
#include "string_utils.h"

namespace utils {

class Module : public StringAble {
 public:
  Module() = default;

  explicit Module(const std::string &name) : name{name} {}

  virtual ~Module() = default;

  virtual void Inspect() const { cInf(ToString()) }

  virtual void PutStat() const { cInf(ToString()) }

  virtual std::string ToString() const {
    return name + ": ToString has not been implemented.";
  }

 private:
 UTILS_ACCESSOR(std::string, name){""};
};

}

