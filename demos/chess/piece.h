#pragma once

namespace chess
{

static constexpr const int kWhiteIndex = 0;
static constexpr const int kBlackIndex = 1;

static constexpr const uint8_t kPieceTypeMask = 0b00000111;
static constexpr const uint8_t kPieceColorMask = 0b00001000;

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

// black pconst ieces
constexpr const Piece BlackPawn = MakePiece(PieceColor::Black, PieceType::Pawn);
constexpr const Piece BlackKnight = MakePiece(PieceColor::Black, PieceType::Knight);
constexpr const Piece BlackBishop = MakePiece(PieceColor::Black, PieceType::Bishop);
constexpr const Piece BlackRook = MakePiece(PieceColor::Black, PieceType::Rook);
constexpr const Piece BlackQueen = MakePiece(PieceColor::Black, PieceType::Queen);
constexpr const Piece BlackKing = MakePiece(PieceColor::Black, PieceType::King);

} // namespace chess
