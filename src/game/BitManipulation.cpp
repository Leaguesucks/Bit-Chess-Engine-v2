#include "../headers/BitManipulation.h"

namespace BitManipulation {
    unsigned char popCountOfByte256[256];
    bool init = false;

    void initialize() {
        if (init)
            return;

        popCountOfByte256[0] = 0;

        for (int i = 1; i < 256; i++)
            popCountOfByte256[i] = (i & 1) + popCountOfByte256[i / 2];
        
        init = true;
    }

    void setBit(u64 *b, int pos) {
        if (pos >= 0 && pos <= 63)
            *b |= (1ULL << pos);
    }

    void popBit(u64 *b, int pos) {
        if (pos >= 0 && pos <= 63)
            *b &= ~(1ULL << pos);
    }

    bool getBit(const u64 b, int pos) {
        if (pos >= 0 && pos <= 63)
            return (b & (1ULL << pos)) != 0;
        return false;
    }

    int countBit(u64 b) {
        return popCountOfByte256[ b        & 0xff] +
          popCountOfByte256[(b >>  8) & 0xff] +
          popCountOfByte256[(b >> 16) & 0xff] +
          popCountOfByte256[(b >> 24) & 0xff] +
          popCountOfByte256[(b >> 32) & 0xff] +
          popCountOfByte256[(b >> 40) & 0xff] +
          popCountOfByte256[(b >> 48) & 0xff] +
          popCountOfByte256[ b >> 56];
    }

    int getLSSB(u64 b) {
        if (b == 0) return -1;
        return index64[((b & -b) * 0x03f79d71b4cb0a89ULL) >> 58];
    }

    int getMSSB(u64 b) {
        if (b == 0) return -1;

        int index = 0;
        if (b >= (1ULL << 32)) { b >>= 32; index += 32; }
        if (b >= (1ULL << 16)) { b >>= 16; index += 16; }
        if (b >= (1ULL << 8))  { b >>= 8;  index += 8; }
        if (b >= (1ULL << 4))  { b >>= 4;  index += 4; }
        if (b >= (1ULL << 2))  { b >>= 2;  index += 2; }
        if (b >= (1ULL << 1))  {           index += 1; }

        return index;
    }
}
