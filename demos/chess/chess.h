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

    const std::vector<uint8_t> GetBoard(void) const;
    const std::string GetZobrist(void) const;

    Bitboard GetOppcupied(PieceColor color) const { return board.GetOccupied(color); }
    Bitboard GetRooks(void) const { return board.GetRooks(); }
    Bitboard GetBishops(void) const { return board.GetBishops(); }
    Bitboard GetQueens(void) const { return board.GetQueens(); }
    Bitboard GetKnights(void) const { return board.GetKnights(); }
    Bitboard GetPawns(void) const { return board.GetPawns(); }
    Bitboard GetKings(void) const { return board.GetKings(); }

    const PieceColor GetTurn(void) const { return board.GetTurn(); }
    void SetTurn(const PieceColor color) { return board.SetTurn(color); }

    const std::vector<chess::Move> Moves() const { return board.Moves(); }
    const std::vector<chess::Move> MovesFromSquare(uint8_t square) const { return board.MovesFromSquare(square); }
    const std::vector<chess::Move> MovesForPiece(Piece piece) const { return board.MovesForPiece(piece); }
    Bitboard GetPossibleMoves(const Piece piece, uint8_t square) const { return board.GetPossibleMoves(piece, square); }

  private:
    Board board;
};

} // namespace chess