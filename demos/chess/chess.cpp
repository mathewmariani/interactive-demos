#include "chess.h"
#include "fen.h"
#include <sstream>

namespace chess
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
}

void Chess::Clear()
{
    Load(KEmptyPostion);
}

void Chess::Load(const std::string& fen)
{
    board.Clear();
    loadFromFEN(fen, &board);
    board.Initialize();
}

void Chess::Move(uint8_t from, uint8_t to)
{
    board.MovePiece(from, to);
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

} // namespace chess
