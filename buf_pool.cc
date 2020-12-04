/**
 * Created on: 6/4/16
 *     Author: xing
 */

#include "utils_cpp/buf_pool.h"

namespace utils
{
Ret
Buffer::write(const uint8_t *src, size_t len) noexcept
{
  if (len > end_ptr - w_ptr) {
    return Ret::E_BUF_OVERFLOW;
  }

  memcpy(w_ptr, src, len);
  w_ptr += len;
  return Ret::OK;
}

Ret
Buffer::write_at(const uint8_t *src, size_t len, size_t offset) noexcept
{
  if (len > cap - offset) {
    return Ret::E_BUF_OVERFLOW;
  }

  w_ptr = data + offset;
  memcpy(w_ptr, src, len);
  w_ptr += len;
  return Ret::OK;
}

Ret
Buffer::read(uint8_t *dst, size_t &len) noexcept
{
  const auto act_len = std::min(len, get_remain_data_size());

  memcpy(dst, r_ptr, act_len);
  r_ptr += act_len;
  return Ret::OK;
}

Ret
Buffer::read_at(uint8_t *dst, size_t &len, size_t offset) noexcept
{
  if (offset > get_total_data_size()) {
    return Ret::E_ARG;
  }

  r_ptr = data + offset;
  const auto act_len = std::min(len, get_remain_data_size());
  memcpy(dst, r_ptr, act_len);
  r_ptr += act_len;
  return Ret::OK;
}

BufferPool::BufferPool(size_t n_buf,
                       size_t buf_capacity,
                       const std::string &name)
    : Module{name}, n_buf{n_buf}, buf_capacity{buf_capacity}
{
  UTILS_RAISE_IF(n_buf <= 0 || buf_capacity < sizeof(nodeptr))

  size_t sz = UTILS_ROUND(buf_capacity + sizeof(Buffer), sizeof(void *));
  mem = new uint8_t[n_buf * sz];
  UTILS_RAISE_UNLESS(mem)

  free_mem = (nodeptr)mem;

  sz /= sizeof(nodeptr);
  nodeptr tmp = free_mem;
  while (--n_buf) {
    tmp->next = tmp + sz;
    tmp = tmp->next;
  }

  tmp->next = nullptr;
}

Buffer *
BufferPool::alloc() noexcept
{
  ++stat.n_total;

  unless(free_mem) { return nullptr; }

  ++stat.n_succ;

  auto b = (Buffer *)free_mem;
  free_mem = free_mem->next;
  b->init(buf_capacity);

  return b;
};

SafeBufferPool::SafeBufferPool(size_t n_buf,
                               size_t buf_capacity,
                               const std::string &name)
    : Module{name}, n_buf{n_buf}, buf_capacity{buf_capacity}
{
  UTILS_RAISE_IF(n_buf <= 0 || buf_capacity < sizeof(nodeptr))

  size_t sz = UTILS_ROUND(buf_capacity + sizeof(Buffer), sizeof(nodeptr));
  mem = new uint8_t[n_buf * sz];
  UTILS_RAISE_UNLESS(mem)

  free_mem = (nodeptr)mem;

  sz /= sizeof(nodeptr);
  nodeptr tmp = free_mem;
  while (--n_buf) {
    tmp->next = tmp + sz;
    tmp = tmp->next;
  }

  tmp->next = nullptr;
}

Buffer *
SafeBufferPool::alloc() noexcept
{
  ++stat.n_total;

  unless(free_mem) { return nullptr; }

  {
    std::lock_guard<std::mutex> bar{mut};
    auto b = (Buffer *)free_mem;
    free_mem = free_mem->next;
    b->init(buf_capacity);
    ++stat.n_succ;
    return b;
  }
};

}  // namespace utils
