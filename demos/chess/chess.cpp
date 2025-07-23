#include "chess.h"
#include "fen.h"
#include <sstream>

namespace Chess
{

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
