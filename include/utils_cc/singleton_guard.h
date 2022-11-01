/**
 * Created on: 1/11/17
 *     Author: xing
 */

#pragma once

#include "utils_cc/common.h"

namespace ucc
{
class SingletonGuard {
 public:
  UCC_DISALLOW_COPY_AND_ASSIGN(SingletonGuard)

  SingletonGuard() = delete;

  explicit SingletonGuard(const std::string &name);

  ~SingletonGuard();

 private:
 UCC_READER(int, fd);
};

}  // namespace ucc
