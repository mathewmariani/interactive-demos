#pragma once

#include <array>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

#include "board.h"
#include "piece.h"

namespace chess
{

class Chess
{
  public:
    Chess();

    const char* GetSAN(const uint8_t square) const;

    void Clear(void);
    void Load(const std::string& fen);
    void Reset(void);
    void Move(uint8_t from, uint8_t to);
    void Put(Piece piece, uint8_t square);
    void Remove(uint8_t square);

    void Undo(void) { board.UndoMove(); }
    void Redo(void) { board.RedoMove(); }

    const std::vector<uint8_t> GetBoard() const;
    const std::string GetZobrist() const;

    Bitboard GetRooks() const { return board.GetRooks(); }
    Bitboard GetBishops() const { return board.GetBishops(); }
    Bitboard GetQueens() const { return board.GetQueens(); }
    Bitboard GetKnights() const { return board.GetKnights(); }
    Bitboard GetPawns() const { return board.GetPawns(); }
    Bitboard GetKings() const { return board.GetKings(); }

    Bitboard GetPossibleMoves(const Piece piece, uint8_t square) const { return board.GetPossibleMoves(piece, square); }

  private:
    Board board;
    PieceColor turn;
};

} // namespace chess