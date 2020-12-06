/**
 *  Created on: Mar 27, 2016
 *      Author: xing
 */

#pragma once

#include <memory>
#include <thread>

#ifndef unless
#define unless(ex) if (!(ex))
#endif

#define UTILS_UNUSED(a) (void)(a)

#ifdef OS_WIN32
#define alloca _alloca
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))
#endif

#define UTILS_ROUND_UNIT 16U
#define UTILS_ROUND(size, round) (((size) + (round)-1) & (~((round)-1)))
#define UTILS_ROUNDED(size) (UTILS_ROUND((size), UTILS_ROUND_UNIT))

#ifndef TO_STRING
#if defined(__ANDROID__)
#define TO_STRING(s) to_stringAndroid(s)
template <typename T>
inline std::string
to_stringAndroid(T value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}
#else
#define TO_STRING(s) std::to_string(s)
#endif
#endif

#define UTILS_STR_S(size) \
  std::string s;          \
  s.reserve(size);
#define UTILS_STR_ATTR_STR(a) \
  s.append(", " #a " ");      \
  s.append(a);
#define UTILS_STR_ATTR(a) \
  s.append(", " #a " ");  \
  s.append(TO_STRING(a));
#define UTILS_STR_ITEA_STR(i, nm) \
  s.append(", " #i "-");          \
  s.append(*(nm + i));
#define UTILS_STR_ITEA(i, nm) \
  s.append(", " #i "-");      \
  s.append(TO_STRING(*(nm + i)));

#define UTILS_DISALLOW_ASSIGN(TypeName)     \
  void operator=(const TypeName&) = delete; \
  void operator=(TypeName&) = delete;

#define UTILS_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;            \
  TypeName(TypeName&) = delete;                  \
  UTILS_DISALLOW_ASSIGN(TypeName);

#define UTILS_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                                 \
  UTILS_DISALLOW_COPY_AND_ASSIGN(TypeName);

#define UTILS_DISALLOW_INSTANTIATION(TypeName)    \
  UTILS_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName); \
  ~TypeName() = delete;

#define UTILS_ATTR(tp, nm) \
 private:                  \
  tp nm

#define UTILS_READER(tp, nm)                                \
 public:                                                    \
  inline const tp& get_##nm() const noexcept { return nm; } \
  inline tp& get_##nm() noexcept { return nm; }             \
  UTILS_ATTR(tp, nm)

#define UTILS_ACCESSOR(tp, nm)                              \
 public:                                                    \
  inline const tp& get_##nm() const noexcept { return nm; } \
  inline tp& get_##nm() noexcept { return nm; }             \
  inline void set_##nm(const tp& v) noexcept { nm = v; }    \
  UTILS_ATTR(tp, nm)

#define UTILS_DIRECT_READER(tp, nm)                        \
 public:                                                   \
  inline const tp get_##nm() const noexcept { return nm; } \
  inline tp get_##nm() noexcept { return nm; }             \
  UTILS_ATTR(tp, nm)

#define UTILS_DIRECT_ACCESSOR(tp, nm)                      \
 public:                                                   \
  inline const tp get_##nm() const noexcept { return nm; } \
  inline tp get_##nm() noexcept { return nm; }             \
  inline void set_##nm(tp v) noexcept { nm = v; }          \
  UTILS_ATTR(tp, nm)

#define UTILS_ROUNDED_STRUCTURE(name, ...)                                \
  struct pad_helper_##name {                                              \
    __VA_ARGS__                                                           \
  };                                                                      \
  struct name {                                                           \
    __VA_ARGS__                                                           \
    unsigned char reserved_padding[UTILS_ROUND(sizeof(pad_helper_##name), \
                                               UTILS_ROUND_UNIT) -        \
                                   sizeof(pad_helper_##name)];            \
  };

#define UTILS_SLEEP(tm) std::this_thread::sleep_for(std::chrono::tm);
