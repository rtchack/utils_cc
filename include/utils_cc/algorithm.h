/**
 * Created on: 6/20/17
 *     Author: xing
 */

#pragma once

#include <array>
#include <future>
#include "utils_cc/log.h"
#include "utils_cc/helpers.h"
#include "utils_cc/macro_utils.h"

namespace utils
{
template<typename T>
void
qsort(T *v, int32_t left, int32_t right)
{
  if (left >= right) {
    return;
  }

  auto last = left;
  for (auto i = left + 1; i <= right; ++i) {
    if (v[i] < v[left] && i != ++last) {
      std::swap(v[i], v[last]);
    }
  }
  unless(left == last) { std::swap(v[left], v[last]); }

  if (last - left > 1024) {
    std::async(std::launch::async,
               [v, left, last] { qsort(v, left, last - 1); });
  } else {
    qsort(v, left, last - 1);
  }

  if (right - last > 1024) {
    std::async(std::launch::async,
               [v, last, right] { qsort(v, last + 1, right); });
  } else {
    qsort(v, last + 1, right);
  }
}

template<typename T, size_t N>
void qsort(T (&v)[N])
{
  qsort(v, 0, (int32_t)(N - 1));
};

}  // namespace utils
