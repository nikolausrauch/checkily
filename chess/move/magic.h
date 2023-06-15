#pragma once

#include "../chess.h"
#include "../bitboard.h"

#include <random>

namespace chess::magic
{

/**************************** RELEVANT BITS ****************************/
constexpr std::array<int, square::square_count> bishop_relevant_bits =
{
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
constexpr int bishop_bits(const square sq) { return bishop_relevant_bits[sq]; }

constexpr std::array<int, square::square_count> rook_relevant_bits =
{
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
constexpr int rook_bits(const square sq) { return rook_relevant_bits[sq]; }


/**************************** GENERATED MAGIC NUMBERS ****************************/
constexpr std::array<std::uint64_t, square::square_count> bishop_magic_number =
{
    11719124551521861696ull, 17296294533302625408ull, 9056854857989767168ull,  5788255950251294720ull,
    377746089113060370ull,   4366805058650383360ull,  1664653110410249216ull,  7693419200300589096ull,
    162415837714056320ull,   18066005914151944706ull, 15294831437189160963ull, 6064110316042924048ull,
    11210264384431137168ull, 11557407628166496768ull, 10060631570074567680ull, 2454257364359057408ull,
    14635648518601590272ull, 16018768378886620160ull, 12337624167171560592ull, 1333632889260212225ull,
    7821631059491096196ull,  11338094352947610113ull, 1660154960575268944ull,  92539782328484864ull,
    644964778583786544ull,   8954880282328566336ull,  17870322997643854848ull, 1177700099806068768ull,
    3884798884584628224ull,  802210831797854212ull,   112882518051013632ull,   4948926578415108352ull,
    6056266457362804736ull,  9732843601893003264ull,  17315180638463067136ull, 14748428967997866112ull,
    1820600498920358148ull,  3409228234851746306ull,  12803847253405073920ull, 11108868059300758016ull,
    9567633564282233088ull,  14255382831221385272ull, 10639706016700174345ull, 8312892709086169088ull,
    6058495067235090944ull,  2958676977449500928ull,  436370142328463872ull,   14068123203702231552ull,
    1664653110410249216ull,  4645605593650823168ull,  11530007261386768448ull, 10564116951906127872ull,
    3596653185002110976ull,  10119061508212785280ull, 481916188358871594ull,   17296294533302625408ull,
    7693419200300589096ull,  2454257364359057408ull,  649418318089883649ull,   7557907094150973442ull,
    8238051658466141184ull,  5296536905385648768ull,  162415837714056320ull,   11719124551521861696ull
};
constexpr std::uint64_t bishop_magic(const square sq) { return bishop_magic_number[sq]; }

constexpr std::array<std::uint64_t, square::square_count> rook_magic_number =
{
    36046938962935808ull,    13907151933480648704ull, 72076286272487432ull,   1333083084052269056ull,
    11673347860759314944ull, 144117387116413968ull,   1297041092893999616ull, 2449958472712782340ull,
    140738302066696ull,      1153625329542103184ull,  9295711178888060928ull, 4614782382908772353ull,
    140771848355970ull,      9575075037459776000ull,  291889568045597184ull,  621637590495265024ull,
    54053640907870336ull,    6896687759499264ull,     1657465950653386756ull, 1155173854318956544ull,
    1155173854243487752ull,  2594214672676684289ull,  4398085343233ull,       2432016366555907073ull,
    1188986110915657728ull,  153210349334568961ull,   9295711178888060928ull, 9237164452448567304ull,
    18018798704001152ull,    4400202383872ull,        563267781560596ull,     9225764589189253376ull,
    4647714953023717441ull,  4504836590534729ull,     4620698715525496864ull, 1155173854318956544ull,
    648659118197834752ull,   9575075037459776000ull,  291889568045597184ull,  140740751532288ull,
    144467033984237568ull,   37718748593782816ull,    4614010388157169696ull, 9223408321006993418ull,
    9234631070568218628ull,  73183549779673097ull,    144195658990223362ull,  10376329826427600900ull,
    9232519974151536896ull,  7064037028033204352ull,  20302759300563456ull,   4521193961423232ull,
    2260595973849216ull,     563499776344704ull,      295126517376221312ull,  577024097469530624ull,
    143027790283010ull,      143027790283010ull,      863117164677825ull,     8147018323520536846ull,
    2311191051756896291ull,  112871482842452561ull,   281560909611169ull,     2306124484731601295ull,
};
constexpr std::uint64_t rook_magic(const square sq) { return rook_magic_number[sq]; }


/**************************** MASKS FOR HASH GENERATION ****************************/
constexpr bitboard bishop_mask(const square sq)
{
    bitboard mask = 0x0;

    int b_r = sq / 8;
    int b_f = sq % 8;

    int r = 0, f = 0;
    for(r = b_r + 1, f = b_f + 1; r <= 6 && f <= 6; r++, f++) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r - 1, f = b_f + 1; r >= 1 && f <= 6; r--, f++) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r + 1, f = b_f - 1; r <= 6 && f >= 1; r++, f--) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r - 1, f = b_f - 1; r >= 1 && f >= 1; r--, f--) { mask |= (1ull << (r*8 + f)); }

    return mask;
}

constexpr bitboard rook_mask(const square sq)
{
    bitboard mask = 0x0;

    int b_r = sq / 8;
    int b_f = sq % 8;

    int r = 0, f = 0;
    for(r = b_r + 1, f = b_f; r <= 6; r++) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r - 1, f = b_f; r >= 1; r--) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r, f = b_f + 1; f <= 6; f++) { mask |= (1ull << (r*8 + f)); }
    for(r = b_r, f = b_f - 1; f >= 1; f--) { mask |= (1ull << (r*8 + f)); }

    return mask;
}

constexpr bitboard occupancy(int index, bitboard attack)
{
    bitboard occupancy = 0x0;

    int bits_in_mask = bit_count(attack);
    for(int c = 0; c < bits_in_mask; c++)
    {
        auto sq = bit_ffs(attack);
        bit_pop(attack, sq);

        if(index & (1ull << c))
        {
            occupancy |= 1ull << sq;
        }
    }

    return occupancy;
}


/**************************** MAGIC NUMBER COMPUTATION ****************************/
typedef bitboard(*mask_func)(const square);
typedef int(*bits_func)(const square);
typedef bitboard(*attack_func)(const square, const bitboard);

template<mask_func PieceMaskFunc, bits_func PieceBitsFunc, attack_func OtfAttackFunc>
std::uint64_t find_magic_number(const square sq)
{
    std::array<bitboard, 4096> occupancies;
    std::array<bitboard, 4096> attacks;
    std::array<bitboard, 4096> used_attacks;

    auto attack_mask = PieceMaskFunc(sq);
    auto relevant_bits = PieceBitsFunc(sq);

    auto indices = 1 << relevant_bits;
    for(int index = 0; index < indices; index++)
    {
        occupancies[index] = occupancy(index, attack_mask);
        attacks[index] = OtfAttackFunc(sq, occupancies[index]);
    }

    std::uniform_int_distribution<std::uint64_t> random_u64;
    std::mt19937 rng;

    for(int c = 0; c < 1024*1024*1024; c++)
    {
        auto magic = random_u64(rng) & random_u64(rng) & random_u64(rng);
        if(bit_count((magic * attack_mask) & 0xFF00000000000000) < 6) continue;

        std::fill(used_attacks.begin(), used_attacks.end(), 0ull);

        bool failed = false;
        for(int index = 0; index < indices; index++)
        {
            int magic_index = (occupancies[index] * magic) >> (64 - relevant_bits);

            if(used_attacks[magic_index] == 0ull)
            {
                used_attacks[magic_index] = attacks[index];
            }
            else
            {
                failed = true;
                break;
            }
        }

        if(!failed)
        {
            return magic;
        }
    }

    return 0ull;
}

struct entry
{
    std::uint64_t m_magic{0};
    bitboard m_mask{0};
    int m_bits{0};
};

}
