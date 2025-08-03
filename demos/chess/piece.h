#pragma once

#include "consts.h"

namespace chess
{

using Piece = uint8_t;

enum PieceColor : uint8_t
{
    White = 0,
    Black = 1,
};

enum PieceType : uint8_t
{
    None = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6,
};

enum CastlingRights : uint8_t
{
    WhiteKingSide = (1 << 0),
    WhiteQueenSide = (1 << 1),
    BlackKingSide = (1 << 2),
    BlackQueenSide = (1 << 3),
};

constexpr Piece MakePiece(PieceColor c, PieceType t)
{
    if (t == PieceType::None)
    {
        return PieceType::None;
    }
    return (c << 3) | t;
}

inline PieceColor GetPieceColor(const Piece piece)
{
    return static_cast<PieceColor>((piece & kPieceColorMask) >> 3);
}

inline PieceType GetPieceType(const Piece piece)
{
    return static_cast<PieceType>(piece & kPieceTypeMask);
}

// white pieces
constexpr const Piece WhitePawn = MakePiece(PieceColor::White, PieceType::Pawn);
constexpr const Piece WhiteKnight = MakePiece(PieceColor::White, PieceType::Knight);
constexpr const Piece WhiteBishop = MakePiece(PieceColor::White, PieceType::Bishop);
constexpr const Piece WhiteRook = MakePiece(PieceColor::White, PieceType::Rook);
constexpr const Piece WhiteQueen = MakePiece(PieceColor::White, PieceType::Queen);
constexpr const Piece WhiteKing = MakePiece(PieceColor::White, PieceType::King);

// black pieces
constexpr const Piece BlackPawn = MakePiece(PieceColor::Black, PieceType::Pawn);
constexpr const Piece BlackKnight = MakePiece(PieceColor::Black, PieceType::Knight);
constexpr const Piece BlackBishop = MakePiece(PieceColor::Black, PieceType::Bishop);
constexpr const Piece BlackRook = MakePiece(PieceColor::Black, PieceType::Rook);
constexpr const Piece BlackQueen = MakePiece(PieceColor::Black, PieceType::Queen);
constexpr const Piece BlackKing = MakePiece(PieceColor::Black, PieceType::King);

} // namespace chess
