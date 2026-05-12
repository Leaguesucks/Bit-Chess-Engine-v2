#include "../headers/Magic.h"

namespace Magic {
    /**
     * @brief Internal used only
     */
    namespace {
        /**
         * @return A random u64 number
         */
        u64 randomU64() {
            u64 u1, u2, u3, u4;
            u1 = (u64)(rand()) & 0xFFFF; u2 = (u64)(rand()) & 0xFFFF;
            u3 = (u64)(rand()) & 0xFFFF; u4 = (u64)(rand()) & 0xFFFF;
            return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
        }

        /**
         * @return A random u64 number with high number of "1" bits
         * @note Useful for magic numbers generation
         */
        u64 randomU64_high1s() {
            return randomU64() & randomU64() & randomU64();
        }

        /**
         * @brief Generate the magic number of the rook or bishop on a given square
         * @param square The given square
         * @param bishop True for the bishop, false for the rook
         * @return The magic number if successful, 0 otherwise
         */
        u64 findMagic(int square, bool bishop) {
            u64 rev_squares[4096], attacks[4096], used_attacks[4096];
            u64 relevant_attacks;
            u32 relevant_attack_num;

            if (bishop) {
                relevant_attacks = Sliders::relevantBishopAttacks[square];
                relevant_attack_num = Sliders::NUM_BISHOP_RELEVANT_SQUARES[square];
            }
            else {
                relevant_attacks = Sliders::relevantRookAttacks[square];
                relevant_attack_num = Sliders::NUM_ROOK_RELEVANT_SQUARES[square];
            }

            // For n relevant attacks, there 2^n different possible relevant attacks configurations
            u32 possible_relevant_attacks_config_num = 1 << relevant_attack_num;

            for (u32 i = 0; i < possible_relevant_attacks_config_num; i++) {
                rev_squares[i] = Sliders::findRelevantSquares(i, relevant_attacks);

                // Initiate all possible attack configurations given this square
                if (bishop)
                    attacks[i] = Sliders::getBlockedBishopAttacks(rev_squares[i], square);
                else
                    attacks[i] = Sliders::getBlockedRookAttacks(rev_squares[i], square);
            }

            /* Generate and test for a magic number */
            for (u32 j = 0; j < 100000000; j++) {
                u64 magic_number = randomU64_high1s(); // Generate a random magic number

                // Skip inefficient magic numbers
                if (BitManipulation::countBit((relevant_attacks * magic_number) & 0xFF00000000000000) < 6)
                    continue;

                memset(used_attacks, 0ULL, sizeof(used_attacks));

                bool success = true;
                
                /* Test for the magic number */
                for (u32 k = 0; k < possible_relevant_attacks_config_num; k++) {
                    u64 magic_index = (rev_squares[k] * magic_number) 
                        >> (64 - relevant_attack_num);

                    if (used_attacks[magic_index] == 0ULL) // No collision
                        used_attacks[magic_index] = attacks[k]; // This index has been used
                    else if (used_attacks[magic_index] != attacks[k]) { // Collision happens, this magic number doesnt work
                        success = false;
                        break;
                    }
                }

                if (success) 
                    return magic_number; // Magic number success if there is no collision
            }

            // After 100000000 unsucessful attemps, return failure
            return 0ULL;
        }
    }

    void printMagic() {
        BitManipulation::initialize();
        Sliders::initialize();
        
        printf("Magic number for bishops:\n\n");

        for (int attempt = 0; attempt < 100; attempt++) { // Try for 100 times
            int i;
            for (i = A8; i <= H1; i++) {
                u64 magic_number = findMagic(i, true);
                if (magic_number == 0ULL) {
                    printf("\nAttempt %d failed, retrying...\n", attempt);
                    break;
                }
                printf("0x%llxULL,\n", magic_number);
            }
            if (i >= 64) {
                printf("\nMagic bishop success!\n\n");
                break;
            }
        }

        printf("Magic number for rooks:\n\n");

        for (int attempt = 0; attempt < 100; attempt++) { // Try for 100 times
            int i;
            for (i = A8; i <= H1; i++) {
                u64 magic_number = findMagic(i, false);
                if (magic_number == 0ULL) {
                    printf("\nAttempt %d failed, retrying...\n", attempt);
                    break;
                }
                printf("0x%llxULL,\n", magic_number);
            }
            if (i >= H1) {
                printf("\nMagic rook success!\n\n");
                break;
            }
        }
    }
}