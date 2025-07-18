#pragma once

#include <array>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

#include "piece.h"

namespace Chess
{

static constexpr int kWidth = 8;
static constexpr int kHeight = 8;

using Bitboard = uint64_t;

struct Move
{
    int piece;
    int from;
    int to;
};

struct State
{
    bool check;
    bool checkmate;
};

class Chess
{
  public:
    uint8_t board[64];

    Chess()
    {
        std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        loadFromFEN(fen);
    }

    std::vector<uint8_t> GetBoard() const
    {
        return std::vector<uint8_t>(board, board + 64);
    }

    Bitboard GetRooks() const { return rooks[kWhiteIndex] | rooks[kBlackIndex]; }
    Bitboard GetBishops() const { return bishops[kWhiteIndex] | bishops[kBlackIndex]; }
    Bitboard GetQueens() const { return queens[kWhiteIndex] | queens[kBlackIndex]; }
    Bitboard GetKnights() const { return knights[kWhiteIndex] | knights[kBlackIndex]; }
    Bitboard GetPawns() const { return pawns[kWhiteIndex] | pawns[kBlackIndex]; }
    Bitboard GetKings() const { return kings[kWhiteIndex] | kings[kBlackIndex]; }

    void Move(int from, int to)
    {
        if (from == to)
            return;
        if (from < 0 || from >= 64 || to < 0 || to >= 64)
            throw std::out_of_range("Move indices out of range");

        Piece piece = board[from];
        PieceType type = GetPieceType(piece);
        PieceColor color = GetPieceColor(piece);

        board[to] = piece;
        board[from] = PieceType::None;

        switch (type)
        {
        case PieceType::King:
            kings[color] &= ~(1ULL << from);
            kings[color] |= (1ULL << to);
            break;
        case PieceType::Pawn:
            pawns[color] &= ~(1ULL << from);
            pawns[color] |= (1ULL << to);
            break;
        case PieceType::Knight:
            knights[color] &= ~(1ULL << from);
            knights[color] |= (1ULL << to);
            break;
        case PieceType::Bishop:
            bishops[color] &= ~(1ULL << from);
            bishops[color] |= (1ULL << to);
            break;
        case PieceType::Rook:
            rooks[color] &= ~(1ULL << from);
            rooks[color] |= (1ULL << to);
            break;
        case PieceType::Queen:
            queens[color] &= ~(1ULL << from);
            queens[color] |= (1ULL << to);
            break;
        default:
            break;
        }
    }

    void loadFromFEN(const std::string& fen)
    {
        // Clear the board first
        for (auto i = 0; i < 64; i++)
        {
            board[i] = PieceType::None;
        }

        auto index = 0; // from 0 (a8) to 63 (h1)
        for (auto i = 0; i < fen.size() && index < 64; ++i)
        {
            char c = fen[i];
            if (c == ' ')
                break; // end of board layout part in FEN

            if (c == '/')
            {
                // next rank
                continue;
            }
            else if (std::isdigit(c))
            {
                // empty squares count
                int emptyCount = c - '0';
                for (int j = 0; j < emptyCount; ++j)
                {
                    board[index++] = PieceType::None; // empty square
                }
            }
            else
            {
                auto lower = std::tolower(c);
                auto type = PieceType::None;
                auto color = std::isupper(c) ? PieceColor::White : PieceColor::Black;

                switch (lower)
                {
                case 'k':
                    type = PieceType::King;
                    break;
                case 'p':
                    type = PieceType::Pawn;
                    break;
                case 'n':
                    type = PieceType::Knight;
                    break;
                case 'b':
                    type = PieceType::Bishop;
                    break;
                case 'r':
                    type = PieceType::Rook;
                    break;
                case 'q':
                    type = PieceType::Queen;
                    break;
                default:
                    type = PieceType::None;
                    break;
                }

                auto piece = MakePiece(color, type);
                AddPiece(piece, index++);
            }
        }
    }

    void AddPiece(Piece piece, int square)
    {
        if (piece == PieceType::None)
        {
            return;
        }

        board[square] = piece;

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

  private:
    Bitboard rooks[2];
    Bitboard bishops[2];
    Bitboard queens[2];
    Bitboard knights[2];
    Bitboard pawns[2];
    Bitboard kings[2];
};

} // namespace Chess