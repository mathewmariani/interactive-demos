#pragma once

#include "piece.h"
#include <algorithm>
#include <string>
#include <vector>

namespace Chess
{

using Bitboard = uint64_t;

static constexpr const char* kBoardSquares[] = {
    // clang-format off
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    // clang-format on
};

class Board
{
  public:
    Board() = default;

    Bitboard GetRooks() const { return rooks[kWhiteIndex] | rooks[kBlackIndex]; }
    Bitboard GetBishops() const { return bishops[kWhiteIndex] | bishops[kBlackIndex]; }
    Bitboard GetQueens() const { return queens[kWhiteIndex] | queens[kBlackIndex]; }
    Bitboard GetKnights() const { return knights[kWhiteIndex] | knights[kBlackIndex]; }
    Bitboard GetPawns() const { return pawns[kWhiteIndex] | pawns[kBlackIndex]; }
    Bitboard GetKings() const { return kings[kWhiteIndex] | kings[kBlackIndex]; }

    void Clear()
    {
        for (auto i = 0; i < 64; i++)
        {
            squares[i] = PieceType::None;
        }

        std::fill(std::begin(rooks), std::end(rooks), 0ULL);
        std::fill(std::begin(bishops), std::end(bishops), 0ULL);
        std::fill(std::begin(queens), std::end(queens), 0ULL);
        std::fill(std::begin(knights), std::end(knights), 0ULL);
        std::fill(std::begin(pawns), std::end(pawns), 0ULL);
        std::fill(std::begin(kings), std::end(kings), 0ULL);
    }

    void AddPiece(Piece piece, int square)
    {
        if (piece == PieceType::None)
        {
            return;
        }

        squares[square] = piece;

        auto type = GetPieceType(piece);
        auto color = GetPieceColor(piece);
        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::King:
            kings[color] |= (1ULL << square);
            break;
        case PieceType::Pawn:
            pawns[color] |= (1ULL << square);
            break;
        case PieceType::Knight:
            knights[color] |= (1ULL << square);
            break;
        case PieceType::Bishop:
            bishops[color] |= (1ULL << square);
            break;
        case PieceType::Rook:
            rooks[color] |= (1ULL << square);
            break;
        case PieceType::Queen:
            queens[color] |= (1ULL << square);
            break;
        }
    }

    void Remove(int square)
    {
        auto piece = squares[square];
        if (piece == PieceType::None)
        {
            return;
        }

        squares[square] = PieceType::None;

        auto type = GetPieceType(piece);
        auto color = GetPieceColor(piece);
        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::King:
            kings[color] &= ~(1ULL << square);
            break;
        case PieceType::Pawn:
            pawns[color] &= ~(1ULL << square);
            break;
        case PieceType::Knight:
            knights[color] &= ~(1ULL << square);
            break;
        case PieceType::Bishop:
            bishops[color] &= ~(1ULL << square);
            break;
        case PieceType::Rook:
            rooks[color] &= ~(1ULL << square);
            break;
        case PieceType::Queen:
            queens[color] &= ~(1ULL << square);
            break;
        }
    }

    void Move(int from, int to)
    {
        if (from == to)
            return;
        if (from < 0 || from >= 64 || to < 0 || to >= 64)
            throw std::out_of_range("Move indices out of range");

        Piece piece = squares[from];
        PieceType type = GetPieceType(piece);
        PieceColor color = GetPieceColor(piece);

        squares[to] = piece;
        squares[from] = PieceType::None;

        switch (type)
        {
        case PieceType::King:
            kings[color] &= ~(1ULL << from);
            kings[color] |= (1ULL << to);
            break;
        case PieceType::Pawn:
            pawns[color] &= ~(1ULL << from);
            pawns[color] |= (1ULL << to);
            break;
        case PieceType::Knight:
            knights[color] &= ~(1ULL << from);
            knights[color] |= (1ULL << to);
            break;
        case PieceType::Bishop:
            bishops[color] &= ~(1ULL << from);
            bishops[color] |= (1ULL << to);
            break;
        case PieceType::Rook:
            rooks[color] &= ~(1ULL << from);
            rooks[color] |= (1ULL << to);
            break;
        case PieceType::Queen:
            queens[color] &= ~(1ULL << from);
            queens[color] |= (1ULL << to);
            break;
        default:
            break;
        }
    }

    std::vector<uint8_t> Data() const
    {
        return std::vector<uint8_t>(squares, squares + 64);
    }

  private:
    Piece squares[64];
    Bitboard rooks[2];
    Bitboard bishops[2];
    Bitboard queens[2];
    Bitboard knights[2];
    Bitboard pawns[2];
    Bitboard kings[2];
};

} // namespace Chess