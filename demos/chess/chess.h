#pragma once

#include <array>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

#include "bitboard.h"
#include "piece.h"

namespace chess
{

struct Move
{
    Piece piece;
    uint8_t from;
    uint8_t to;
};

struct Undo
{
    Move move;
    Piece captured;
    CastlingRights oldCastlingRights;
    CastlingRights newCastlingRights;
};

class Chess
{
  public:
    Chess();

    const char* GetSAN(const uint8_t square) const;

    void Clear(void);
    void Load(const std::string& fen);
    void Reset(void);
    bool MovePiece(uint8_t from, uint8_t to);
    Piece GetPiece(uint8_t square) const;
    void PutPiece(Piece piece, uint8_t square);
    void RemovePiece(uint8_t square);

    void Undo(void);
    void Redo(void);

    const std::vector<Piece> GetBoard(void) const;
    const std::string GetZobrist(void) const;

    const Bitboard GetRooks(void) const;
    const Bitboard GetBishops(void) const;
    const Bitboard GetQueens(void) const;
    const Bitboard GetKnights(void) const;
    const Bitboard GetPawns(void) const;
    const Bitboard GetKings(void) const;
    const Bitboard GetOccupied(PieceColor color) const;
    const Bitboard GetOpponentAttacks() const;
    const Bitboard GetOpponentAttacksToSquare(uint8_t square) const;
    const Bitboard GetPossibleMoves(const Piece piece, uint8_t square) const;

    const PieceColor GetTurn(void) const { return turn; }
    void SetTurn(const PieceColor color) { turn = color; }

    const CastlingRights GetCastlingRights(void) const { return castlingRights; }
    void SetCastlingRights(CastlingRights rights) { castlingRights |= rights; }

    bool InCheck(void) const;
    bool InCheckmate(void) const;

    const std::vector<chess::Move> Moves() const;
    const std::vector<chess::Move> MovesFromSquare(uint8_t square) const;
    const std::vector<chess::Move> MovesForPiece(Piece piece) const;

  private:
    bool IsValidMove(int from, int to) const;
    bool IsCastlingMove(uint8_t from, uint8_t to, Piece movingPiece);

    const Bitboard GeneratePawnMoves(uint8_t square) const;
    const Bitboard GenerateKnightMoves(uint8_t square) const;
    const Bitboard GenerateBishopMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateRookMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateQueenMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateKingMoves(uint8_t square) const;

    void UpdateZobristMove(Piece moving,
                           uint8_t from,
                           uint8_t to,
                           Piece captured,
                           CastlingRights prevCastlingRights,
                           CastlingRights newCastlingRights);

  private:
    PieceColor turn;
    CastlingRights castlingRights;

    Piece squares[64];
    Bitboard rooks[2];
    Bitboard bishops[2];
    Bitboard queens[2];
    Bitboard knights[2];
    Bitboard pawns[2];
    Bitboard kings[2];

    uint64_t hash;
    std::vector<chess::Undo> undo_stack;
    std::vector<chess::Undo> redo_stack;
};

} // namespace chess