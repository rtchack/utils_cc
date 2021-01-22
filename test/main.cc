/*
 * Created by xing in 2020.
 */

#include <gtest/gtest.h>
#include <utils_cc/common.h>

int
main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);

  ucc::enable_core_dump();
  UCC_INIT_LOG;

  return RUN_ALL_TESTS();
}
