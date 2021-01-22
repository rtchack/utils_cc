/*
 * Created by galaxyhuang in 2021.
 */

#include <utils_c/bitset.h>
#include <gtest/gtest.h>

#include <memory>
#include <functional>
#include <bitset>

namespace test
{
static constexpr size_t BITSET_SIZE{133};

TEST(c_bitset, should_work)
{
  auto bs = std::unique_ptr<bitset_t, std::function<void(bitset_t *)>>(
      bitset_new(BITSET_SIZE), [](bitset_t *b) { bitset_del(b); });
  std::bitset<BITSET_SIZE> sbs{};

  size_t indexes[]{0, 1, 3, 4, 5, 7, 71, 99, 101, BITSET_SIZE - 1};
  for (const auto i : indexes) {
    bitset_set_at(&*bs, i);
    sbs[i] = true;
  }

  for (size_t i = 0; i != BITSET_SIZE; ++i) {
    EXPECT_EQ(bitset_get_at(&*bs, i), sbs[i]);
  }

  bitset_set_at(&*bs, BITSET_SIZE);
  EXPECT_FALSE(bitset_get_at(&*bs, BITSET_SIZE));

  bitset_clear(&*bs, 4, 4);
  EXPECT_TRUE(bitset_get_at(&*bs, 3));
  EXPECT_FALSE(bitset_get_at(&*bs, 4));
  EXPECT_TRUE(bitset_get_at(&*bs, 5));
  bitset_set_at(&*bs, 4);

  bitset_clear(&*bs, 5, 3);
  EXPECT_TRUE(bitset_get_at(&*bs, 3));
  EXPECT_TRUE(bitset_get_at(&*bs, 4));
  EXPECT_TRUE(bitset_get_at(&*bs, 5));

  bitset_set_at(&*bs, 2);
  bitset_clear(&*bs, 3, 4);
  EXPECT_TRUE(bitset_get_at(&*bs, 2));
  EXPECT_FALSE(bitset_get_at(&*bs, 3));
  EXPECT_FALSE(bitset_get_at(&*bs, 4));
  EXPECT_TRUE(bitset_get_at(&*bs, 5));
  bitset_set_at(&*bs, 3);
  bitset_set_at(&*bs, 4);

  bitset_set_at(&*bs, 6);
  bitset_clear(&*bs, 3, 5);
  EXPECT_TRUE(bitset_get_at(&*bs, 2));
  EXPECT_FALSE(bitset_get_at(&*bs, 3));
  EXPECT_FALSE(bitset_get_at(&*bs, 4));
  EXPECT_FALSE(bitset_get_at(&*bs, 5));
  EXPECT_TRUE(bitset_get_at(&*bs, 6));
  bitset_set_at(&*bs, 3);
  bitset_set_at(&*bs, 4);
  bitset_set_at(&*bs, 5);

  bitset_set_at(&*bs, BITSET_SIZE - 1);
  EXPECT_TRUE(bitset_get_at(&*bs, BITSET_SIZE - 1));
  bitset_clear(&*bs, BITSET_SIZE - 1, BITSET_SIZE - 1);
  EXPECT_FALSE(bitset_get_at(&*bs, BITSET_SIZE - 1));

  for (size_t i = 10; i != 30; ++i) {
    bitset_set_at(&*bs, i);
    EXPECT_TRUE(bitset_get_at(&*bs, i));
  }
  bitset_clear(&*bs, 11, 28);
  for (size_t i = 11; i != 29; ++i) {
    EXPECT_FALSE(bitset_get_at(&*bs, i));
  }
  EXPECT_TRUE(bitset_get_at(&*bs, 10));
  EXPECT_TRUE(bitset_get_at(&*bs, 29));

  bitset_clear_all(&*bs);
  EXPECT_FALSE(bitset_get_at(&*bs, 0));
  EXPECT_FALSE(bitset_get_at(&*bs, 10));
  EXPECT_FALSE(bitset_get_at(&*bs, BITSET_SIZE - 1));
}

TEST(c_inline_bitset, should_work_on_u8)
{
  std::bitset<sizeof(uint64_t)> sbs{};

  uint8_t bsu8;

  size_t indexes[]{1, 3, 4, 5, sizeof(uint8_t) - 1};
  for (const auto i : indexes) {
    inline_bitset_set_at(&bsu8, i);
    sbs[i] = true;
  }

  for (size_t i = 0; i != sizeof(uint8_t); ++i) {
    EXPECT_EQ(inline_bitset_get_at(&bsu8, i), sbs[i]);
  }

  inline_bitset_clear(&bsu8, 4, 4);
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 3));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 4));
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 5));
  inline_bitset_set_at(&bsu8, 4);

  inline_bitset_clear(&bsu8, 5, 3);
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 3));
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 4));
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 5));

  inline_bitset_set_at(&bsu8, 2);
  inline_bitset_clear(&bsu8, 3, 4);
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 2));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 3));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 4));
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 5));
  inline_bitset_set_at(&bsu8, 3);
  inline_bitset_set_at(&bsu8, 4);

  inline_bitset_set_at(&bsu8, 6);
  inline_bitset_clear(&bsu8, 3, 5);
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 2));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 3));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 4));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 5));
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, 6));
  inline_bitset_set_at(&bsu8, 3);
  inline_bitset_set_at(&bsu8, 4);
  inline_bitset_set_at(&bsu8, 5);

  inline_bitset_set_at(&bsu8, sizeof(uint8_t) - 1);
  EXPECT_TRUE(inline_bitset_get_at(&bsu8, sizeof(uint8_t) - 1));
  inline_bitset_clear(&bsu8, sizeof(uint8_t) - 1, sizeof(uint8_t) - 1);
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, sizeof(uint8_t) - 1));

  inline_bitset_clear_all(&bsu8, sizeof(bsu8));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 0));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, 4));
  EXPECT_FALSE(inline_bitset_get_at(&bsu8, sizeof(uint8_t) - 1));
}

}  // namespace test