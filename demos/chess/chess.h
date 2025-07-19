#pragma once

#include <array>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

#include "board.h"
#include "piece.h"

namespace Chess
{

class Chess
{
  public:
    Chess();

    void Clear(void);
    void Load(const std::string& fen);
    void Reset(void);
    void Move(int from, int to);
    void Put(Piece piece, int square);
    void Remove(int square);

    std::vector<uint8_t> GetBoard() const;
    Bitboard GetRooks() const { return board.GetRooks(); }
    Bitboard GetBishops() const { return board.GetBishops(); }
    Bitboard GetQueens() const { return board.GetQueens(); }
    Bitboard GetKnights() const { return board.GetKnights(); }
    Bitboard GetPawns() const { return board.GetPawns(); }
    Bitboard GetKings() const { return board.GetKings(); }

  private:
    Board board;
    PieceColor turn;
};

} // namespace Chess