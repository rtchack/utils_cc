/*
 * Created by galaxyhuang in 2021.
 */

#include <gtest/gtest.h>
#include <utils_c/c_checksum.h>
#include <utils_cc/common.h>
#include <utils_cc/helpers.h>

namespace test
{
template<size_t N>
void
gen_rand_buf_data(uint8_t (&data)[N])
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

template <size_t N>
void gen_array_checksum(uint16_t &checksum, uint8_t (&data)[N])
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

  gen_array_checksum(checksum, buf1);
  gen_array_checksum(checksum, buf2);
  gen_array_checksum(checksum, buf3);
  gen_array_checksum(checksum, buf4);
  EXPECT_TRUE(verify_oc_checksum(checksum));
}

}  // namespace test