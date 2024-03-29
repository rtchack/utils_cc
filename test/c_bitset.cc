/*
 * Created by xing in 2021.
 */

#include <utils_c/bitset.h>
#include <gtest/gtest.h>
#include <utils_cc/log.h>

#include <memory>
#include <functional>
#include <bitset>
#include <array>
#include <algorithm>

namespace test
{
TEST(c_bitset, bitset_attach_should_work)
{
  uint8_t buf[12];
  bitset_t bs;
  bitset_attach(&bs, buf, sizeof(buf));
  std::bitset<sizeof(buf) << 3> sbs{};

  const size_t indexes[]{0, 1, 3, 4, 5, 7, 71, 99, 101, (sizeof(buf) << 3) - 1};
  for (const auto i: indexes) {
    bitset_set(&bs, i);
    sbs[i] = true;
  }

  for (size_t i = 0; i != sizeof(buf) << 3; ++i) {
    EXPECT_EQ(bitset_get(&bs, i), sbs[i]);
  }
}

TEST(c_bitset, bitset_all_should_work)
{
  const int lens[]{1, 7, 8, 13, 15, 16, 23, 24, 25};
  for (auto n: lens) {
    auto bs = std::unique_ptr<bitset_t, std::function<void(bitset_t *)>>(
        bitset_new(n), [](bitset_t *b) { bitset_del(b); });
    EXPECT_FALSE(bitset_any(&*bs));
    EXPECT_FALSE(bitset_all(&*bs));
    for (int i = 0; i != n; ++i) {
      bitset_set(&*bs, i);
    }
    EXPECT_TRUE(bitset_any(&*bs));
    EXPECT_TRUE(bitset_all(&*bs));
    bitset_clear(&*bs, 0, std::max(0, n - 2));
    EXPECT_FALSE(bitset_all(&*bs));
  }
}

TEST(c_bitset, should_work)
{
  static constexpr size_t BITSET_SIZE{133};
  auto bs =
      std::unique_ptr<bitset_t, std::function<void(bitset_t *)>>(
          bitset_new(BITSET_SIZE),
          [](bitset_t *b) { bitset_del(b); });
  std::bitset<BITSET_SIZE> sbs{};

  const size_t indexes[]{0, 1, 3, 4, 5, 7, 71, 99, 101, BITSET_SIZE - 1};
  for (const auto i: indexes) {
    bitset_set(&*bs, i);
    sbs[i] = true;
  }

  EXPECT_EQ(sbs.count(), bitset_num(&*bs));

  for (size_t i = 0; i != BITSET_SIZE; ++i) {
    EXPECT_EQ(bitset_get(&*bs, i), sbs[i]);
  }

  bitset_set(&*bs, BITSET_SIZE);
  EXPECT_FALSE(bitset_get(&*bs, BITSET_SIZE));
  EXPECT_EQ(sbs.count(), bitset_num(&*bs));

  bitset_clear(&*bs, 4, 4);
  EXPECT_TRUE(bitset_get(&*bs, 3));
  EXPECT_FALSE(bitset_get(&*bs, 4));
  EXPECT_TRUE(bitset_get(&*bs, 5));
  EXPECT_EQ(sbs.count() - 1, bitset_num(&*bs));
  bitset_set(&*bs, 4);
  EXPECT_EQ(sbs.count(), bitset_num(&*bs));

  bitset_set(&*bs, 2);
  EXPECT_EQ(sbs.count() + 1, bitset_num(&*bs));
  bitset_clear(&*bs, 3, 4);
  EXPECT_TRUE(bitset_get(&*bs, 2));
  EXPECT_FALSE(bitset_get(&*bs, 3));
  EXPECT_FALSE(bitset_get(&*bs, 4));
  EXPECT_TRUE(bitset_get(&*bs, 5));
  EXPECT_EQ(sbs.count() - 1, bitset_num(&*bs));
  bitset_set(&*bs, 3);
  bitset_set(&*bs, 4);
  EXPECT_EQ(sbs.count() + 1, bitset_num(&*bs));

  bitset_set(&*bs, 6);
  EXPECT_EQ(sbs.count() + 2, bitset_num(&*bs));

  bitset_clear(&*bs, 3, 5);
  EXPECT_TRUE(bitset_get(&*bs, 2));
  EXPECT_FALSE(bitset_get(&*bs, 3));
  EXPECT_FALSE(bitset_get(&*bs, 4));
  EXPECT_FALSE(bitset_get(&*bs, 5));
  EXPECT_TRUE(bitset_get(&*bs, 6));
  EXPECT_EQ(sbs.count() - 1, bitset_num(&*bs));
  bitset_set(&*bs, 3);
  bitset_set(&*bs, 4);
  bitset_set(&*bs, 5);
  EXPECT_EQ(sbs.count() + 2, bitset_num(&*bs));

  bitset_set(&*bs, BITSET_SIZE - 1);
  EXPECT_TRUE(bitset_get(&*bs, BITSET_SIZE - 1));
  bitset_clear(&*bs, BITSET_SIZE - 1, BITSET_SIZE - 1);
  EXPECT_FALSE(bitset_get(&*bs, BITSET_SIZE - 1));
  EXPECT_EQ(sbs.count() + 1, bitset_num(&*bs));

  for (size_t i = 10; i != 30; ++i) {
    bitset_set(&*bs, i);
    EXPECT_TRUE(bitset_get(&*bs, i));
  }
  bitset_clear(&*bs, 11, 28);
  EXPECT_TRUE(bitset_any(&*bs));
  EXPECT_FALSE(bitset_all(&*bs));

  for (size_t i = 11; i != 29; ++i) {
    EXPECT_FALSE(bitset_get(&*bs, i));
  }
  EXPECT_TRUE(bitset_get(&*bs, 10));
  EXPECT_TRUE(bitset_get(&*bs, 29));

  bitset_clear_all(&*bs);
  EXPECT_FALSE(bitset_get(&*bs, 0));
  EXPECT_FALSE(bitset_get(&*bs, 10));
  EXPECT_FALSE(bitset_get(&*bs, BITSET_SIZE - 1));
  EXPECT_EQ(0, bitset_num(&*bs));
  EXPECT_FALSE(bitset_any(&*bs));
  EXPECT_FALSE(bitset_all(&*bs));

  bitset_set(&*bs, 1);
  bitset_set(&*bs, bs->len - 3);
  bitset_set(&*bs, bs->len - 2);
  bitset_clear(&*bs, bs->len - 2, 1);
  EXPECT_FALSE(bitset_get(&*bs, 0));
  EXPECT_FALSE(bitset_get(&*bs, 1));
  EXPECT_FALSE(bitset_get(&*bs, bs->len - 2));
  EXPECT_FALSE(bitset_get(&*bs, bs->len - 1));
  EXPECT_TRUE(bitset_get(&*bs, bs->len - 3));
}

TEST(c_inline_bitset, should_work_on_u8)
{
  std::bitset<sizeof(uint64_t)> sbs{};

  uint8_t bsu8;

  const size_t indexes[]{1, 3, 4, 5, sizeof(uint8_t) - 1};
  for (const auto i: indexes) {
    uint8_bitset_set(&bsu8, i);
    sbs[i] = true;
  }

  for (size_t i = 0; i != sizeof(uint8_t); ++i) {
    EXPECT_EQ(uint8_bitset_get(&bsu8, i), sbs[i]);
  }

  uint8_bitset_clear(&bsu8, 4, 4);
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 3));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 4));
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 5));
  uint8_bitset_set(&bsu8, 4);

  uint8_bitset_set(&bsu8, 2);
  uint8_bitset_clear(&bsu8, 3, 4);
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 2));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 3));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 4));
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 5));
  uint8_bitset_set(&bsu8, 3);
  uint8_bitset_set(&bsu8, 4);

  uint8_bitset_set(&bsu8, 6);
  uint8_bitset_clear(&bsu8, 3, 5);
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 2));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 3));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 4));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 5));
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 6));
  uint8_bitset_set(&bsu8, 3);
  uint8_bitset_set(&bsu8, 4);
  uint8_bitset_set(&bsu8, 5);

  uint8_bitset_set(&bsu8, sizeof(uint8_t) - 1);
  EXPECT_TRUE(uint8_bitset_get(&bsu8, sizeof(uint8_t) - 1));
  uint8_bitset_clear(&bsu8, sizeof(uint8_t) - 1, sizeof(uint8_t) - 1);
  EXPECT_FALSE(uint8_bitset_get(&bsu8, sizeof(uint8_t) - 1));

  uint8_bitset_clear_all(&bsu8);
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 0));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 4));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, sizeof(uint8_t) - 1));

  uint8_bitset_set(&bsu8, 1);
  uint8_bitset_set(&bsu8, 5);
  uint8_bitset_set(&bsu8, 6);
  uint8_bitset_clear(&bsu8, 6, 1);
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 0));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 1));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 6));
  EXPECT_FALSE(uint8_bitset_get(&bsu8, 7));
  EXPECT_TRUE(uint8_bitset_get(&bsu8, 5));
}

}  // namespace test
