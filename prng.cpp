#include "prng.hpp"

static constexpr uint32_t XORSHIFT32_ALPHA=0x2545F491;
static constexpr uint64_t XORSHIFT64_ALPHA=0x2545F4914F6CDD1D;

static constexpr uint32_t PRNG32_INITIAL_OFFSET=0x6C835AF4;
static constexpr uint64_t PRNG64_INITIAL_OFFSET=0x6C835AF43E27329A;

// == prng-32 ====
prng32::prng32()
{
	pSeed=PRNG32_INITIAL_OFFSET;
}

void prng32::set_seed_value(uint32_t seed_value)
{
	pSeed=seed_value;
}

float prng32::get_rn_fp() const
{
	union fpConverter
	{
		uint32_t uv;
		float fpv;
	} rn;
	rn.uv=0x3F800000 | (pSeed & 0x7FFFFF);
	rn.fpv=rn.fpv-1.0;
	rn.uv|=(pSeed & 0x80000000);
	return(rn.fpv);
}

uint32_t prng32::get_rn_uint() const
{
	return(pSeed);
}

void prng32::generate_xorshift()
{
	pSeed ^= pSeed << 13;
	pSeed ^= pSeed >> 17;
	pSeed ^= pSeed << 5;
}

void prng32::generate_xorshift_star()
{
	pSeed ^= pSeed >> 13;
	pSeed ^= pSeed << 17;
	pSeed ^= pSeed >> 5;
	pSeed *= XORSHIFT32_ALPHA;
}

// == prng-64 ====
prng64::prng64()
{
	pSeed=PRNG64_INITIAL_OFFSET;
}

void prng64::set_seed_value(uint64_t seed_value)
{
	pSeed=seed_value;
}

double prng64::get_rn_fp() const
{
	union fpConverter
	{
		uint64_t uv;
		double fpv;
	} rn;
	rn.uv=0x3FF0000000000000 | (pSeed & 0x000FFFFFFFFFFFFF);
	rn.fpv=rn.fpv-1.0;
	rn.uv|=(pSeed & 0x8000000000000000);
	return(rn.fpv);
}

uint64_t prng64::get_rn_uint() const
{
	return(pSeed);
}

void prng64::generate_xorshift()
{
	pSeed ^= pSeed << 13;
	pSeed ^= pSeed >> 7;
	pSeed ^= pSeed << 17;
}

void prng64::generate_xorshift_star()
{
	pSeed ^= pSeed >> 25;
	pSeed ^= pSeed << 13;
	pSeed ^= pSeed >> 27;
	pSeed *= XORSHIFT64_ALPHA;
}
