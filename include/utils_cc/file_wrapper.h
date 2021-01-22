/**
 *  Created on: Mar 24, 2016
 *      Author: xing
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <functional>

#include "utils_cc/common.h"
#include "utils_cc/macro_utils.h"

namespace utils
{
class FileWrapper final
{
 public:
  UCC_DISALLOW_COPY_AND_ASSIGN(FileWrapper);

  FileWrapper(FileWrapper &&other) : fl{other.fl}
  {
    UCC_RAISE_UNLESS(fl)
    other.fl = NULL;
  }

  explicit FileWrapper(FILE *file)
      : fl{file} {UCC_RAISE_UNLESS(fl)}

        FileWrapper(const char *name, const char *mode)
      : fl(fopen(name, mode)){UCC_RAISE_VERB_UNLESS(fl, name << ":" << mode)}

        FileWrapper(const std::string &name, const char *mode)
      : FileWrapper(name.c_str(), mode)
  {
  }

  FileWrapper(const std::string &name, const std::string &mode)
      : FileWrapper(name.c_str(), mode.c_str())
  {
  }

  ~FileWrapper() noexcept { fclose(fl); };

  FileWrapper &
  operator=(FileWrapper &&other) noexcept
  {
    if (fl) {
      fclose(fl);
    }
    fl = other.fl;
    other.fl = NULL;
    return *this;
  }

  inline std::string
  read()
  {
    const auto size = (size_t)size();
    auto mem = new char[size + 1];
    mem[size] = '\0';
    unless(size == read(mem, size))
    {
      delete[] mem;
      return std::string{};
    }
    std::string str{mem};
    delete[] mem;
    return str;
  }

  inline size_t
  read(void *__restrict ptr, size_t size, size_t n) noexcept
  {
    return fread(ptr, size, n, fl);
  }

  inline size_t
  read(void *__restrict ptr, size_t size) noexcept
  {
    return fread(ptr, 1, size, fl);
  }

  inline size_t
  write(const std::string &str) noexcept
  {
    return write(str.c_str(), str.size());
  }

  inline size_t
  write(const void *__restrict ptr, size_t size, size_t n) noexcept
  {
    return fwrite(ptr, size, n, fl);
  }

  inline size_t
  write(const void *__restrict ptr, size_t size) noexcept
  {
    return fwrite(ptr, 1, size, fl);
  }

  inline int
  seek(long int off, int whence) noexcept
  {
    return fseek(fl, off, whence);
  }

  inline long int
  tell() noexcept
  {
    return ftell(fl);
  }

  inline int
  flush() noexcept
  {
    return fflush(fl);
  }

  inline int
  get_c() noexcept
  {
    return fgetc(fl);
  }

  inline long int
  size() noexcept
  {
    seek(0, SEEK_END);
    long int len{tell()};
    seek(0, SEEK_SET);
    return len;
  }

  inline int
  read_at(int offset) noexcept
  {
    seek(offset, SEEK_SET);
    return get_c();
  }

  inline int
  print(const char *__restrict fmt, ...) noexcept
  {
    va_list vl;
    va_start(vl, fmt);
    int a = vfprintf(fl, fmt, vl);
    va_end(vl);
    return a;
  }

  /**
   * evaluate each char until f returns false
   */
  inline void
  each_char(const std::function<void(int)> &f) noexcept
  {
    seek(0, SEEK_SET);
    int c;
    while (EOF != (c = get_c())) {
      f(c);
    }
  }

 private:
  UCC_DIRECT_READER(FILE *, fl);
};

}  // namespace utils
