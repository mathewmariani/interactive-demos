#pragma once

namespace Chess
{

constexpr int kWhiteIndex = 0;
constexpr int kBlackIndex = 1;

constexpr uint8_t kPieceTypeMask = 0b00000111;
constexpr uint8_t kPieceColorMask = 0b00001000;

using Piece = uint8_t;

struct PieceList
{
    std::vector<int> occupied;
    std::vector<int> map;
    int count;

    void Add(int square)
    {
        occupied[count] = square;
        map[square] = count;
        count++;
    }

    void Remove(int square)
    {
        int index = map[square];
        occupied[index] = occupied[count - 1];
        map[occupied[index]] = index;
        count--;
    }

    int operator[](int index) const
    {
        return occupied[index];
    }

    int& operator[](int index)
    {
        return occupied[index];
    }
};

enum PieceColor : uint8_t
{
    White = 0,
    Black = 1,
};

enum PieceType : uint8_t
{
    None = 0,
    King = 1,
    Pawn = 2,
    Knight = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,
};

constexpr Piece MakePiece(PieceColor c, PieceType t)
{
    if (t == PieceType::None)
    {
        return 0;
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
constexpr Piece WhitePawn = MakePiece(PieceColor::White, PieceType::Pawn);
constexpr Piece WhiteKnight = MakePiece(PieceColor::White, PieceType::Knight);
constexpr Piece WhiteBishop = MakePiece(PieceColor::White, PieceType::Bishop);
constexpr Piece WhiteRook = MakePiece(PieceColor::White, PieceType::Rook);
constexpr Piece WhiteQueen = MakePiece(PieceColor::White, PieceType::Queen);
constexpr Piece WhiteKing = MakePiece(PieceColor::White, PieceType::King);

// black pieces
constexpr Piece BlackPawn = MakePiece(PieceColor::Black, PieceType::Pawn);
constexpr Piece BlackKnight = MakePiece(PieceColor::Black, PieceType::Knight);
constexpr Piece BlackBishop = MakePiece(PieceColor::Black, PieceType::Bishop);
constexpr Piece BlackRook = MakePiece(PieceColor::Black, PieceType::Rook);
constexpr Piece BlackQueen = MakePiece(PieceColor::Black, PieceType::Queen);
constexpr Piece BlackKing = MakePiece(PieceColor::Black, PieceType::King);

} // namespace Chess
