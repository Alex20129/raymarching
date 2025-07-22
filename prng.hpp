#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

struct prng_u32
{
private:
	uint32_t pSeed;
public:
	prng_u32();
	void set_seed_value(uint32_t new_seed);
	uint32_t generate_mms();
	uint32_t generate_fnv();
	uint32_t generate_xs();
};

struct prng_u64
{
private:
	uint64_t pSeed;
public:
	prng_u64();
	void set_seed_value(uint64_t new_seed);
	uint64_t generate_mms();
	uint64_t generate_fnv();
	uint64_t generate_xs();
};

#endif // PRNG_H
