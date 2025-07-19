#include "prng.hpp"

static const uint32_t MMS_ALPHA_32=0xAC948B27;
static const uint64_t MMS_ALPHA_64=0xE58B6F35AC948B27;

static const uint32_t FNV32_PRIME =0x01000193;
static const uint64_t FNV64_PRIME =0x00000100000001B3;

static uint32_t SEED_VALUE_32=0xDEADBEEF;
static uint64_t SEED_VALUE_64=0xDEADBEEFDEADBEEF;

void prng_set_seed_32(uint32_t seed)
{
	SEED_VALUE_32=seed;
}

void prng_set_seed_64(uint64_t seed)
{
	SEED_VALUE_64=seed;
}

uint32_t mms_prng_32()
{
	uint32_t result=SEED_VALUE_32;
	result=(result & UINT16_MAX) * MMS_ALPHA_32 + (result >> 16);
	SEED_VALUE_32=result;
	return(result);
}

uint64_t mms_prng_64()
{
	uint64_t result=SEED_VALUE_64;
	result=(result & UINT32_MAX) * MMS_ALPHA_64 + (result >> 32);
	SEED_VALUE_64=result;
	return(result);
}

uint32_t fnv_prng_32()
{
	uint32_t result=SEED_VALUE_32;
	result^=result & UINT16_MAX;
	result*=FNV32_PRIME;
	SEED_VALUE_32=result;
	return(result);
}

uint64_t fnv_prng_64()
{
	uint64_t result=SEED_VALUE_64;
	result^=result & UINT32_MAX;
	result*=FNV64_PRIME;
	SEED_VALUE_32=result;
	return(result);
}
