#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

#define UINT16_MASK			0xFFFF
#define UINT32_MASK			0xFFFFFFFF

#define UINT32_SPARSE_MASK	0x55555555
#define UINT64_SPARSE_MASK	0x5555555555555555

uint32_t mms_prng_32();
uint64_t mms_prng_64();

uint32_t fnv1a_prng_32();
uint64_t fnv1a_prng_64();

#endif // PRNG_H
