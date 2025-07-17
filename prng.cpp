#include "prng.hpp"

static const uint32_t MMS_ALPHA_32=0xBC948A27;
static const uint64_t MMS_ALPHA_64=0xA58B6A35BC948A27;

static const uint32_t FNV32_PRIME =0x01000193;
static const uint64_t FNV64_PRIME =0x00000100000001B3;

static uint32_t SEED_VALUE_32=0xDEADBEEF;
static uint64_t SEED_VALUE_64=0xDEADBEEFDEADBEEF;

uint32_t mms_prng_32()
{
	uint32_t mms_result=SEED_VALUE_32;
	mms_result+=mms_result & UINT32_SPARSE_MASK;
	mms_result=(mms_result & UINT16_MASK) * MMS_ALPHA_32 + (mms_result >> 16);
	SEED_VALUE_32=mms_result;
	return(mms_result);
}

uint64_t mms_prng_64()
{
	uint64_t mms_result=SEED_VALUE_64;
	mms_result+=mms_result & UINT64_SPARSE_MASK;
	mms_result=(mms_result & UINT32_MASK) * MMS_ALPHA_64 + (mms_result >> 32);
	SEED_VALUE_64=mms_result;
	return(mms_result);
}

uint32_t fnv1a_prng_32()
{
	uint32_t fnv_result=SEED_VALUE_32;
	fnv_result^=fnv_result & UINT32_SPARSE_MASK;
	fnv_result*=FNV32_PRIME;
	SEED_VALUE_32=fnv_result;
	return(fnv_result);
}

uint64_t fnv1a_prng_64()
{
	uint64_t fnv_result=SEED_VALUE_64;
	fnv_result^=fnv_result & UINT64_SPARSE_MASK;
	fnv_result*=FNV64_PRIME;
	SEED_VALUE_64=fnv_result;
	return(fnv_result);
}
