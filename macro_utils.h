/**
 *  Created on: Mar 27, 2016
 *      Author: xing
 */

#ifndef COMMON_INC_CONSTRUCTOR_MACROS_MAGIC_H_
#define COMMON_INC_CONSTRUCTOR_MACROS_MAGIC_H_

#include <memory>
#include <thread>


#ifndef unless
#define unless(ex) if(!(ex))
#endif

#define BASE_ROUND_UNIT 16
#define BASE_ROUND(size, round) (((size) + (round) - 1) & (~((round) - 1)))
#define BASE_ROUNDED(size) (BASE_ROUND((size), BASE_ROUND_UNIT))

#ifndef TO_STRING
#if defined(__ANDROID__)
#define TO_STRING(s) to_stringAndroid(s)
template <typename T>
inline std::string to_stringAndroid(T value){
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}
#else
#define TO_STRING(s) std::to_string(s)
#endif
#endif

#define BASE_STR_S(size)\
  std::string s;\
  s.reserve(size);
#define BASE_STR_ATTR_STR(a)\
  s.append(", " #a " ");\
  s.append(a);
#define BASE_STR_ATTR(a)\
  s.append(", " #a " ");\
  s.append(TO_STRING(a));
#define BASE_STR_ITEA_STR(i, nm)\
  s.append(", " #i "-");\
  s.append(*(nm + i));
#define BASE_STR_ITEA(i, nm)\
  s.append(", " #i "-");\
  s.append(TO_STRING(*(nm + i)));

#define BASE_DISALLOW_ASSIGN(TypeName) \
  void operator=(const TypeName&) = delete;

#define BASE_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;          \
  BASE_DISALLOW_ASSIGN(TypeName);

#define BASE_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                               \
  BASE_DISALLOW_COPY_AND_ASSIGN(TypeName);

#define BASE_DISALLOW_INSTANTIATION(TypeName) \
  BASE_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName); \
  ~TypeName() = delete;

#define BASE_ATTR(tp, nm) \
  private:\
    tp nm

#define BASE_READER(tp, nm) \
  public:\
    inline const tp& Get_ ##nm() const noexcept{return nm;}\
    inline tp& Get_ ##nm() noexcept{return nm;}\
  BASE_ATTR(tp, nm)
#define BASE_ACCESSOR(tp, nm) \
  public:\
    inline const tp& Get_ ##nm() const noexcept{return nm;}\
    inline tp& Get_ ##nm() noexcept{return nm;}\
    inline void Set_ ##nm(const tp &v) noexcept{nm = v;}\
  BASE_ATTR(tp, nm)

#define BASE_DIRECT_READER(tp, nm) \
  public:\
    inline const tp Get_ ##nm() const noexcept{return nm;}\
    inline tp Get_ ##nm() noexcept{return nm;}\
  BASE_ATTR(tp, nm)
#define BASE_DIRECT_ACCESSOR(tp, nm) \
  public:\
    inline const tp Get_ ##nm() const noexcept{return nm;}\
    inline tp Get_ ##nm() noexcept{return nm;}\
    inline void Set_ ##nm(tp v) noexcept{nm = v;}\
  BASE_ATTR(tp, nm)


#define BASE_ROUNDED_STRUCTURE(name, ...) \
  struct pad_helper_##name { __VA_ARGS__ }; \
  struct name { \
      __VA_ARGS__ \
      unsigned char reserved_padding[BASE_ROUND(sizeof(pad_helper_##name),\
          BASE_ROUND_UNIT) - sizeof(pad_helper_##name)]; \
  };

#define BASE_SLEEP(tm) \
  std::this_thread::sleep_for(std::chrono::tm);

#endif /* COMMON_INC_CONSTRUCTOR_MACROS_MAGIC_H_ */
