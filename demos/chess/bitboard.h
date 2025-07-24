#pragma once

#include <array>

#include "consts.h"

namespace chess
{

using Bitboard = uint64_t;

constexpr Bitboard SquareMask(int rank, int file)
{
    return 1ULL << (rank * 8 + file);
}

constexpr std::array<int, kBoardSize> dr_king = {-1, -1, -1, 0, 1, 1, 1, 0};
constexpr std::array<int, kBoardSize> df_king = {-1, 0, 1, 1, 1, 0, -1, -1};

constexpr Bitboard KingMask(int square)
{
    int rank = square / 8;
    int file = square % 8;
    Bitboard mask = 0x0000;

    for (auto i = 0; i < 8; ++i)
    {
        auto r = rank + dr_king[i];
        auto f = file + df_king[i];
        if (r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKingMasks()
{
    std::array<Bitboard, kBoardSize> masks = {};
    for (auto sq = 0; sq < kBoardSize; ++sq)
    {
        masks[sq] = KingMask(sq);
    }
    return masks;
}

constexpr auto KingMasks = InitKingMasks();

constexpr std::array<int, kBoardSize> dr_knight = {-2, -1, 1, 2, 2, 1, -1, -2};
constexpr std::array<int, kBoardSize> df_knight = {1, 2, 2, 1, -1, -2, -2, -1};

constexpr Bitboard KnightMask(int square)
{
    int rank = square / 8;
    int file = square % 8;
    Bitboard mask = 0;

    for (auto i = 0; i < 8; ++i)
    {
        auto r = rank + dr_knight[i];
        auto f = file + df_knight[i];
        if (r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKnightMasks()
{
    std::array<Bitboard, kBoardSize> masks = {};
    for (auto sq = 0; sq < kBoardSize; ++sq)
    {
        masks[sq] = KnightMask(sq);
    }
    return masks;
}

constexpr std::array<int, kBoardSize> dr_rook = {-2, -1, 1, 2, 2, 1, -1, -2};
constexpr std::array<int, kBoardSize> df_rook = {1, 2, 2, 1, -1, -2, -2, -1};

constexpr Bitboard RookMask(int square)
{
    int rank = square / 8;
    int file = square % 8;
    Bitboard mask = 0;

    for (auto i = 0; i < 8; ++i)
    {
        auto r = rank + dr_rook[i];
        auto f = file + df_rook[i];
        if (r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitRookMasks()
{
    std::array<Bitboard, kBoardSize> masks = {};
    for (auto sq = 0; sq < kBoardSize; ++sq)
    {
        masks[sq] = KnightMask(sq);
    }
    return masks;
}

constexpr auto RookMasks = InitRookMasks();

} // namespace chess