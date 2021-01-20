/*
 * Created by galaxyhuang in 2021.
 */

#include <gtest/gtest.h>
#include <utils_c/number.h>

namespace test
{
TEST(seq_num_unwrapper, should_work)
{
  {
    uint16_t M{0x1000};
    seq_num_unwrapper_t unwrapper_u12 = SEQ_NUM_UNWRAPPER_INIT;

    for (uint16_t i = M - 10; i != M; ++i) {
      const auto seq = seq_num_unwrapper_on_uint12(&unwrapper_u12, (uint16_t)i);
      EXPECT_EQ(seq, i);
    }

    for (uint16_t i = 1; i != 10; ++i) {
      const auto seq = seq_num_unwrapper_on_uint12(&unwrapper_u12, (uint16_t)i);
      EXPECT_EQ(seq, M + i);
    }

    EXPECT_EQ(M - 1, seq_num_unwrapper_on_uint12(&unwrapper_u12, M - 1));
    EXPECT_EQ(M + 1, seq_num_unwrapper_on_uint12(&unwrapper_u12, 1));
    EXPECT_EQ(M + (M >> 1U),
              seq_num_unwrapper_on_uint12(&unwrapper_u12, M >> 1U));
    EXPECT_EQ((M << 1U) - 1,
              seq_num_unwrapper_on_uint12(&unwrapper_u12, M - 1));
    EXPECT_EQ((M << 1U) + 1, seq_num_unwrapper_on_uint12(&unwrapper_u12, 1));
    EXPECT_EQ((M << 1U) + (M >> 1U),
              seq_num_unwrapper_on_uint12(&unwrapper_u12, M >> 1U));
    EXPECT_EQ((M << 1U), seq_num_unwrapper_on_uint12(&unwrapper_u12, 0));
  }

  {
    seq_num_unwrapper_t unwrapper_u16 = SEQ_NUM_UNWRAPPER_INIT;

    for (uint16_t i = UINT16_MAX - 10; i != 0; ++i) {
      const auto seq = seq_num_unwrapper_on_uint16(&unwrapper_u16, (uint16_t)i);
      EXPECT_EQ(seq, i);
    }

    for (uint16_t i = 1; i != 10; ++i) {
      const auto seq = seq_num_unwrapper_on_uint16(&unwrapper_u16, (uint16_t)i);
      EXPECT_EQ(seq, UINT16_MAX + 1 + i);
    }

    const auto last_v = (uint16_t)unwrapper_u16.last_value;
    const auto last_unwrapped = unwrapper_u16.last_unwrapped;
    EXPECT_EQ(last_unwrapped - 10,
              seq_num_unwrapper_on_uint16(&unwrapper_u16, last_v - 10));
    EXPECT_EQ(last_unwrapped - 20,
              seq_num_unwrapper_on_uint16(&unwrapper_u16, last_v - 20));

    EXPECT_EQ(UINT16_MAX + 1 + 1,
              seq_num_unwrapper_on_uint16(&unwrapper_u16, 1));
    EXPECT_EQ(
        UINT16_MAX + 1 + (UINT16_MAX >> 1U) + 1,
        seq_num_unwrapper_on_uint16(&unwrapper_u16, (UINT16_MAX >> 1U) + 1));
    EXPECT_EQ(((UINT16_MAX + 1) << 1U) - 1,
              seq_num_unwrapper_on_uint16(&unwrapper_u16, UINT16_MAX));
  }
}

}  // namespace test