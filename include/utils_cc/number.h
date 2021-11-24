/*
 * Created by xing in 2020.
 */
/*
 * Borrowed from WebRTC
 */

#pragma once

#include <utils_cc/macro_utils.h>

namespace ucc
{
/** Mod M add */
template <unsigned long M>  // NOLINT
inline unsigned long
add(unsigned long a, unsigned long b)
{
  UCC_DCHECK(a < M)
  unsigned long t = M - b % M;  // NOLINT
  unsigned long res = a - t;    // NOLINT
  if (t > a) return res + M;
  return res;
}

/** Mod M subtract */
template <unsigned long M>  // NOLINT
inline unsigned long
subtract(unsigned long a, unsigned long b)
{
  UCC_DCHECK(a < M)
  unsigned long sub = b % M;  // NOLINT
  if (a < sub) return M - (sub - a);
  return a - sub;
}

/**
 * Calculates the forward difference between two wrapping numbers.
 *
 * Example:
 * uint8_t x = 253;
 * uint8_t y = 2;
 *
 * forward_diff(x, y) == 5
 *
 *   252   253   254   255    0     1     2     3
 * #################################################
 * |     |  x  |     |     |     |     |  y  |     |
 * #################################################
 *          |----->----->----->----->----->
 *
 * forward_diff(y, x) == 251
 *
 *   252   253   254   255    0     1     2     3
 * #################################################
 * |     |  x  |     |     |     |     |  y  |     |
 * #################################################
 * -->----->                              |----->---
 *
 * If M > 0 then wrapping occurs at M, if M == 0 then wrapping occurs at the
 * largest value representable by T.
 */
template <typename T, T M>
inline typename std::enable_if<(M > 0), T>::type
forward_diff(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  UCC_DCHECK(a < M)
  UCC_DCHECK(b < M)
  return a <= b ? b - a : M - (a - b);
}

template <typename T, T M>
inline typename std::enable_if<(M == 0), T>::type
forward_diff(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  return b - a;
}

template <typename T>
inline T
forward_diff(T a, T b)
{
  return forward_diff<T, 0>(a, b);
}

/**
 * Calculates the reverse difference between two wrapping numbers.
 *
 * Example:
 * uint8_t x = 253;
 * uint8_t y = 2;
 *
 * reverse_diff(y, x) == 5
 *
 *   252   253   254   255    0     1     2     3
 * #################################################
 * |     |  x  |     |     |     |     |  y  |     |
 * #################################################
 *          <-----<-----<-----<-----<-----|
 *
 * reverse_diff(x, y) == 251
 *
 *   252   253   254   255    0     1     2     3
 * #################################################
 * |     |  x  |     |     |     |     |  y  |     |
 * #################################################
 * ---<-----|                             |<-----<--
 *
 * If M > 0 then wrapping occurs at M, if M == 0 then wrapping occurs at the
 * largest value representable by T.
 */
template <typename T, T M>
inline typename std::enable_if<(M > 0), T>::type
reverse_diff(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  UCC_DCHECK(a < M)
  UCC_DCHECK(b < M)
  return b <= a ? a - b : M - (b - a);
}

template <typename T, T M>
inline typename std::enable_if<(M == 0), T>::type
reverse_diff(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  return a - b;
}

template <typename T>
inline T
reverse_diff(T a, T b)
{
  return reverse_diff<T, 0>(a, b);
}

/**
 * Calculates the minimum distance between to wrapping numbers.
 *
 * The minimum distance is defined as min(forward_diff(a, b), reverse_diff(a,
 * b))
 */
template <typename T, T M = 0>
inline T
min_diff(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  return std::min(forward_diff<T, M>(a, b), reverse_diff<T, M>(a, b));
}

/**
 * Test if the sequence number |a| is ahead or at sequence number |b|.
 *
 * If |M| is an even number and the two sequence numbers are at max distance
 * from each other, then the sequence number with the highest value is
 * considered to be ahead.
 */
template <typename T, T M>
inline typename std::enable_if<(M > 0), bool>::type
ahead_or_at(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  const T maxDist = M / 2;
  if (!(M & 1) && min_diff<T, M>(a, b) == maxDist) return b < a;
  return forward_diff<T, M>(b, a) <= maxDist;
}

template <typename T, T M>
inline typename std::enable_if<(M == 0), bool>::type
ahead_or_at(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  const T maxDist = std::numeric_limits<T>::max() / 2 + T(1);
  if (a - b == maxDist) return b < a;
  return forward_diff(b, a) < maxDist;
}

template <typename T>
inline bool
ahead_or_at(T a, T b)
{
  return ahead_or_at<T, 0>(a, b);
}

/**
 * Test if the sequence number |a| is ahead of sequence number |b|.
 *
 * If |M| is an even number and the two sequence numbers are at max distance
 * from each other, then the sequence number with the highest value is
 * considered to be ahead.
 */
template <typename T, T M = 0>
inline bool
ahead_of(T a, T b)
{
  static_assert(std::is_unsigned<T>::value,
                "Type must be an unsigned integer.");
  return a != b && ahead_or_at<T, M>(a, b);
}

/**
 * Comparator used to compare sequence numbers in a continuous fashion.
 *
 * WARNING! If used to sort sequence numbers of length M then the interval
 *          covered by the sequence numbers may not be larger than floor(M/2).
 */
template <typename T, T M = 0>
struct AscendingSeqNumComp {
  bool
  operator()(T a, T b) const
  {
    return ahead_of<T, M>(a, b);
  }
};

/**
 * Comparator used to compare sequence numbers in a continuous fashion.
 *
 * WARNING! If used to sort sequence numbers of length M then the interval
 *          covered by the sequence numbers may not be larger than floor(M/2).
 */
template <typename T, T M = 0>
struct DescendingSeqNumComp {
  bool
  operator()(T a, T b) const
  {
    return ahead_of<T, M>(b, a);
  }
};

/**
 * A sequence number unwrapper where the first unwrapped value equals the
 * first value being unwrapped.
 */
template <typename T, T M = 0>
class SeqNumUnwrapper
{
  static_assert(
      std::is_unsigned<T>::value &&
          std::numeric_limits<T>::max() < std::numeric_limits<int64_t>::max(),
      "Type unwrapped must be an unsigned integer smaller than int64_t.");

 public:
  SeqNumUnwrapper() = default;

  ~SeqNumUnwrapper() { delete last_value_; }

  int64_t
  unwrap(T value)
  {
    if (!last_value_) {
      last_unwrapped_ = {value};
      last_value_ = new T{};
    } else {
      last_unwrapped_ += forward_diff<T, M>(*last_value_, value);

      if (!ahead_or_at<T, M>(value, *last_value_)) {
        constexpr int64_t kBackwardAdjustment =
            M == 0 ? int64_t{std::numeric_limits<T>::max()} + 1 : M;
        last_unwrapped_ -= kBackwardAdjustment;
      }
    }

    *last_value_ = value;
    return last_unwrapped_;
  }

 private:
  UCC_DISALLOW_COPY_AND_ASSIGN(SeqNumUnwrapper);

  UCC_READER(int64_t, last_unwrapped_){0};
  UCC_DIRECT_READER(T *, last_value_){};
};

}  // namespace ucc