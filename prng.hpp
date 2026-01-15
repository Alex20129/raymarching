#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

struct prng_u32
{
private:
	uint32_t pSeed;
public:
	prng_u32();
	prng_u32(uint32_t seed_value);
	uint32_t get_seed_value() const;
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
	prng_u64(uint64_t seed_value);
	uint64_t get_seed_value() const;
	void set_seed_value(uint64_t seed_value);
	uint64_t generate_mwc();
	uint64_t generate_fnv1();
	uint64_t generate_xorshift();
	uint64_t generate_xorshift_star();
};

#endif // PRNG_H
