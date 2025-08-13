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
    bool IsSquareAttacked(uint8_t square) const;
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

    inline constexpr const Bitboard GetPawns(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::Pawn)];
    }

    inline constexpr const Bitboard GetKnights(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::Knight)];
    }

    inline constexpr const Bitboard GetBishops(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::Bishop)];
    }

    inline constexpr const Bitboard GetRooks(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::Rook)];
    }

    inline constexpr const Bitboard GetQueens(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::Queen)];
    }

    inline constexpr const Bitboard GetKings(PieceColor turn) const
    {
        return pieces[static_cast<uint8_t>(turn)][static_cast<uint8_t>(PieceType::King)];
    }

  private:
    PieceColor turn;
    CastlingRights castlingRights;

    Piece squares[kNumSquares];
    Bitboard pieces[kNumColors][kNumPieces];

    uint64_t hash;
    std::vector<chess::Undo> undo_stack;
    std::vector<chess::Undo> redo_stack;
};

} // namespace chess