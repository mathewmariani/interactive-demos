#pragma once

#include <array>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

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
    uint8_t enPassantCaptureSquare;
    uint8_t oldEnPassant;
    uint8_t newEnPassant;
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
    const Bitboard GetAttacksOnSquare(uint8_t square, PieceColor from) const;
    const Bitboard GetAttacks(PieceColor from) const;

    const PieceColor GetTurn(void) const { return turn; }
    const PieceColor GetOpponent(void) const { return GetTurn() == PieceColor::White ? PieceColor::Black : PieceColor::White; }
    const PieceColor GetOpposite(PieceColor color) const { return color == PieceColor::White ? PieceColor::Black : PieceColor::White; }
    void SetTurn(const PieceColor color) { turn = color; }

    const CastlingRights GetCastlingRights(void) const { return castlingRights; }
    void SetCastlingRights(CastlingRights rights) { castlingRights |= rights; }

    bool InCheck(PieceColor turn) const;
    bool InCheckmate(void) const;

    const std::vector<chess::Move> Moves() const;
    const std::vector<chess::Move> MovesFromSquare(uint8_t square) const;
    const std::vector<chess::Move> MovesForPiece(Piece piece) const;

  private:
    bool IsValidMove(int from, int to) const;
    bool IsCastlingMove(uint8_t from, uint8_t to, Piece movingPiece) const;
    bool MoveLeadsToCheck(uint8_t from, uint8_t to) const;

    const Bitboard GeneratePawnMoves(uint8_t square) const;
    const Bitboard GenerateKnightMoves(uint8_t square) const;
    const Bitboard GenerateBishopMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateRookMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateQueenMoves(uint8_t square, const Bitboard blockers) const;
    const Bitboard GenerateKingMoves(uint8_t square) const;
    const Bitboard GenerateMovesForPieceAt(const Piece piece, uint8_t square) const;

    void UpdateZobristMove(Piece moving,
                           uint8_t from,
                           uint8_t to,
                           Piece captured,
                           uint8_t enPassant,
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
    uint8_t enPassantSquare;

    Piece board[kNumSquares];
    Bitboard pieces[kNumColors][kNumPieces];

    uint64_t hash;
    std::vector<chess::Undo> undoStack;
    std::vector<chess::Undo> redoStack;
};

} // namespace chess