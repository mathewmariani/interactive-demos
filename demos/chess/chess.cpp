#include "chess.h"
#include "fen.h"

namespace Chess
{

Chess::Chess()
{
    Reset();
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
}

void Chess::Move(int from, int to)
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

    board.Move(from, to);
    turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

void Chess::Put(Piece piece, int square)
{
    board.AddPiece(piece, square);
}

void Chess::Remove(int square)
{
    board.RemovePiece(square);
}

std::vector<uint8_t> Chess::GetBoard() const
{
    return board.Data();
}

} // namespace Chess
