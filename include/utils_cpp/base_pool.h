/**
 * Created on: 8/27/16
 *     Author: xing
 */

#pragma once

#include <atomic>
#include <mutex>
#include <functional>

#include "utils_cpp/common.h"
#include "utils_cpp/module.h"

namespace utils
{
/**
 * Pointer wrapper
 * @note Smart pointer in STD is prefered usually,
 * 		This class if for situation when STD smart pointer is not
 * available, e.g. in lambda capture
 */
template <typename T>
class PooledPtr : public StringAble
{
 public:
  PooledPtr() = default;

  PooledPtr(T *ptr, std::function<void(T *)> &&del)
      : ptr{ptr}, del{std::move(del)}
  {
  }

  ~PooledPtr() { recycle(); }

  PooledPtr &
  operator=(const PooledPtr &other)
  {
    auto &ot = const_cast<PooledPtr &>(other);
    recycle();
    ptr = ot.ptr;
    del = ot.del;
    ot.ptr = nullptr;
    ot.del = nullptr;
    return *this;
  }

  PooledPtr &
  operator=(PooledPtr &&other) noexcept
  {
    recycle();
    ptr = other.ptr;
    del = other.del;
    other.ptr = nullptr;
    other.del = nullptr;
    return *this;
  }

  T *
  get() const noexcept
  {
    return ptr;
  }

  std::string
  to_s() const noexcept override
  {
    std::stringstream s;
    s << "ptr:" << ptr;
    return s.str();
  }

 private:
  inline void
  recycle() noexcept
  {
    if (ptr && del) {
      del(ptr);
      ptr = nullptr;
    }
  }

  T *ptr{};
  std::function<void(T *)> del{};
};

/**
 * BasePool
 */
template <typename T>
class BasePool : public Module
{
 public:
  explicit BasePool(size_t capacity) : BasePool(capacity, "", false) {}

  BasePool(size_t capacity, std::string &&name, bool be_thread_safe)
      : Module{std::move(name)}, capacity{capacity}
  {
    UTILS_RAISE_IF(capacity <= 0)

    if (be_thread_safe) {
      mut = new std::mutex{};
    }

    mem = new uint8_t[capacity * sizeof(Node)];
    free_nodes = (Node *)mem;

    auto tmp = free_nodes;
    while (--capacity) {
      tmp->next = tmp + 1;
      tmp = tmp->next;
    }

    tmp->next = nullptr;
  }

  ~BasePool() override
  {
    if (mut) {
      std::lock_guard<std::mutex> lock{*mut};
      delete[] mem;
    } else {
      delete[] mem;
    }

    delete mut;
  }

  /**
   * get a unique_ptr to a T instance
   */
  template <typename... Args>
  inline std::unique_ptr<T, std::function<void(T *)>>
  alloc_unique(Args &&... args) noexcept
  {
    return std::unique_ptr<T, std::function<void(T *)>>{
        alloc(std::forward<Args>(args)...), [this](T *t) { dealloc(t); }};
  }

  /**
   * get a shared_ptr to a T instance
   */
  template <typename... Args>
  inline std::shared_ptr<T>
  alloc_shared(Args &&... args) noexcept
  {
    unless(mut) { return nullptr; }

    return std::shared_ptr<T>{alloc(std::forward<Args>(args)...),
                              [this](T *t) { dealloc(t); }};
  }

  template <typename... Args>
  PooledPtr<T>
  alloc_pooled(Args &&... args) noexcept
  {
    return PooledPtr<T>{
        alloc(std::forward<Args>(args)...),
        static_cast<std::function<void(T *)>>([this](T *t) { dealloc(t); })};
  }

  std::string
  to_s() const noexcept override
  {
    return get_name() + stat.to_s();
  }

 private:
  /**
   * Node for internal memory link
   */
  struct Node {
    Node *next{};
    uint8_t mem[sizeof(T)];
  };

  template <typename... Args>
  T *
  borrow_item(Args &&... args)
  {
    unless(free_nodes) { return nullptr; }
    auto t = new (free_nodes->mem) T{std::forward<Args>(args)...};
    free_nodes = free_nodes->next;
    ++stat.n_succ;
    ++stat.n_allocated;
    return t;
  }

  /**
   * Alloc for type T
   */
  template <typename... Args>
  T *
  alloc(Args &&... args) noexcept
  {
    ++stat.n_total;

    if (mut) {
      std::lock_guard<std::mutex> bar{*mut};
      return borrow_item(std::forward<Args>(args)...);
    } else {
      return borrow_item(std::forward<Args>(args)...);
    }
  };

  void
  return_item(T *t) noexcept
  {
    unless(t) { return; }
    auto node = container_of(t, Node, mem);
    node->next = free_nodes;
    free_nodes = node;
    t->~T();
    ++stat.n_deallocated;
  }

  void
  dealloc(T *t) noexcept
  {
    if (mut) {
      std::lock_guard<std::mutex> bar{*mut};
      return_item(t);
    } else {
      return_item(t);
    }
  }

  struct Stat {
    inline std::string
    to_s() const noexcept
    {
      UTILS_STR_S(64)
      UTILS_STR_ATTR(n_total)
      UTILS_STR_ATTR(n_succ)
      UTILS_STR_ATTR(n_allocated)
      UTILS_STR_ATTR(n_deallocated)
      return s;
    }

    uint64_t n_total{};
    uint64_t n_succ{};
    uint64_t n_allocated{};
    uint64_t n_deallocated{};
  } stat{};

  UTILS_READER(size_t, capacity);
  uint8_t *mem;
  Node *free_nodes;
  std::mutex *mut{};

  UTILS_DISALLOW_COPY_AND_ASSIGN(BasePool)
};

}  // namespace utils
