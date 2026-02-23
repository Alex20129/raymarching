#include "prng.hpp"

static constexpr uint32_t XORSHIFT32_ALPHA=0x4FA4BB39;
static constexpr uint64_t XORSHIFT64_ALPHA=0x2545F4914FA4BB39;

static constexpr uint32_t PRNG32_INITIAL_OFFSET=0x6C835AF4;
static constexpr uint64_t PRNG64_INITIAL_OFFSET=0x6C835AF43E27329A;

// == prng-32 ====
prng_u32::prng_u32()
{
	pSeed=PRNG32_INITIAL_OFFSET;
}

prng_u32::prng_u32(uint32_t seed_value)
{
	pSeed=seed_value;
}

uint32_t prng_u32::get_seed_value() const
{
	return(pSeed);
}

void prng_u32::set_seed_value(uint32_t seed_value)
{
	pSeed=seed_value;
}

uint32_t prng_u32::generate_xorshift()
{
	uint32_t result = pSeed;
	result ^= result << 13;
	result ^= result >> 17;
	result ^= result << 5;
	pSeed = result;
	return(result);
}

uint32_t prng_u32::generate_xorshift_star()
{
	uint32_t result = pSeed;
	result ^= result >> 13;
	result ^= result << 17;
	result ^= result >> 5;
	result *= XORSHIFT32_ALPHA;
	pSeed = result;
	return(result);
}

// == prng-64 ====
prng_u64::prng_u64()
{
	pSeed=PRNG64_INITIAL_OFFSET;
}

prng_u64::prng_u64(uint64_t seed_value)
{
	pSeed=seed_value;
}

uint64_t prng_u64::get_seed_value() const
{
	return(pSeed);
}

void prng_u64::set_seed_value(uint64_t seed_value)
{
	pSeed=seed_value;
}

uint64_t prng_u64::generate_xorshift()
{
	uint64_t result = pSeed;
	result ^= result << 13;
	result ^= result >> 7;
	result ^= result << 17;
	pSeed = result;
	return(result);
}

uint64_t prng_u64::generate_xorshift_star()
{
	uint64_t result = pSeed;
	result ^= result >> 13;
	result ^= result << 25;
	result ^= result >> 27;
	result *= XORSHIFT64_ALPHA;
	pSeed = result;
	return(result);
}
