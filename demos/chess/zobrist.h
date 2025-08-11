#pragma once

#include <array>
#include <cstdint>
#include <iostream>

#include "piece.h"

namespace chess
{

struct SplitMix64
{
    uint64_t state;

    constexpr SplitMix64(uint64_t seed) : state(seed) {}

    constexpr uint64_t next(void)
    {
        uint64_t z = (state += 0x9E3779B97f4A7C15ull);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }
};

struct Zobrist
{
    std::array<std::array<uint64_t, kNumSquares>, kNumPieces> psq;
    std::array<uint64_t, kCastlingBits> castling;
    std::array<uint64_t, kEnPassentBits> enpassant;
    uint64_t side;

    constexpr Zobrist(void) : psq{}, castling{}, enpassant{}, side{}
    {
        SplitMix64 rng(2025);

        for (int p = 0; p < kNumPieces; ++p)
        {
            for (int s = 0; s < kNumSquares; ++s)
            {
                psq[p][s] = rng.next();
            }
        }

        for (int i = 0; i < kCastlingBits; ++i)
        {
            castling[i] = rng.next();
        }

        for (int f = 0; f < kEnPassentBits; ++f)
        {
            enpassant[f] = rng.next();
        }

        side = rng.next();
    }
};

constexpr Zobrist zobrist = Zobrist();

constexpr uint64_t ComputeZobristHash(const Piece board[kNumSquares],
                                      const PieceColor turn,
                                      const CastlingRights castlingRights,
                                      int epFile)
{
    uint64_t hash = 0;

    for (auto sq = 0; sq < kNumSquares; ++sq)
    {
        if (board[sq] != static_cast<uint8_t>(PieceType::None))
        {
            hash ^= zobrist.psq[board[sq] - 1][sq]; // -1 because EMPTY=0
        }
    }

    if (turn == PieceColor::White)
    {
        hash ^= zobrist.side;
    }

    hash ^= zobrist.castling[static_cast<uint8_t>(castlingRights)];

    if (epFile >= 0)
    {
        hash ^= zobrist.enpassant[epFile];
    }

    return hash;
}

} // namespace chess