/**
 * Created on: 6/4/16
 *     Author: xing
 */

#pragma once

#include <cstring>
#include <mutex>
#include <functional>

#include "utils_cpp/module.h"

namespace utils
{
/**
 * Buffer
 */
class Buffer
{
 public:
  Buffer() = delete;

  inline void
  init(size_t capacity) noexcept
  {
    cap = capacity;
    reset();
  }

  void
  reset() noexcept
  {
    w_ptr = data;
    r_ptr = data;
    end_ptr = data + cap;
  }

  Ret
  write(const uint8_t *src, size_t len) noexcept;

  Ret
  write_at(const uint8_t *src, size_t len, size_t offset) noexcept;

  Ret
  read(uint8_t *dst, size_t &len) noexcept;

  Ret
  read_at(uint8_t *dst, size_t &len, size_t offset) noexcept;

  void
  inc_offset(size_t n) noexcept
  {
    if (!r_ptr) {
      r_ptr = data + n;
    } else {
      r_ptr += n;
    }
  }

  size_t
  get_remain_data_size() const noexcept
  {
    return w_ptr - r_ptr;
  }

  size_t
  get_total_data_size() const noexcept
  {
    return w_ptr - data;
  }

 private:
  UTILS_READER(size_t, cap){};

  uint8_t *w_ptr;    // write pointer
  uint8_t *r_ptr;    // read pointer
  uint8_t *end_ptr;  // end pointer

  static constexpr size_t prefix_len{sizeof(size_t) + sizeof(size_t)};

#ifdef OS_UNIX
  uint8_t pad[UTILS_ROUND(prefix_len, sizeof(void *)) - prefix_len];
#endif
  uint8_t data[sizeof(void *)];
};

typedef std::function<void(Buffer *)> buf_callback;
typedef std::unique_ptr<Buffer, buf_callback> unique_buf;
typedef std::shared_ptr<Buffer> shared_buf;

/**
 * Buffer Pool
 * @note Not thread safe. Try CBufferPool in multi-thread circumstances
 */
class BufferPool : public Module
{
 public:
  BufferPool(size_t n_buf, size_t buf_capacity)
      : BufferPool(n_buf, buf_capacity, "")
  {
  }

  BufferPool(size_t n_buf, size_t buf_capacity, const std::string &name);

  ~BufferPool() override { delete[] mem; }

  inline unique_buf
  alloc_unique() noexcept
  {
    return unique_buf{alloc(), del};
  }

  inline shared_buf
  alloc_shared() noexcept
  {
    return shared_buf{alloc(), del};
  }

  std::string
  to_s() const noexcept override
  {
    return get_name() + stat.to_s();
  }

 private:
  UTILS_DISALLOW_COPY_AND_ASSIGN(BufferPool)

  struct NodeHead {
    NodeHead() = default;

    explicit NodeHead(void *ptr) : next{(NodeHead *)ptr} {}

    NodeHead *next{};
  };

  typedef NodeHead *nodeptr;

  Buffer *
  alloc() noexcept;

  buf_callback del{[this](Buffer *b) {
    unless(b) { return; }
    ((nodeptr)b)->next = free_mem;
    free_mem = (nodeptr)b;
  }};

  struct Stat {
    inline std::string
    to_s() const noexcept
    {
      UTILS_STR_S(32)
      UTILS_STR_ATTR(n_total)
      UTILS_STR_ATTR(n_succ)
      return s;
    }

    uint64_t n_total{};
    uint64_t n_succ{};
  } stat{};

  UTILS_READER(size_t, n_buf);
  UTILS_READER(size_t, buf_capacity);
  uint8_t *mem;
  nodeptr free_mem;
};

/**
 * Multi-thread safe Buffer Pool
 * @note This Pool is thread safe.
 * 	For run-time efficiency, we create this stand alone concurrent buffer
 *      pool other than add concurrent feature into existing Buffer Pool
 */
class SafeBufferPool : public Module
{
 public:
  SafeBufferPool(size_t n_buf, size_t buf_capacity)
      : SafeBufferPool(n_buf, buf_capacity, "")
  {
  }

  SafeBufferPool(size_t n_buf, size_t buf_capacity, const std::string &name);

  ~SafeBufferPool() override { delete[] mem; }

  inline unique_buf
  alloc_unique() noexcept
  {
    return unique_buf{alloc(), del};
  }

  inline shared_buf
  alloc_shared() noexcept
  {
    return shared_buf{alloc(), del};
  }

  std::string
  to_s() const noexcept override
  {
    return get_name() + stat.to_s();
  }

 private:
  UTILS_DISALLOW_COPY_AND_ASSIGN(SafeBufferPool)

  struct NodeHead {
    NodeHead() = default;

    explicit NodeHead(void *ptr) : next{(NodeHead *)ptr} {}

    NodeHead *next{};
  };

  typedef NodeHead *nodeptr;

  Buffer *
  alloc() noexcept;

  buf_callback del{[this](Buffer *b) {
    unless(b) { return; }
    {
      std::lock_guard<std::mutex> bar{mut};
      ((nodeptr)b)->next = free_mem;
      free_mem = (nodeptr)b;
    }
  }};

  struct Stat {
    inline std::string
    to_s() const noexcept
    {
      UTILS_STR_S(32)
      UTILS_STR_ATTR(n_total)
      UTILS_STR_ATTR(n_succ)
      return s;
    }

    uint64_t n_total{};
    uint64_t n_succ{};
  } stat{};

  UTILS_READER(size_t, n_buf);
  UTILS_READER(size_t, buf_capacity);
  uint8_t *mem;
  nodeptr free_mem;
  std::mutex mut{};
};

}  // namespace utils
