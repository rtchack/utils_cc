/**
 * Created on: 3/28/17
 *     Author: xing
 */

#pragma once

#include <stdint.h>
#include <arpa/inet.h>

#define UTILS_NET_ATTR_S(nm)                             \
 private:                                                \
  uint16_t nm;                                           \
                                                         \
 public:                                                 \
  inline uint16_t get_##nm() const { return ntohs(nm); } \
  inline void set_##nm(uint16_t _nm) { nm = htons(_nm); }

#define UTILS_NET_ATTR_L(nm)                             \
 private:                                                \
  uint32_t nm;                                           \
                                                         \
 public:                                                 \
  inline uint32_t get_##nm() const { return ntohl(nm); } \
  inline void set_##nm(uint32_t _nm) { nm = htonl(_nm); }
