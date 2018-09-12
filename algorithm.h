/**
 * Created on: 6/20/17
 *     Author: xing
 */

#pragma once

#include <array>
#include <future>
#include "log.h"
#include "helpers.h"
#include "macro_utils.h"


namespace utils {

template<typename T>
void QuickSort(T *v, int32_t left, int32_t right) {
  if (left >= right) {
    return;
  }

  auto last = left;
  for (auto i = left + 1; i <= right; ++i) {
    if (v[i] < v[left] && i != ++last) {
      std::swap(v[i], v[last]);
    }
  }
  unless(left == last) {
    std::swap(v[left], v[last]);
  }

  if (last - left > 1024) {
    std::async(std::launch::async,
               [v, left, last] { QuickSort(v, left, last - 1); });
  } else {
    QuickSort(v, left, last - 1);
  }

  if (right - last > 1024) {
    std::async(std::launch::async,
               [v, last, right] { QuickSort(v, last + 1, right); });
  } else {
    QuickSort(v, last + 1, right);
  }
}

template<typename T, size_t N>
void QuickSort(T (&v)[N]) {
  QuickSort(v, 0, (int32_t) (N - 1));
};

}
