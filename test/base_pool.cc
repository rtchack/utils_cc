/*
 * Created by xing in 2020.
 */

#include <utils_cpp/base_pool.h>
#include <gtest/gtest.h>

namespace test
{
class TmpKlass
{
 public:
  explicit TmpKlass(std::string &&name) : name{std::move(name)} {}

 private:
  UTILS_READER(std::string, name);
};

TEST(Test_base_pool, should_work_in_single_thread)
{
  for (auto be_safe : std::array<bool, 2>{true, false}) {
    utils::BasePool<TmpKlass> pool{4};

    pool.alloc_pooled(std::string{"Pooled"});
    auto unique = pool.alloc_unique(std::string{"Pooled"});
    auto shared = pool.alloc_shared(std::string{"Pooled"});
  }
}

}  // namespace test
