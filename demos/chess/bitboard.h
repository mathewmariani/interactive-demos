#pragma once

#include <array>

#include "consts.h"

namespace chess
{

using Bitboard = uint64_t;

constexpr Bitboard SquareMask(int rank, int file)
{
    return 1ULL << (rank * kBoardSize + file);
}

constexpr Bitboard WhitePawnPushMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    if (rank <= 0)
    {
        return kEmptyBitboard;
    }
    return SquareMask(rank - 1, file);
}

constexpr Bitboard WhitePawnDoublePushMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    if (rank == 6)
    {
        return SquareMask(rank - 2, file);
    }
    return kEmptyBitboard;
}

constexpr Bitboard WhitePawnCaptureMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;

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
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    if (rank >= 7)
    {
        return kEmptyBitboard;
    }
    return SquareMask(rank + 1, file);
}

constexpr Bitboard BlackPawnDoublePushMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    if (rank == 1)
    {
        return SquareMask(rank + 2, file);
    }
    return kEmptyBitboard;
}

constexpr Bitboard BlackPawnCaptureMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;

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

constexpr std::array<int, kBoardSize> dr_king = {-1, -1, -1, 0, 1, 1, 1, 0};
constexpr std::array<int, kBoardSize> df_king = {-1, 0, 1, 1, 1, 0, -1, -1};

constexpr Bitboard KingMask(int square)
{
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    Bitboard mask = 0x0000;

    for (auto i = 0; i < kBoardSize; ++i)
    {
        auto r = rank + dr_king[i];
        auto f = file + df_king[i];
        if (r >= 0 && r < kBoardSize && f >= 0 && f < kBoardSize)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKingMasks(void)
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
    int rank = square / kBoardSize;
    int file = square % kBoardSize;
    Bitboard mask = kEmptyBitboard;

    for (auto i = 0; i < kBoardSize; ++i)
    {
        auto r = rank + dr_knight[i];
        auto f = file + df_knight[i];
        if (r >= 0 && r < kBoardSize && f >= 0 && f < kBoardSize)
        {
            mask |= SquareMask(r, f);
        }
    }

    return mask;
}

constexpr auto InitKnightMasks(void)
{
    std::array<Bitboard, kBoardSize> masks = {};
    for (auto sq = 0; sq < kBoardSize; ++sq)
    {
        masks[sq] = KnightMask(sq);
    }
    return masks;
}

constexpr auto KnightMasks = InitKnightMasks();

constexpr Bitboard RookMask(int square, const Bitboard blockers)
{
    Bitboard attacks = kEmptyBitboard;

    int rank = square / kBoardSize;
    int file = square % kBoardSize;

    // Up (+8)
    for (auto r = rank + 1; r < kBoardSize; ++r)
    {
        auto sq = r * 8 + file;
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq))
        {
            break;
        }
    }

    // Down (-8)
    for (auto r = rank - 1; r >= 0; --r)
    {
        auto sq = r * kBoardSize + file;
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq))
        {
            break;
        }
    }

    // Right (+1)
    for (auto f = file + 1; f < kBoardSize; ++f)
    {
        auto sq = rank * kBoardSize + f;
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq))
        {
            break;
        }
    }

    // Left (-1)
    for (auto f = file - 1; f >= 0; --f)
    {
        auto sq = rank * kBoardSize + f;
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq))
        {
            break;
        }
    }

    return attacks;
}

} // namespace chess