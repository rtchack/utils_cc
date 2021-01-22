/*
 * Created by xing in 2020.
 */

#include <utils_cpp/buffer.h>
#include <gtest/gtest.h>

namespace test
{
TEST(buffer, should_work)
{
  utils::Buffer<1024> buf;
  EXPECT_EQ(0, buf.get_total_data_size());
  EXPECT_EQ(0, buf.get_remain_data_size());

  const auto mem_len = buf.get_capacity();
  auto src_buf = std::make_unique<uint8_t[]>(mem_len);
  memset(src_buf.get(), 1, buf.get_capacity());

  auto tgt_len = mem_len >> 1U;

  EXPECT_EQ(utils::Ret::OK, buf.write(src_buf.get(), tgt_len));
  EXPECT_EQ(tgt_len, buf.get_total_data_size());
  EXPECT_EQ(utils::Ret::OK, buf.write(src_buf.get(), tgt_len));
  EXPECT_EQ(mem_len, buf.get_total_data_size());
  EXPECT_NE(utils::Ret::OK, buf.write(src_buf.get(), 1U));

  auto tgt_buf = std::make_unique<uint8_t[]>(mem_len);

  EXPECT_EQ(utils::Ret::OK, buf.read(src_buf.get(), tgt_len));
  EXPECT_EQ(mem_len >> 1U, tgt_len);
  EXPECT_EQ(mem_len >> 1U, buf.get_remain_data_size());

  tgt_len = mem_len;
  EXPECT_EQ(utils::Ret::OK, buf.read(src_buf.get(), tgt_len));
  EXPECT_EQ(mem_len >> 1U, tgt_len);
  EXPECT_EQ(0, buf.get_remain_data_size());
  EXPECT_EQ(mem_len, buf.get_total_data_size());

  tgt_len = 1U;
  EXPECT_NE(utils::Ret::OK, buf.read(src_buf.get(), tgt_len));

  EXPECT_EQ(utils::Ret::OK, buf.write_at(src_buf.get(), mem_len, 0));
  EXPECT_EQ(mem_len, buf.get_remain_data_size());

  tgt_len = mem_len >> 1U;
  EXPECT_EQ(utils::Ret::OK, buf.read(src_buf.get(), tgt_len));
  EXPECT_EQ(mem_len >> 1U, tgt_len);

  tgt_len = mem_len;
  EXPECT_EQ(utils::Ret::OK, buf.read_at(src_buf.get(), tgt_len, 0));
  EXPECT_EQ(mem_len, tgt_len);
}

}  // namespace test