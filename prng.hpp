#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

class prng32
{
	uint32_t pSeed;
public:
	prng32();
	void set_seed_value(uint32_t seed_value);
	float get_rn_fp() const;
	uint32_t get_rn_uint() const;
	void generate_xorshift();
	void generate_xorshift_star();
};

class prng64
{
	uint64_t pSeed;
public:
	prng64();
	void set_seed_value(uint64_t seed_value);
	double get_rn_fp() const;
	uint64_t get_rn_uint() const;
	void generate_xorshift();
	void generate_xorshift_star();
};

#endif // PRNG_H
