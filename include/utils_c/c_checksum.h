/*
 * Created by xing in 2021.
 */

#ifndef UTILS_CC_SOURCE_C_CHECKSUM_H
#define UTILS_CC_SOURCE_C_CHECKSUM_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

const static uint16_t OC_CHECKSUM_INIT = 0;

uint16_t
gen_oc_checksum(uint16_t checksum, const uint8_t *data, size_t len);

static inline uint16_t
finalize_oc_checksum(uint16_t checksum)
{
  return (uint16_t)(~checksum & 0xffff);
}

static inline bool
verify_oc_checksum(uint16_t checksum)
{
  return checksum == 0xffff;
}

#ifdef __cplusplus
};
#endif

#endif  // UTILS_CC_SOURCE_C_CHECKSUM_H
