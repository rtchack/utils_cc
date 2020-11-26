/*
 *  Created on: Mar 28, 2016
 *      Author: xing
 */

#pragma once

#include <chrono>

#include "utils_cpp/common.h"
#include "utils_cpp/macro_utils.h"

namespace utils
{
class Time final
{
 public:
  typedef std::chrono::steady_clock::time_point Point;
  typedef std::chrono::steady_clock::duration Duration;
  typedef std::chrono::steady_clock::period Period;

  Time() : start{std::chrono::steady_clock::now()} {}

  inline void
  reset() noexcept
  {
    start = std::chrono::steady_clock::now();
  }

  inline Duration
  lap() const noexcept
  {
    return std::chrono::steady_clock::now() - start;
  }

  /**
   * Has time passed duration till now?
   * @param[in] duration - time duration
   * @return OK - time is up, and ts has been updated
   * 		NO - time is not up, and ts stays the same
   */
  Ret
  time_up(const Duration &duration) noexcept;

  /**
   * Has time passed duration till now?
   * @param[in] duration - time duration
   * @return OK - time is up, NO - time is not up
   * @note This call will ALWAYS update the value of timer to now
   */
  Ret
  time_up_and_update(const Duration &duration) noexcept;

  void
  inspect() const noexcept;

#define BASE_TIMER_HELPER_CAST(unit)                                  \
  static inline intmax_t of_##unit(const Duration &du) noexcept       \
  {                                                                   \
    return std::chrono::duration_cast<std::chrono::unit>(du).count(); \
  }

#define BASE_TIMER_HELPER_STATE(unit)               \
  static inline Duration unit(int64_t val) noexcept \
  {                                                 \
    return std::chrono::unit(val);                  \
  }

#define BASE_TIMER_HELPER(unit) \
  BASE_TIMER_HELPER_STATE(unit) \
  BASE_TIMER_HELPER_CAST(unit)

  BASE_TIMER_HELPER(nanoseconds)

  BASE_TIMER_HELPER(microseconds)

  BASE_TIMER_HELPER(milliseconds)

  BASE_TIMER_HELPER(seconds)

  BASE_TIMER_HELPER(minutes)

  BASE_TIMER_HELPER(hours)

 private:
  UTILS_READER(Point, start);
  Point tmp{};
};

}  // namespace utils
