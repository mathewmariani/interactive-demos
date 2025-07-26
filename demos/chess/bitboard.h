#pragma once

#include <array>

#include "consts.h"

namespace chess
{

using Bitboard = uint64_t;

constexpr Bitboard SquareMask(int rank, int file)
{
    return 1ULL << (rank * kNumRanks + file);
}

constexpr Bitboard WhitePawnPushMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    if (rank <= 0)
    {
        return kEmptyBitboard;
    }
    return SquareMask(rank - 1, file);
}

constexpr Bitboard WhitePawnDoublePushMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    if (rank == 6)
    {
        return SquareMask(rank - 2, file);
    }
    return kEmptyBitboard;
}

constexpr Bitboard WhitePawnCaptureMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;

    Bitboard mask = kEmptyBitboard;
    if (rank > 0 && file > 0)
    {
        mask |= SquareMask(rank - 1, file - 1);
    }
    if (rank > 0 && file < 7)
    {
        mask |= SquareMask(rank - 1, file + 1);
    }
    return mask;
}

constexpr Bitboard BlackPawnPushMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    if (rank >= 7)
    {
        return kEmptyBitboard;
    }
    return SquareMask(rank + 1, file);
}

constexpr Bitboard BlackPawnDoublePushMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    if (rank == 1)
    {
        return SquareMask(rank + 2, file);
    }
    return kEmptyBitboard;
}

constexpr Bitboard BlackPawnCaptureMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;

    Bitboard mask = kEmptyBitboard;
    if (rank < 7 && file > 0)
    {
        mask |= SquareMask(rank + 1, file - 1);
    }
    if (rank < 7 && file < 7)
    {
        mask |= SquareMask(rank + 1, file + 1);
    }
    return mask;
}

constexpr auto InitWhitePawnPushMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = WhitePawnPushMask(i);
    }
    return arr;
}

constexpr auto InitWhitePawnDoublePushMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = WhitePawnDoublePushMask(i);
    }
    return arr;
}

constexpr auto InitWhitePawnCaptureMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = WhitePawnCaptureMask(i);
    }
    return arr;
}

constexpr auto InitBlackPawnPushMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = BlackPawnPushMask(i);
    }
    return arr;
}

constexpr auto InitBlackPawnDoublePushMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = BlackPawnDoublePushMask(i);
    }
    return arr;
}

constexpr auto InitBlackPawnCaptureMasks(void)
{
    std::array<Bitboard, kNumSquares> arr{};
    for (auto i = 0; i < kNumSquares; ++i)
    {
        arr[i] = BlackPawnCaptureMask(i);
    }
    return arr;
}

constexpr auto WhitePawnPushMasks = InitWhitePawnPushMasks();
constexpr auto WhitePawnDoublePushMasks = InitWhitePawnDoublePushMasks();
constexpr auto WhitePawnCaptureMasks = InitWhitePawnCaptureMasks();

constexpr auto BlackPawnPushMasks = InitBlackPawnPushMasks();
constexpr auto BlackPawnDoublePushMasks = InitBlackPawnDoublePushMasks();
constexpr auto BlackPawnCaptureMasks = InitBlackPawnCaptureMasks();

constexpr std::array<int, 8> dr_king = {-1, -1, -1, 0, 1, 1, 1, 0};
constexpr std::array<int, 8> df_king = {-1, 0, 1, 1, 1, 0, -1, -1};

constexpr Bitboard KingMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    Bitboard mask = kEmptyBitboard;

    for (auto i = 0; i < 8; ++i)
    {
        auto r = rank + dr_king[i];
        auto f = file + df_king[i];
        if (r >= 0 && r < kNumRanks && f >= 0 && f < kNumFiles)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKingMasks(void)
{
    std::array<Bitboard, kNumSquares> masks = {};
    for (auto sq = 0; sq < kNumSquares; ++sq)
    {
        masks[sq] = KingMask(sq);
    }
    return masks;
}

constexpr auto KingMasks = InitKingMasks();

constexpr std::array<int, 8> dr_knight = {-2, -1, 1, 2, 2, 1, -1, -2};
constexpr std::array<int, 8> df_knight = {1, 2, 2, 1, -1, -2, -2, -1};

constexpr Bitboard KnightMask(int square)
{
    int rank = square / kNumRanks;
    int file = square % kNumRanks;
    Bitboard mask = kEmptyBitboard;

    for (auto i = 0; i < 8; ++i)
    {
        auto r = rank + dr_knight[i];
        auto f = file + df_knight[i];
        if (r >= 0 && r < kNumRanks && f >= 0 && f < kNumFiles)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKnightMasks(void)
{
    std::array<Bitboard, kNumSquares> masks = {};
    for (auto sq = 0; sq < kNumSquares; ++sq)
    {
        masks[sq] = KnightMask(sq);
    }
    return masks;
}

constexpr auto KnightMasks = InitKnightMasks();

constexpr Bitboard BishopMask(int square, const Bitboard blockers)
{
    constexpr int deltas = 4;
    constexpr std::array<int, deltas> dr = {1, 1, -1, -1};
    constexpr std::array<int, deltas> df = {1, -1, 1, -1};

    Bitboard mask = kEmptyBitboard;

    const int rank = square / kNumRanks;
    const int file = square % kNumRanks;

    for (auto dir = 0; dir < deltas; ++dir)
    {
        auto r = rank;
        auto f = file;

        while (true)
        {
            r += dr[dir];
            f += df[dir];
            if (r < 0 || r >= kNumRanks || f < 0 || f >= kNumFiles)
            {
                break;
            }

            auto sq = r * kNumRanks + f;
            mask |= (1ULL << sq);

            if (blockers & (1ULL << sq))
            {
                break;
            }
        }
    }

    return mask;
}

constexpr Bitboard RookMask(int square, Bitboard blockers)
{
    constexpr int deltas = 4;
    constexpr std::array<int, deltas> dr = {1, -1, 0, 0};
    constexpr std::array<int, deltas> df = {0, 0, 1, -1};

    Bitboard mask = kEmptyBitboard;

    const int rank = square / kNumRanks;
    const int file = square % kNumRanks;

    for (auto dir = 0; dir < deltas; ++dir)
    {
        auto r = rank;
        auto f = file;

        while (true)
        {
            r += dr[dir];
            f += df[dir];
            if (r < 0 || r >= kNumRanks || f < 0 || f >= kNumFiles)
            {
                break;
            }

            auto sq = r * kNumRanks + f;
            mask |= (1ULL << sq);

            if (blockers & (1ULL << sq))
            {
                break;
            }
        }
    }

    return mask;
}

constexpr Bitboard QueenMask(int square, const Bitboard blockers)
{
    constexpr int deltas = 8;
    constexpr std::array<int, deltas> dr = {1, -1, 0, 0, 1, 1, -1, -1};
    constexpr std::array<int, deltas> df = {0, 0, 1, -1, 1, -1, 1, -1};

    Bitboard mask = kEmptyBitboard;

    const int rank = square / kNumRanks;
    const int file = square % kNumRanks;

    for (auto dir = 0; dir < deltas; ++dir)
    {
        auto r = rank;
        auto f = file;

        while (true)
        {
            r += dr[dir];
            f += df[dir];
            if (r < 0 || r >= kNumRanks || f < 0 || f >= kNumFiles)
            {
                break;
            }

            auto sq = r * kNumRanks + f;
            mask |= (1ULL << sq);

            if (blockers & (1ULL << sq))
            {
                break;
            }
        }
    }

    return mask;
}

} // namespace chess