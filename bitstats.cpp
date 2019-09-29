/// ----------------------------------------------------------------------------------------------------------- bitstats.cpp

struct cBitStat
{
	ull times_1;
	ull times_0;
	ull times_switched;
};

inline cBitStat operator-(const cBitStat &a, const cBitStat &b)
{
	return{ a.times_1-b.times_1, a.times_0-b.times_0, a.times_switched-b.times_switched };
}

cBitStat bit_stat_from_0(unsigned int bit, ull to) // inclusive to
{
	auto group_size=1ull<<(bit+1);
	auto half_group_size=group_size>>1ull;
	auto full_group=(to+1)/group_size;
	auto last_group_size=(to+1)&(group_size-1ull);
	cBitStat stat;
	stat.times_1=full_group*half_group_size+(last_group_size>half_group_size?last_group_size-half_group_size:0);
	stat.times_0=to+1-stat.times_1;
	auto full_group_for_switch=to/group_size;
	auto last_group_for_switch=to&(group_size-1ull);
	stat.times_switched=full_group_for_switch*2+(last_group_for_switch>=half_group_size?1:0);
	return stat;
}

cBitStat bit_stat_between(unsigned int bit, ull from, ull to) // inclusive from and to
{
	return bit_stat_from_0(bit, to)-bit_stat_from_0(bit, from-1);
}
/*
ull XorSumUntil(ull to)
{
	ull result=0;
	for(unsigned int b=0; b<64; ++b)
	{
		result|=(bit_stat_from_0(b, to).times_1&1)<<b;
	}
	return result;
}
*/
ull XorSumUntil(ull to)
{
	switch(to&3)
	{
	case 0: return n;
	case 1: return 1;
	case 2: return n+1;
	default: return 0;
	}
}

ull XorSumBetween(ull from, ull to)
{
	return XorSumUntil(from-1)^XorSumUntil(to);
}


////////////////////////////

template<class T> auto isolate_lowest_bit(T x)
{
	return x&(0-x);
}

ull isolate_highest_bit(ull x)
{
	x|=x>>1;
	x|=x>>2;
	x|=x>>4;
	x|=x>>8;
	x|=x>>16;
	x|=x>>32;
	return x&~(x>>1);
}

ull ones_up_till_highest_bit(ull x) //   for example 0000'0000'0010'1101 -->  0000'0000'0011'1111
{
	return x==0?0:(((isolate_highest_bit(x)-1ull)<<1ull)|1ull);
}

ull ones_above_highest_bit(ull x) //   for example 0000'0000'0010'1101 -->  1111'1111'1100'0000
{
	return ~ones_up_till_highest_bit(x);
}

template<class T>
bool is_power_of_2(T x)
{
	return x>0&&((x&(x-1))==0);
}

unsigned int remove_bit(unsigned int x, int bit_to_remove) //   for example 0000'0000'0010'1101  , 3 -->  for example 0000'0000'0001'0101
                                                           //                              ^ this gets removed
{
	unsigned int mask=(1<<bit_to_remove)-1;
	return (x&mask)|((x>>1)&~mask);
}



unsigned int reverse_bits(unsigned int x)   /// UNTESTED!!!
{
	static unsigned int reversed_bits[16]=
	{
		0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
		0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
	};
	return reversed_bits[x>>28]|
		(reversed_bits[(x>>24)&0xf]<<4)|
		(reversed_bits[(x>>20)&0xf]<<8)|
		(reversed_bits[(x>>16)&0xf]<<12)|
		(reversed_bits[(x>>12)&0xf]<<16)|
		(reversed_bits[(x>>8)&0xf]<<20)|
		(reversed_bits[(x>>4)&0xf]<<24)|
		(reversed_bits[x&0xf]<<28);
}

////////////////////////////
// http://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers

const int tab64[64] = {
	63,  0, 58,  1, 59, 47, 53,  2,
	60, 39, 48, 27, 54, 33, 42,  3,
	61, 51, 37, 40, 49, 18, 28, 20,
	55, 30, 34, 11, 43, 14, 22,  4,
	62, 57, 46, 52, 38, 26, 32, 41,
	50, 36, 17, 19, 29, 10, 13, 21,
	56, 45, 25, 31, 35, 16,  9, 12,
	44, 24, 15,  8, 23,  7,  6,  5 };

int log2_64(uint64_t value)
{
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value |= value >> 32;
	return tab64[((uint64_t)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
}

const int tab32[32] = {
	0,  9,  1, 10, 13, 21,  2, 29,
	11, 14, 16, 18, 22, 25,  3, 30,
	8, 12, 20, 28, 15, 17, 24,  7,
	19, 27, 23,  6, 26,  5,  4, 31 };

int log2_32(uint32_t value)
{
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

// http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
int GetNumberOfSetBits(uint32_t i)
{
	i = i - ((i >> 1) & 0x55555555u);
	i = (i & 0x33333333u) + ((i >> 2) & 0x33333333u);
	return (((i + (i >> 4)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
}

/// ----------------------------------------------------------------------------------------------------------- end of bitstats.cpp
