/*
 * Created by galaxyhuang in 2021.
 */

#include <gtest/gtest.h>
#include <utils_c/checksum.h>
#include <utils_cc/common.h>
#include <utils_cc/helpers.h>

namespace test
{
template<size_t N>
void gen_rand_buf_data(uint8_t (&data)[N])
{
  size_t i = N % 4 ? N % 4 : 4;
  for (size_t j = 0; j < i; ++j) {
    data[j] = (uint8_t)rand();
  }

  for (; i < N; i += 4) {
    *(uint32_t *)(data + i) = (uint32_t)rand();
  }

  UCC_DCHECK(i == N);
}

template<size_t N>
void
gen_array_checksum(uint16_t &checksum, uint8_t (&data)[N])
{
  checksum = gen_oc_checksum(checksum, data, N);
}

TEST(checksum, should_work)
{
  uint8_t buf1[1];
  uint8_t buf2[11];
  uint8_t buf3[100];
  uint8_t buf4[1001];

  gen_rand_buf_data(buf1);
  gen_rand_buf_data(buf2);
  gen_rand_buf_data(buf3);
  gen_rand_buf_data(buf4);

  auto checksum = OC_CHECKSUM_INIT;
  gen_array_checksum(checksum, buf1);
  gen_array_checksum(checksum, buf2);
  gen_array_checksum(checksum, buf3);
  gen_array_checksum(checksum, buf4);
  checksum = finalize_oc_checksum(checksum);

  {
    auto tmp = checksum;
    gen_array_checksum(tmp, buf1);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf2);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_TRUE(verify_oc_checksum(tmp));
  }

  {
    // Not able to detect out of order
    auto tmp = checksum;
    gen_array_checksum(tmp, buf2);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf1);
    EXPECT_TRUE(verify_oc_checksum(tmp));
  }

  {
    // Able to detect truncated
    auto tmp = checksum;
    gen_array_checksum(tmp, buf1);
    EXPECT_FALSE(verify_oc_checksum(tmp));

    checksum = gen_oc_checksum(checksum, buf2, sizeof(buf2) - 1);
    EXPECT_FALSE(verify_oc_checksum(tmp));

    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_FALSE(verify_oc_checksum(tmp));
  }

  {
    // Able to detect odd numbers of bit-reverse
    auto tmp = checksum;

    {
      uint8_t tmp_buf[sizeof(buf1)];
      memcpy(tmp_buf, buf1, sizeof(buf1));
      tmp_buf[0] ^= tmp_buf[0];
      gen_array_checksum(tmp, tmp_buf);
    }
    EXPECT_FALSE(verify_oc_checksum(tmp));

    gen_array_checksum(tmp, buf2);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_FALSE(verify_oc_checksum(tmp));
  }

  {
    // Able to detect even numbers of bit-reverse

    auto tmp = checksum;

    gen_array_checksum(tmp, buf1);

    {
      uint8_t tmp_buf[sizeof(buf2)];
      memcpy(tmp_buf, buf2, sizeof(buf2));
      ASSERT_GT(sizeof(buf2), 4);
      tmp_buf[0] ^= tmp_buf[0];
      tmp_buf[1] ^= tmp_buf[1];
      tmp_buf[2] ^= tmp_buf[2];
      tmp_buf[3] ^= tmp_buf[3];
      gen_array_checksum(tmp, tmp_buf);
    }
    EXPECT_FALSE(verify_oc_checksum(tmp));

    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_FALSE(verify_oc_checksum(tmp));
  }

  {
    // Able to detect even numbers of bit-reverse
    auto tmp = checksum;

    {
      uint8_t tmp_buf[sizeof(buf1)];
      memcpy(tmp_buf, buf1, sizeof(buf1));
      tmp_buf[0] ^= tmp_buf[0];
      gen_array_checksum(tmp, tmp_buf);
    }
    EXPECT_FALSE(verify_oc_checksum(tmp));

    {
      uint8_t tmp_buf[sizeof(buf2)];
      memcpy(tmp_buf, buf2, sizeof(buf2));
      tmp_buf[0] ^= tmp_buf[0];
      gen_array_checksum(tmp, tmp_buf);
    }
    EXPECT_FALSE(verify_oc_checksum(tmp));

    gen_array_checksum(tmp, buf3);
    EXPECT_FALSE(verify_oc_checksum(tmp));
    gen_array_checksum(tmp, buf4);
    EXPECT_FALSE(verify_oc_checksum(tmp));
  }
}

}  // namespace test