/**
 * Created by xing on 4/14/16.
 */

#include "utils_cpp/time.h"

namespace utils
{
/**
 * Has time passed seconds seconds from ts to now?
 * @param[in] sec - time interval in seconds
 * @return OK - time is up, and ts has been updated
 * 		NO - time is not up, and ts stays the same
 */
Ret
Time::time_up(const Duration &duration) noexcept
{
  tmp = std::chrono::steady_clock::now();
  if (start + duration <= tmp) {
    start = tmp;
    return Ret::OK;
  }

  return Ret::NO;
};

/**
 * Has time passed nanoseconds nanoseconds from ts to now?
 * @param[in] sec - time interval in seconds
 * @return OK - time is up, NO - time is not up
 * @note This call will ALWAYS update the value of timer to now
 */
Ret
Time::time_up_and_update(const Duration &duration) noexcept
{
  tmp = std::chrono::steady_clock::now();
  if (start + duration <= tmp) {
    start = tmp;
    return Ret::OK;
  }

  start = tmp;
  return Ret::NO;
};

void
Time::inspect() const noexcept
{
  UTILS_STR_S(64)
  s.append("/n/tstart ");
  s.append(std::to_string(of_microseconds(start.time_since_epoch())));
  s.append("/n/ttmp ");
  s.append(std::to_string(of_microseconds(tmp.time_since_epoch())));
  lInf(s);
}

}  // namespace utils
