/**
 * Created on: 4/18/16
 *     Author: xing
 */

#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace utils
{
using unique_mem = std::unique_ptr<uint8_t[]>;

template<typename T, typename... Args>
std::unique_ptr<T>
make_uni_helper(std::false_type, Args &&...args) noexcept
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
std::unique_ptr<T>
make_uni_helper(std::true_type, Args &&...args) noexcept
{
  static_assert(std::extent<T>::value == 0,
                "make_uni<T[N]>() is forbidden, please use make_uni<T[]>().");

  typedef typename std::remove_extent<T>::type U;
  return std::unique_ptr<T>(
      new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

template<typename T, typename... Args>
std::unique_ptr<T>
make_uni(Args &&...args) noexcept
{
  return make_uni_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

// The arraysize(arr) macro returns the # of elements in an array arr.  The
// expression is a compile-time constant, and therefore can be used in defining
// new arrays, for example.  If you use arraysize on a pointer by mistake, you
// will get a compile-time error.  For the technical details, refer to
// http://blogs.msdn.com/b/the1/archive/2004/05/07/128242.aspx.

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template<typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(array) (sizeof(utils::ArraySizeHelper(array)))

template<typename T, size_t N>
T &last(T (&array)[N])
{
  return array[N - 1];
}

}  // namespace utils
