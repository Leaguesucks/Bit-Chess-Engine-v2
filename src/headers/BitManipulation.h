#pragma once

#include <string>

#include "General.h"

/**
 * @brief Provide methods to manipulate bits in an u64
 * 
 * This class provides methods to manipulate the bits in a 64-bits integer. Some common
 * methods are set bits, pop bits, etc.
 * 
 * @author Dang Nguyen
 * @note The bit position is counted up from 0-63 from LSB to MSB
 */
namespace BitManipulation {
    extern unsigned char popCountOfByte256[256]; // Lookup table for the countBit function
    
    // Lookup table for LSS bit scan operations 
    const int index64[64] = {
        0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
    };

    extern bool init;

    /**
     * @brief Initialize any neccessary resource
     * @note Must be called at least once before using any other functions
     */
    void initialize();

    /**
     * @brief Set a single bit to 1
     * 
     * This function sets the "square" bit on the bit board to 1
     * 
     * @param b Pointer to the u64 number
     * @param pos The bit position to be set
     */
    void setBit(u64 *b, int pos);

    /**
     * @brief Remove a set bit
     * 
     * This function pop a "1" bit on the bitboard
     * 
     * @param b Pointer to the u64 number
     * @param pos The bit position to be pop
     */
    void popBit(u64 *b, int pos);

    /**
     * @brief Return the bit value at a given position
     * 
     * This function checks if a bit has been set (to 1)
     * 
     * @param b The u64 number to evaluate
     * @param pos The bit position to be returned
     * @return True if the bit has been set to 1, false otherwise
     */
    bool getBit(const u64 b, int pos);

    /**
     * @brief Count the number of set bit 
     * 
     * This function counts how many bits have been set to "1"
     * 
     * @param b The u64 number to be counted
     * @return The number of set bits
     */
    int countBit(u64 b);

    /**
     * @brief Return the least significant set bit (LSSB)
     * 
     * This function returns the least significant bit that has been set to "1"
     * 
     * @author Martin Läuter (1997)
     *         Charles E. Leiserson
     *         Harald Prokop
     *         Keith H. Randall
     * @param b The u64 number
     * @return The least significant bit-index that has been set to "1"
     */
    int getLSSB(u64 b);

    /**
     * @brief Return the most significant set bit (MSSB)
     * 
     * This function returns the most significant bit that has been set to "1"
     * 
     * @param b The u64 number
     * @return The most significant bit-index that has been set to "1"
     */
    int getMSSB(u64 b);

    /**
     * @brief Encode the bit masks into a style that closely resembles the FEN position field.
     * @brief Any masked square is encoded with 'x' and between them are the number of unmasked squares
     * 
     * @param b The bit field, in this case an u64 number, to encode
     * @return The masked squares in a bit fields encoded as a FEN-styled string
     */
    std::string encodeMaskBits(u64 b);
}