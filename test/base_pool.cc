/*
 * Created by xing in 2020.
 */

#include <array>

#include <utils_cc/base_pool.h>
#include <gtest/gtest.h>

namespace test
{
class TmpKlass {
 public:
  explicit TmpKlass(std::string &&name) : name{std::move(name)} {}

 private:
 UCC_READER(std::string, name);
};

TEST(base_pool, should_work_in_single_thread)
{
  for (auto be_safe: std::array<bool, 2>{/*true,*/ false}) {
    ucc::BasePool<TmpKlass> pool{3, "", be_safe};

    auto &stat = pool.get_stat();

    auto unique = pool.alloc_unique(std::string{"unique"});
    EXPECT_EQ(stat.n_allocated, 1);
    EXPECT_TRUE(unique);

    auto shared = pool.alloc_shared(std::string{"shared"});
    EXPECT_EQ(stat.n_allocated, 2);
    EXPECT_TRUE(shared);

    {
      auto shared_1 = pool.alloc_shared(std::string{"shared"});
      EXPECT_EQ(stat.n_allocated, 3);
      EXPECT_TRUE(shared_1);

      EXPECT_FALSE(pool.alloc_shared(std::string{"shared"}));
      EXPECT_FALSE(pool.alloc_unique(std::string{"unique"}));
      EXPECT_EQ(stat.n_alloc_failure, 2);
      EXPECT_EQ(stat.n_allocated, 3);
    }

    EXPECT_EQ(stat.n_allocated, 2);

    EXPECT_TRUE(pool.alloc_unique(std::string{"unique"}));
    EXPECT_EQ(stat.n_allocated, 2);
  }
}

}  // namespace test
