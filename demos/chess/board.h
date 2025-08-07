#pragma once

#include "bitboard.h"
#include "piece.h"
#include "zobrist.h"
#include <algorithm>
#include <bit>
#include <sstream>
#include <string>
#include <vector>

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
    CastlingRights castlingRights;
};

class Board
{
  public:
    Board(void) = default;

    void Initialize(void)
    {
        hash = ComputeZobristHash(squares, turn, castlingRights, 0);
    }

    bool IsValidMove(int from, int to)
    {
        auto piece = GetPiece(from);
        auto possible = GetPossibleMoves(piece, from);
        return (possible & (1ULL << to)) != 0;
    }

    const Bitboard GetOccupied(const PieceColor color) const
    {
        return rooks[color] | bishops[color] | queens[color] |
               knights[color] | pawns[color] | kings[color];
    }

    const Bitboard GetRooks(void) const { return rooks[kWhiteIndex] | rooks[kBlackIndex]; }
    const Bitboard GetBishops(void) const { return bishops[kWhiteIndex] | bishops[kBlackIndex]; }
    const Bitboard GetQueens(void) const { return queens[kWhiteIndex] | queens[kBlackIndex]; }
    const Bitboard GetKnights(void) const { return knights[kWhiteIndex] | knights[kBlackIndex]; }
    const Bitboard GetPawns(void) const { return pawns[kWhiteIndex] | pawns[kBlackIndex]; }
    const Bitboard GetKings(void) const { return kings[kWhiteIndex] | kings[kBlackIndex]; }

    const PieceColor GetTurn(void) const { return turn; }
    void SetTurn(const PieceColor color) { turn = color; }

    void Clear(void)
    {
        for (auto i = 0; i < kNumSquares; i++)
        {
            squares[i] = PieceType::None;
        }

        std::fill(std::begin(rooks), std::end(rooks), kEmptyBitboard);
        std::fill(std::begin(bishops), std::end(bishops), kEmptyBitboard);
        std::fill(std::begin(queens), std::end(queens), kEmptyBitboard);
        std::fill(std::begin(knights), std::end(knights), kEmptyBitboard);
        std::fill(std::begin(pawns), std::end(pawns), kEmptyBitboard);
        std::fill(std::begin(kings), std::end(kings), kEmptyBitboard);

        undo_stack.clear();
        redo_stack.clear();
    }

    void AddPiece(Piece piece, uint8_t square)
    {
        if (piece == PieceType::None)
        {
            return;
        }

        squares[square] = piece;

        auto type = GetPieceType(piece);
        auto color = GetPieceColor(piece);
        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::King:
            kings[color] |= (1ULL << square);
            break;
        case PieceType::Pawn:
            pawns[color] |= (1ULL << square);
            break;
        case PieceType::Knight:
            knights[color] |= (1ULL << square);
            break;
        case PieceType::Bishop:
            bishops[color] |= (1ULL << square);
            break;
        case PieceType::Rook:
            rooks[color] |= (1ULL << square);
            break;
        case PieceType::Queen:
            queens[color] |= (1ULL << square);
            break;
        }
    }

    const Piece GetPiece(uint8_t square) const
    {
        return squares[square];
    }

    void RemovePiece(uint8_t square)
    {
        auto piece = squares[square];
        if (piece == PieceType::None)
        {
            return;
        }

        squares[square] = PieceType::None;

        auto type = GetPieceType(piece);
        auto color = GetPieceColor(piece);
        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::King:
            kings[color] &= ~(1ULL << square);
            break;
        case PieceType::Pawn:
            pawns[color] &= ~(1ULL << square);
            break;
        case PieceType::Knight:
            knights[color] &= ~(1ULL << square);
            break;
        case PieceType::Bishop:
            bishops[color] &= ~(1ULL << square);
            break;
        case PieceType::Rook:
            rooks[color] &= ~(1ULL << square);
            break;
        case PieceType::Queen:
            queens[color] &= ~(1ULL << square);
            break;
        }
    }

    CastlingRights GetCastlingRights(void) const
    {
        return castlingRights;
    }

    void SetCastlingRights(CastlingRights rights)
    {
        castlingRights |= rights;
    }

    bool MovePiece(uint8_t from, uint8_t to)
    {
        if ((from == to) || (from < 0 || from >= 64 || to < 0 || to >= 64))
        {
            return false;
        }

        auto moving = squares[from];
        auto captured = squares[to];

        if (!IsValidMove(from, to))
        {
            return false;
        }

        if (GetPieceColor(moving) != GetTurn())
        {
            return false;
        }

        // cache move
        undo_stack.push_back(Undo{{moving, from, to}, captured, castlingRights});
        redo_stack.clear();

        // update the hash
        auto cached_hash = hash;
        hash ^= zobrist.psq[moving][from];
        hash ^= zobrist.psq[moving][to];
        hash ^= zobrist.side;
        if (captured != PieceType::None)
        {
            hash ^= zobrist.psq[captured][to];
        }

        // move pieces
        RemovePiece(to);
        RemovePiece(from);
        AddPiece(moving, to);

        // castling rights
        const auto type = GetPieceType(moving);
        const auto color = GetPieceColor(moving);
        switch (type)
        {
        case PieceType::Rook:
        {
            switch (color)
            {
            case PieceColor::White:
                if (from == H1)
                {
                    castlingRights &= ~CastlingRights::WhiteKingSide;
                }
                else if (from == A1)
                {
                    castlingRights &= ~CastlingRights::WhiteQueenSide;
                }
                break;
            case PieceColor::Black:
                if (from == H8)
                {
                    castlingRights &= ~CastlingRights::BlackKingSide;
                }
                else if (from == A8)
                {
                    castlingRights &= ~CastlingRights::BlackQueenSide;
                }
                break;
            }
            break;
        }
        case PieceType::King:
        {
            switch (color)
            {
            case PieceColor::White:
                castlingRights &= ~(CastlingRights::WhiteKingSide | CastlingRights::WhiteQueenSide);
                break;
            case PieceColor::Black:
                castlingRights &= ~(CastlingRights::BlackKingSide | CastlingRights::BlackQueenSide);
                break;
            }
            break;
        }
        default:
            break;
        }

        turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;

        return true;
    }

    void UndoMove(void)
    {
        if (undo_stack.empty())
        {
            return;
        }

        auto prev = undo_stack.back();
        undo_stack.pop_back();

        // move pieces
        RemovePiece(prev.move.to);
        RemovePiece(prev.move.from);
        AddPiece(prev.move.piece, prev.move.from);
        AddPiece(prev.captured, prev.move.to);

        // game state
        castlingRights = prev.castlingRights;

        // update the hash
        auto cached_hash = hash;
        hash ^= zobrist.psq[prev.move.piece][prev.move.from];
        hash ^= zobrist.psq[prev.move.piece][prev.move.to];
        hash ^= zobrist.side;
        if (prev.captured != PieceType::None)
        {
            hash ^= zobrist.psq[prev.captured][prev.move.to];
        }

        redo_stack.push_back(prev);

        turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    }

    void RedoMove(void)
    {
        if (redo_stack.empty())
        {
            return;
        }

        auto next = redo_stack.back();
        redo_stack.pop_back();

        // move pieces
        RemovePiece(next.move.to);
        RemovePiece(next.move.from);
        AddPiece(next.move.piece, next.move.to);

        // game state
        // castlingRights = next.castlingRights;

        // update the hash
        hash ^= zobrist.psq[next.move.piece][next.move.from];
        hash ^= zobrist.psq[next.move.piece][next.move.to];
        hash ^= zobrist.side;
        if (next.captured != PieceType::None)
        {
            hash ^= zobrist.psq[next.captured][next.move.to];
        }

        undo_stack.push_back(next);

        turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    }

    bool InCheck(void) const
    {
        auto attacking = GetAttacking();
        auto king = kings[GetTurn()];
        return (king & attacking) != 0;
    }

    bool IsCheckmate(void) const
    {
        if (!InCheck())
        {
            return false;
        }

        auto square = MoveFromBitboard(kings[GetTurn()]);
        auto king_moves = KingMasks[square] & ~GetOccupied(GetTurn());
        auto safe_moves = king_moves & ~GetAttacking();

        // FIXME: need to know if we can capture the piece checking us

        return safe_moves == kEmptyBitboard;
    }

    const std::vector<Move> Moves(void) const
    {
        std::vector<Move> moves = {};
        for (auto sq = 0; sq < kNumSquares; sq++)
        {
            auto piece = GetPiece(sq);
            auto color = GetPieceColor(piece);
            if (piece == PieceType::None || color != GetTurn())
            {
                continue;
            }

            auto possible = GetPossibleMoves(piece, sq);
            while (possible)
            {
                auto to = MoveFromBitboard(possible);
                possible &= possible - 1;

                moves.push_back((Move){
                    .from = (uint8_t)sq,
                    .to = to,
                });
            }
        }
        return moves;
    }

    const std::vector<Move> MovesFromSquare(uint8_t square) const
    {
        std::vector<Move> moves = {};
        auto piece = GetPiece(square);
        auto color = GetPieceColor(piece);
        if (piece != PieceType::None || color == GetTurn())
        {
            auto possible = GetPossibleMoves(piece, square);
            while (possible)
            {
                auto to = MoveFromBitboard(possible);
                possible &= possible - 1;

                moves.push_back((Move){
                    .from = square,
                    .to = to,
                });
            }
        }

        return moves;
    }

    const std::vector<Move> MovesForPiece(Piece piece) const
    {
        std::vector<Move> moves = {};
        const auto type = GetPieceColor(piece);
        const auto color = GetPieceColor(piece);

        for (auto sq = 0; sq < kNumSquares; sq++)
        {
            if (piece != GetPiece(sq))
            {
                continue;
            }
            auto color = GetPieceColor(piece);
            if (piece == PieceType::None || color != GetTurn())
            {
                continue;
            }

            auto possible = GetPossibleMoves(piece, sq);
            while (possible)
            {
                auto to = MoveFromBitboard(possible);
                possible &= possible - 1;

                moves.push_back((Move){
                    .from = (uint8_t)sq,
                    .to = to,
                });
            }
        }
        return moves;
    }

    Bitboard GetAttacking(void) const
    {
        Bitboard attacks = kEmptyBitboard;

        auto occupancy = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

        for (auto i = 0; i < kNumSquares; ++i)
        {
            auto piece = GetPiece(i);
            if (piece == PieceType::None)
            {
                continue;
            }

            auto color = GetPieceColor(piece);
            if (color == GetTurn())
            {
                continue;
            }

            switch (GetPieceType(piece))
            {
            case PieceType::Pawn:
                attacks |= (color == PieceColor::White)
                               ? WhitePawnCaptureMasks[i]
                               : BlackPawnCaptureMasks[i];
                break;

            case PieceType::Knight:
                attacks |= KnightMasks[i];
                break;

            case PieceType::Bishop:
                attacks |= BishopMask(i, occupancy);
                break;

            case PieceType::Rook:
                attacks |= RookMask(i, occupancy);
                break;

            case PieceType::Queen:
                attacks |= QueenMask(i, occupancy);
                break;

            case PieceType::King:
                attacks |= KingMasks[i];
                break;

            default:
                break;
            }
        }

        return attacks;
    }

    const Bitboard GetPossibleMoves(const Piece piece, uint8_t square) const
    {
        Bitboard possible_moves = kEmptyBitboard;

        const auto type = GetPieceType(piece);
        const auto color = GetPieceColor(piece);

        const auto opponent = GetTurn() == PieceColor::White ? PieceColor::Black : PieceColor::White;

        const auto empty = ~GetOccupied(PieceColor::White) & ~GetOccupied(PieceColor::Black);
        const auto enemy = GetOccupied(opponent);

        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::Pawn:
        {
            Bitboard push = kEmptyBitboard;
            Bitboard double_push = kEmptyBitboard;
            Bitboard captures = kEmptyBitboard;
            switch (color)
            {
            case PieceColor::White:
                push = WhitePawnPushMasks[square];
                double_push = WhitePawnDoublePushMasks[square];
                captures = WhitePawnCaptureMasks[square] & enemy;
                break;
            case PieceColor::Black:
                push = BlackPawnPushMasks[square];
                double_push = BlackPawnDoublePushMasks[square];
                captures = BlackPawnCaptureMasks[square] & enemy;
                break;
            }

            auto push_valid = push & empty;
            if (push_valid)
            {
                double_push &= empty;
            }
            else
            {
                double_push = kEmptyBitboard;
            }

            possible_moves = push_valid | double_push | captures;
        }
        break;
        case PieceType::Knight:
            possible_moves = KnightMasks[square];
            break;
        case PieceType::Bishop:
        {
            auto blockers = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);
            possible_moves = BishopMask(square, blockers);
        }
        break;
        case PieceType::Rook:
        {
            auto blockers = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);
            possible_moves = RookMask(square, blockers);
        }
        break;
        case PieceType::Queen:
        {
            auto blockers = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);
            possible_moves = QueenMask(square, blockers);
        }
        break;
        case PieceType::King:
        {
            possible_moves = KingMasks[square];
            possible_moves &= ~GetAttacking();

            switch (color)
            {
            case PieceColor::White:
            {
                bool canWhiteCastleKingSide = (castlingRights & CastlingRights::WhiteKingSide) != CastlingRights::None;
                if (canWhiteCastleKingSide)
                {
                    possible_moves |= 1ULL << G1;
                }
                bool canWhiteCastleQueenSide = (castlingRights & CastlingRights::WhiteQueenSide) != CastlingRights::None;
                if (canWhiteCastleQueenSide)
                {
                    possible_moves |= 1ULL << C1;
                }
                break;
            }
            case PieceColor::Black:
                bool canBlackCastleKingSide = (castlingRights & CastlingRights::BlackKingSide) != CastlingRights::None;
                if (canBlackCastleKingSide)
                {
                    possible_moves |= 1ULL << G8;
                }
                bool canBlackCastleQueenSide = (castlingRights & CastlingRights::BlackQueenSide) != CastlingRights::None;
                if (canBlackCastleQueenSide)
                {
                    possible_moves |= 1ULL << C8;
                }
                break;
            }
        }
        break;
        }

        // check overlap with same colored pieces
        possible_moves &= ~GetOccupied(color);

        return possible_moves;
    }

    const Piece* Data(void) const
    {
        return squares;
    }

    const std::string Hash(void) const
    {
        std::stringstream ss;
        ss << std::hex << hash;
        return ss.str();
    }

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
    std::vector<Undo> undo_stack;
    std::vector<Undo> redo_stack;
};

} // namespace chess