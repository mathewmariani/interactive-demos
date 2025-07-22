#include "chess.h"
#include "fen.h"
#include <sstream>

namespace Chess
{

static constexpr const char* kSANPositions[] = {
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

Chess::Chess()
{
    Reset();
}

const char* Chess::GetSAN(const uint8_t square) const
{
    return kSANPositions[square];
}

void Chess::Reset()
{
    Load(kDefaultPosition);

    turn = PieceColor::White;
}

void Chess::Clear()
{
    Load(KEmptyPostion);
}

void Chess::Load(const std::string& fen)
{
    board.Clear();
    loadFromFEN(fen, this);
    board.Initialize();
}

void Chess::Move(uint8_t from, uint8_t to)
{
    if (GetPieceColor(board.GetPiece(from)) != turn)
    {
        return;
    }

    // check square for piece
    auto current_piece = board.GetPiece(to);
    if (GetPieceType(current_piece) != PieceType::None)
    {
        if (GetPieceColor(current_piece) == turn)
        {
            return;
        }
    }

    board.MovePiece(from, to);
    turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

void Chess::Put(Piece piece, uint8_t square)
{
    board.AddPiece(piece, square);
}

void Chess::Remove(uint8_t square)
{
    board.RemovePiece(square);
}

const std::vector<uint8_t> Chess::GetBoard() const
{
    const auto* data = board.Data();
    return std::vector<uint8_t>(data, data + 64);
}

const std::string Chess::GetZobrist() const
{
    return board.Hash();
}

} // namespace Chess
