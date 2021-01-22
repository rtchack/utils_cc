/*
 * Created by xing in 2020.
 */

#include <gtest/gtest.h>
#include <utils_cc/number.h>

#include <map>

using namespace ucc;

namespace test
{
TEST(ahead_or_at, should_work)
{
  for (auto &&p : std::map<uint8_t, uint8_t>{
           {5U, 4U},
           {5U, 5U},
           {0U, 5U},
       }) {
    const auto ret = ahead_or_at<uint8_t, 8U>(p.first, p.second);
    EXPECT_TRUE(ret);
  }

  for (auto &&p : std::map<uint8_t, uint8_t>{
           {2U, 4U},
       }) {
    const auto ret = ahead_or_at<uint8_t, 8U>(p.first, p.second);
    EXPECT_FALSE(ret);
  }

  for (auto &&p : std::map<uint32_t, uint32_t>{
           {UINT32_MAX, UINT32_MAX - 1U},
           {0U, 0U},
           {0U, UINT32_MAX},
           {0U, UINT32_MAX - 1},
       }) {
    EXPECT_TRUE(ahead_or_at(p.first, p.second));
  }

  for (auto &&p : std::map<uint32_t, uint32_t>{
           {UINT32_MAX, 0U},
           {UINT32_MAX, UINT32_MAX},
       }) {
    EXPECT_FALSE(ahead_or_at(p.first, p.second));
  }
}

TEST(SeqNumUnwrapper, should_work)
{
  {
    constexpr uint8_t M{14U};
    SeqNumUnwrapper<uint8_t, M> unwrapper{};

    for (uint8_t round = 0; round != 3U; ++round) {
      for (uint8_t i = 0; i != M; ++i) {
        const auto seq = unwrapper.unwrap(i);
        EXPECT_EQ(seq, round * M + i);
      }
    }

    const auto last_v = *unwrapper.get_last_value_();
    const auto last_unwrapped = unwrapper.get_last_unwrapped_();
    EXPECT_EQ(last_unwrapped - 1, unwrapper.unwrap(last_v - 1));
    EXPECT_EQ(last_unwrapped - 2, unwrapper.unwrap(last_v - 2));
  }

  {
    SeqNumUnwrapper<uint8_t> unwrapper{};

    for (uint16_t round = 0; round != 3U; ++round) {
      for (uint16_t i = 0; i <= UINT8_MAX; ++i) {
        const auto seq = unwrapper.unwrap((uint8_t)i);
        EXPECT_EQ(seq, round * (UINT8_MAX + 1U) + i);
      }
    }

    const auto last_v = *unwrapper.get_last_value_();
    const auto last_unwrapped = unwrapper.get_last_unwrapped_();
    EXPECT_EQ(last_unwrapped - 1, unwrapper.unwrap(last_v - 1));
    EXPECT_EQ(last_unwrapped - 2, unwrapper.unwrap(last_v - 2));
  }
}
}  // namespace test