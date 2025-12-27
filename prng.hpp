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
	uint32_t generate_mwc();
	uint32_t generate_fnv1();
	uint32_t generate_xorshift();
	uint32_t generate_xorshift_star();
};

struct prng_u64
{
private:
	uint64_t pSeed;
public:
	prng_u64();
	void set_seed_value(uint64_t new_seed);
	uint64_t generate_mwc();
	uint64_t generate_fnv1();
	uint64_t generate_xorshift();
	uint64_t generate_xorshift_star();
};

#endif // PRNG_H
