#include "prng.hpp"

static const uint32_t MMS_ALPHA_32=0xAC948B27;
static const uint64_t MMS_ALPHA_64=0xE58B6F35AC948B27;

static const uint32_t FNV_PRIME_32=0x01000193;
static const uint64_t FNV_PRIME_64=0x00000100000001B3;

// == prng-32 ====
prng_u32::prng_u32()
{
	pSeed=0xDEADBEEF;
}

void prng_u32::set_seed_value(uint32_t new_seed)
{
	pSeed=new_seed;
}

uint32_t prng_u32::generate_mms()
{
	pSeed=(pSeed & UINT16_MAX) * MMS_ALPHA_32 + (pSeed >> 16);
	return(pSeed);
}

uint32_t prng_u32::generate_fnv()
{
	pSeed^=pSeed & UINT16_MAX;
	pSeed*=FNV_PRIME_32;
	return(pSeed);
}

uint32_t prng_u32::generate_xs()
{
	pSeed^=pSeed<<13;
	pSeed^=pSeed>>17;
	pSeed^=pSeed<<5;
	return(pSeed);
}

// == prng-64 ====
prng_u64::prng_u64()
{
	pSeed=0xDEADBEEFDEADBEEF;
}

void prng_u64::set_seed_value(uint64_t new_seed)
{
	pSeed=new_seed;
}

uint64_t prng_u64::generate_mms()
{
	pSeed=(pSeed & UINT32_MAX) * MMS_ALPHA_64 + (pSeed >> 32);
	return(pSeed);
}

uint64_t prng_u64::generate_fnv()
{
	pSeed^=pSeed & UINT32_MAX;
	pSeed*=FNV_PRIME_64;
	return(pSeed);
}

uint64_t prng_u64::generate_xs()
{
	pSeed^=pSeed<<13;
	pSeed^=pSeed>>7;
	pSeed^=pSeed<<17;
	return(pSeed);
}
