/**
 * Created on: 8/27/16
 *     Author: xing
 */

#pragma once

#include <atomic>
#include <mutex>
#include <functional>

#include "utils_cc/common.h"
#include "utils_cc/module.h"

namespace ucc
{
/**
 * BasePool
 */
template<typename T>
class BasePool : public Module {
 public:
  explicit BasePool(size_t capacity) : BasePool(capacity, "", false) {}

  BasePool(size_t capacity, std::string &&name, bool be_thread_safe)
      : Module{std::move(name)}, capacity{capacity}
  {
    UCC_RAISE_IF(capacity <= 0)

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
  template<typename... Args>
  inline std::unique_ptr<T, std::function<void(T *)>>
  alloc_unique(Args &&...args) noexcept
  {
    return std::unique_ptr<T, std::function<void(T *)>>{
        alloc(std::forward<Args>(args)...), [this](T *t) { dealloc(t); }};
  }

  /**
   * get a shared_ptr to a T instance
   */
  template<typename... Args>
  inline std::shared_ptr<T>
  alloc_shared(Args &&...args) noexcept
  {
    return std::shared_ptr<T>{alloc(std::forward<Args>(args)...),
                              [this](T *t) { dealloc(t); }};
  }

  std::string
  to_s() const noexcept override
  {
    return get_name() + "{" + stat.to_s() + "}";
  }

  size_t
  get_free_node_num() const
  {
    UCC_RAISE_UNLESS(capacity >= stat.n_allocated);
    return capacity - stat.n_allocated;
  }

 private:
  /**
   * Node for internal memory link
   */
  struct Node {
    Node *next{};
    uint8_t mem[sizeof(T)];
  };

  template<typename... Args>
  T *
  borrow_item(Args &&...args)
  {
    unless(free_nodes) {
      ++stat.n_alloc_failure;
      return nullptr;
    }

    auto t = new(free_nodes->mem) T{std::forward<Args>(args)...};
    free_nodes = free_nodes->next;
    ++stat.n_allocated;
    return t;
  }

  /**
   * Alloc for type T
   */
  template<typename... Args>
  T *
  alloc(Args &&...args) noexcept
  {
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
    unless(t) {
      ++stat.n_dealloc_failure;
      return;
    }

    auto node = container_of(t, Node, mem);
    node->next = free_nodes;
    free_nodes = node;
    t->~T();
    --stat.n_allocated;
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
      UCC_STR_S(64)
      UCC_STR_ATTR(n_alloc_failure)
      UCC_STR_ATTR(n_dealloc_failure)
      UCC_STR_ATTR(n_allocated)
      return s;
    }

    uint64_t n_alloc_failure{};
    uint64_t n_dealloc_failure{};
    uint64_t n_allocated{};
  };
 UCC_READER(Stat, stat){};

 UCC_READER(size_t, capacity);
  uint8_t *mem;
  Node *free_nodes;
  std::mutex *mut{};

  UCC_DISALLOW_COPY_AND_ASSIGN(BasePool)
};

}  // namespace ucc
