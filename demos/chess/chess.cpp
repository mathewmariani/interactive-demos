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
}

void Chess::Clear()
{
    Load(KEmptyPostion);
}

void Chess::Load(const std::string& fen)
{
    loadFromFEN(fen, board);
}

void Chess::Move(int from, int to)
{
    board.Move(from, to);
}

void Chess::Put(Piece piece, int square)
{
    board.AddPiece(piece, square);
}

void Chess::Remove(int square)
{
    board.Remove(square);
}

std::vector<uint8_t> Chess::GetBoard() const
{
    return board.Data();
}

} // namespace Chess
