#pragma once

namespace Chess
{

static constexpr const int kWhiteIndex = 0;
static constexpr const int kBlackIndex = 1;

static constexpr const char kPawn = 'p';
static constexpr const char kKnight = 'n';
static constexpr const char kBishop = 'b';
static constexpr const char kRook = 'r';
static constexpr const char kQueen = 'q';
static constexpr const char kKing = 'k';

static constexpr const uint8_t kPieceTypeMask = 0b00000111;
static constexpr const uint8_t kPieceColorMask = 0b00001000;

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

} // namespace Chess
