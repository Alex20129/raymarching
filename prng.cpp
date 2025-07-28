#include "prng.hpp"

static const uint32_t MMS32_ALPHA=0x7F545415;
static const uint64_t MMS64_ALPHA=0x000007F22254544B;

static const uint32_t FNV32_INITIAL_OFFSET=0x811C9DC5;
static const uint64_t FNV64_INITIAL_OFFSET=0xCBF29CE484222325;

static const uint32_t FNV32_PRIME=0x01000193;
static const uint64_t FNV64_PRIME=0x00000100000001B3;

// == prng-32 ====
prng_u32::prng_u32()
{
	pSeed=FNV32_INITIAL_OFFSET;
}

void prng_u32::set_seed_value(uint32_t new_seed)
{
	pSeed=new_seed;
}

uint32_t prng_u32::generate_mms()
{
	pSeed+=pSeed & UINT8_MAX;
	pSeed=(pSeed & UINT16_MAX) * MMS32_ALPHA + (pSeed >> 16);
	return(pSeed);
}

uint32_t prng_u32::generate_fnv1a()
{
	pSeed^=pSeed & UINT8_MAX;
	pSeed*=FNV32_PRIME;
	return(pSeed);
}

uint32_t prng_u32::generate_xorshift()
{
	pSeed+=pSeed & UINT8_MAX;
	pSeed^=pSeed<<13;
	pSeed^=pSeed>>17;
	pSeed^=pSeed<<5;
	return(pSeed);
}

// == prng-64 ====
prng_u64::prng_u64()
{
	pSeed=FNV64_INITIAL_OFFSET;
}

void prng_u64::set_seed_value(uint64_t new_seed)
{
	pSeed=new_seed;
}

uint64_t prng_u64::generate_mms()
{
	pSeed+=pSeed & UINT8_MAX;
	pSeed=(pSeed & UINT32_MAX) * MMS64_ALPHA + (pSeed >> 32);
	return(pSeed);
}

uint64_t prng_u64::generate_fnv1a()
{
	pSeed^=pSeed & UINT8_MAX;
	pSeed*=FNV64_PRIME;
	return(pSeed);
}

uint64_t prng_u64::generate_xorshift()
{
	pSeed+=pSeed & UINT8_MAX;
	pSeed^=pSeed<<13;
	pSeed^=pSeed>>7;
	pSeed^=pSeed<<17;
	return(pSeed);
}
