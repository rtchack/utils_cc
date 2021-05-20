/*
 * Created by xing in 2021.
 */

#include <utils_c/checksum.h>
#include <utils_c/common.h>

uint16_t
gen_oc_checksum(uint16_t checksum, const uint8_t *data, size_t len)
{
  UC_DCHECK(data && len);
  if (!len) {
    return checksum;
  }

  size_t i = len % 2 ? 1 : 2;

  checksum += 1 == i ? (uint16_t)*data : *(const uint16_t *)data;
  if (checksum > 0xffff) {
    checksum -= 0xffff;
  }

  for (; i < len; i += 2) {
    checksum += *(const uint16_t *)(data + i);
    if (checksum > 0xffff) {
      checksum -= 0xffff;
    }
  }

  UC_DCHECK(i == len);
  return checksum;
}
