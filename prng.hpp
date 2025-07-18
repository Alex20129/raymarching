#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

uint32_t mms_prng_32();
uint64_t mms_prng_64();

uint32_t fnv_prng_32();
uint64_t fnv_prng_64();

#endif // PRNG_H
