/*
 * Created by xing in 2020.
 */

#include <gtest/gtest.h>
#include <utils_cpp/common.h>

int
main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);

  utils::enable_core_dump();
  UTILS_INIT_LOG;

  return RUN_ALL_TESTS();
}
