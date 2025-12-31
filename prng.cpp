#include "prng.hpp"

static const uint32_t MWC32_ALPHA=0x7F545431;
static const uint64_t MWC64_ALPHA=0x00007F5454545437;

static const uint32_t FNV32_INITIAL_OFFSET=0x811C9DC5;
static const uint64_t FNV64_INITIAL_OFFSET=0xCBF29CE484222325;

static const uint32_t FNV32_PRIME=0x01000193;
static const uint64_t FNV64_PRIME=0x00000100000001B3;

static const uint32_t XORSHIFT32_ALPHA=0x4F6CDD1D;
static const uint64_t XORSHIFT64_ALPHA=0x2545F4914F6CDD1D;

// == prng-32 ====
prng_u32::prng_u32()
{
	pSeed=FNV32_INITIAL_OFFSET;
}

uint32_t prng_u32::get_seed_value() const
{
	return(pSeed);
}

void prng_u32::set_seed_value(uint32_t seed_value)
{
	pSeed=seed_value;
}

uint32_t prng_u32::generate_mwc()
{
	uint32_t result=pSeed;
	result=(result & UINT16_MAX) * MWC32_ALPHA + (result >> 16);
	pSeed=result;
	return(result);
}

uint32_t prng_u32::generate_fnv1()
{
	uint32_t result = pSeed;
	result *= FNV32_PRIME;
	result ^= pSeed;
	pSeed = result;
	return(result);
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
	pSeed=FNV64_INITIAL_OFFSET;
}

uint64_t prng_u64::get_seed_value() const
{
	return(pSeed);
}

void prng_u64::set_seed_value(uint64_t seed_value)
{
	pSeed=seed_value;
}

uint64_t prng_u64::generate_mwc()
{
	uint64_t result=pSeed;
	result=(result & UINT32_MAX) * MWC64_ALPHA + (result >> 32);
	pSeed=result;
	return(result);
}

uint64_t prng_u64::generate_fnv1()
{
	uint64_t result = pSeed;
	result *= FNV64_PRIME;
	result ^= pSeed;
	pSeed = result;
	return(result);
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
