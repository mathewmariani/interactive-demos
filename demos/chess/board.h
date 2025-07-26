#pragma once

#include "bitboard.h"
#include "piece.h"
#include "zobrist.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace chess
{

struct Undo
{
    struct
    {
        uint8_t from;
        uint8_t to;
    } move;
    Piece captured;
    uint64_t hash;
};

class Board
{
  public:
    Board(void) = default;

    void Initialize(void)
    {
        hash = computeZobristHash(squares, turn, 0x0, 0);
    }

    const Bitboard GetOccupied(const PieceColor color) const
    {
        return rooks[color] | bishops[color] | queens[color] |
               knights[color] | pawns[color] | kings[color];
    }

    const Bitboard GetRooks() const { return rooks[kWhiteIndex] | rooks[kBlackIndex]; }
    const Bitboard GetBishops() const { return bishops[kWhiteIndex] | bishops[kBlackIndex]; }
    const Bitboard GetQueens() const { return queens[kWhiteIndex] | queens[kBlackIndex]; }
    const Bitboard GetKnights() const { return knights[kWhiteIndex] | knights[kBlackIndex]; }
    const Bitboard GetPawns() const { return pawns[kWhiteIndex] | pawns[kBlackIndex]; }
    const Bitboard GetKings() const { return kings[kWhiteIndex] | kings[kBlackIndex]; }

    const PieceColor GetTurn() const { return turn; }
    void SetTurn(const PieceColor color) { turn = color; }

    void Clear(void)
    {
        for (auto i = 0; i < 64; i++)
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

    void MovePiece(uint8_t from, uint8_t to)
    {
        if (from == to)
            return;
        if (from < 0 || from >= 64 || to < 0 || to >= 64)
            throw std::out_of_range("Move indices out of range");

        Piece moving = squares[from];
        Piece captured = squares[to];

        PieceType moving_type = GetPieceType(moving);
        PieceColor moving_color = GetPieceColor(moving);

        if (moving_color != turn)
        {
            return;
        }

        // check square for piece
        // auto current_piece = board.GetPiece(to);
        // if (moving_type != PieceType::None)
        // {
        //     if (GetPieceColor(current_piece) == turn)
        //     {
        //         return;
        //     }
        // }

        // update the hash
        hash ^= zobrist.psq[moving][from];
        hash ^= zobrist.psq[moving][to];
        hash ^= zobrist.side;
        if (captured != PieceType::None)
        {
            hash ^= zobrist.psq[captured][to];
        }

        RemovePiece(to);
        RemovePiece(from);
        AddPiece(moving, to);

        undo_stack.push_back(Undo{{from, to}, captured, hash});
        redo_stack.clear();

        turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    }

    void UndoMove(void)
    {
        if (undo_stack.empty())
        {
            return;
        }

        auto prev = undo_stack.back();
        undo_stack.pop_back();

        auto piece = squares[prev.move.to];
        RemovePiece(prev.move.to);
        RemovePiece(prev.move.from);

        AddPiece(piece, prev.move.from);
        AddPiece(prev.captured, prev.move.to);

        hash = prev.hash;

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

        Piece moving = squares[next.move.from];
        Piece captured = next.captured;

        // update the hash
        hash ^= zobrist.psq[moving][next.move.from];
        hash ^= zobrist.psq[moving][next.move.to];
        hash ^= zobrist.side;
        if (captured != PieceType::None)
        {
            hash ^= zobrist.psq[captured][next.move.to];
        }

        RemovePiece(next.move.to);
        RemovePiece(next.move.from);
        AddPiece(moving, next.move.to);

        undo_stack.push_back(next);

        turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    }

    const Bitboard GetPossibleMoves(const Piece piece, uint8_t square) const
    {
        const auto type = GetPieceType(piece);
        const auto color = GetPieceColor(piece);

        Bitboard possible_moves = kEmptyBitboard;

        PieceColor opponent = GetTurn() == PieceColor::White ? PieceColor::Black : PieceColor::White;

        Bitboard empty = ~GetOccupied(PieceColor::White) & ~GetOccupied(PieceColor::Black);
        Bitboard enemy = GetOccupied(opponent);

        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::King:
            possible_moves = KingMasks[square];
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
        }

        // check overlap with same colored pieces
        possible_moves &= ~GetOccupied(color);

        return possible_moves;
    }

    const uint8_t* Data(void) const
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