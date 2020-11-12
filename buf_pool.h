/**
 * Created on: 6/4/16
 *     Author: xing
 */

#pragma once

#include <cstring>
#include <mutex>
#include <functional>

#include "module.h"

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
  Init(size_t size, size_t len) noexcept
  {
    this->size = size;
    this->len = len;
  }

  Ret
  Write(const uint8_t *src, size_t length) noexcept;

  Ret
  Read(uint8_t *dst, size_t &length) const noexcept;

  uint8_t *
  Data() noexcept
  {
    return data;
  }

 private:
  UTILS_READER(size_t, size);
  UTILS_READER(size_t, len);

  static constexpr size_t prefix_len{sizeof(size_t) + sizeof(size_t)};

#ifdef UNIX
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
  UTILS_DISALLOW_COPY_AND_ASSIGN(BufferPool)

  BufferPool(size_t buf_count, size_t buf_size)
      : BufferPool(buf_count, buf_size, "")
  {
  }

  BufferPool(size_t buf_count, size_t buf_size, const std::string &name);

  ~BufferPool() override
  {
    PutStat();
    delete[] mem;
  }

  inline unique_buf
  AllocUnique() noexcept
  {
    return unique_buf{Alloc(), del};
  }

  inline shared_buf
  AllocShared() noexcept
  {
    return shared_buf{Alloc(), del};
  }

  std::string
  ToString() const noexcept override
  {
    return Get_name() + stat.ToString();
  }

 private:
  struct NodeHead {
    NodeHead() = default;

    explicit NodeHead(void *ptr) : next{(NodeHead *)ptr} {}

    NodeHead *next{};
  };

  typedef NodeHead *nodeptr;

  Buffer *
  Alloc() noexcept;

  buf_callback del{[this](Buffer *b) {
    unless(b) { return; }
    ((nodeptr)b)->next = free_mem;
    free_mem = (nodeptr)b;
  }};

  struct Stat {
    inline std::string
    ToString() const noexcept
    {
      UTILS_STR_S(32)
      UTILS_STR_ATTR(total)
      UTILS_STR_ATTR(succ)
      return s;
    }

    uint64_t total{};
    uint64_t succ{};
  } stat{};

  UTILS_READER(size_t, buf_count);
  UTILS_READER(size_t, buf_size);
  uint8_t *mem;
  nodeptr free_mem;
};

/**
 * Concurrent Buffer Pool
 * @note This Pool is thread safe.
 * 	For run-time efficiency, we create this stand alone concurrent buffer
 * pool other than add cocurrent feature into existing Buffer Pool
 */
class CBufferPool : public Module
{
 public:
  UTILS_DISALLOW_COPY_AND_ASSIGN(CBufferPool)

  CBufferPool(size_t buf_count, size_t buf_size)
      : CBufferPool(buf_count, buf_size, "")
  {
  }

  CBufferPool(size_t buf_count, size_t buf_size, const std::string &name);

  ~CBufferPool() override
  {
    PutStat();
    delete[] mem;
  }

  inline unique_buf
  AllocUnique() noexcept
  {
    return unique_buf{Alloc(), del};
  }

  inline shared_buf
  AllocShared() noexcept
  {
    return shared_buf{Alloc(), del};
  }

  std::string
  ToString() const noexcept override
  {
    return Get_name() + stat.ToString();
  }

 private:
  struct NodeHead {
    NodeHead() = default;

    explicit NodeHead(void *ptr) : next{(NodeHead *)ptr} {}

    NodeHead *next{};
  };

  typedef NodeHead *nodeptr;

  Buffer *
  Alloc() noexcept;

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
    ToString() const noexcept
    {
      UTILS_STR_S(32)
      UTILS_STR_ATTR(total)
      UTILS_STR_ATTR(succ)
      return s;
    }

    uint64_t total{};
    uint64_t succ{};
  } stat{};

  UTILS_READER(size_t, buf_count);
  UTILS_READER(size_t, buf_size);
  uint8_t *mem;
  nodeptr free_mem;
  std::mutex mut{};
};

}  // namespace utils
