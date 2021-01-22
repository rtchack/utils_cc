/**
 * Created on: 6/28/17
 *     Author: xing
 */

#pragma once

#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <string.h>

#include "utils_cc/log.h"
#include "utils_cc/common.h"

namespace utils
{
const std::string
hardware_addr()
{
  ifaddrs *ifaddr{};
  UCC_RAISE_VERB_IF(getifaddrs(&ifaddr) == -1, strerror(errno))

  char addr_str[256];
  size_t pos = 0;
  size_t remain = 256;
  for (auto ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if ((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET)) {
      if (strlen(ifa->ifa_name) == 2 && !strncmp(ifa->ifa_name, "lo", 2)) {
        continue;
      }

#define HARDWAREADDR_PRINT_MOVE(fmt, ...)                                  \
  {                                                                        \
    const auto ret = snprintf(addr_str + pos, remain, fmt, ##__VA_ARGS__); \
    if (ret < 0) {                                                         \
      lErr(strerror(errno));                                               \
      goto final_act;                                                      \
    }                                                                      \
    pos += ret;                                                            \
    remain -= ret;                                                         \
    if (remain <= 0) {                                                     \
      lInf("Truncated") goto final_act;                                    \
    }                                                                      \
  }

      HARDWAREADDR_PRINT_MOVE("%s~>", ifa->ifa_name)

      const auto s = (sockaddr_ll *)ifa->ifa_addr;
      for (int i = 0; i < s->sll_halen; i++) {
        HARDWAREADDR_PRINT_MOVE(
            "%02x%c", (s->sll_addr[i]), (i + 1 != s->sll_halen) ? ':' : '\t')
      }
    }
  }

final_act:
  freeifaddrs(ifaddr);
  std::string s{addr_str};
  return s;
}

}  // namespace utils
