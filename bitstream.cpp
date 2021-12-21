struct cBitPointer
{
    unsigned char* base;
    int pos = 0;

    cBitPointer(unsigned char* bits) : base(bits) {}

    cBitPointer& operator+=(int offset)
    {
        pos += offset;
        return *this;
    }
    int readBit()
    {
        int shift = 7 - (pos & 7);
        int bytePos = pos >> 3;
        ++pos;
        return (base[bytePos] & (1 << shift)) >> shift;
    }
    ull readNumber(int numberOfBits)
    {
        ull result = 0;
        FOR(i, numberOfBits)
        {
            result <<= 1;
            result |= readBit();
        }
        return result;
    }
};
