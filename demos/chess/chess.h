#pragma once

#include <array>
#include <cctype>
#include <string>
#include <vector>

namespace Chess
{

static constexpr int kWidth = 8;
static constexpr int kHeight = 8;

enum Piece : uint8_t
{
    None = 0,
    King = 1,
    Pawn = 2,
    Knight = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,

    White = 8,
    Black = 16,
};

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

    void Move(int from, int to)
    {
        if (from == to)
            return;
        if (from < 0 || from >= 64 || to < 0 || to >= 64)
            throw std::out_of_range("Move indices out of range");
        board[to] = board[from];
        board[from] = Piece::None;
    }

    void loadFromFEN(const std::string& fen)
    {
        // Clear the board first
        for (auto i = 0; i < 64; i++)
        {
            board[i] = Piece::None;
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
                    board[index++] = Piece::None; // empty square
                }
            }
            else
            {
                auto color = std::isupper(c) ? Piece::White : Piece::Black;
                auto lower = std::tolower(c);
                auto piece = Piece::None;

                switch (lower)
                {
                case 'k':
                    piece = Piece::King;
                    break;
                case 'p':
                    piece = Piece::Pawn;
                    break;
                case 'n':
                    piece = Piece::Knight;
                    break;
                case 'b':
                    piece = Piece::Bishop;
                    break;
                case 'r':
                    piece = Piece::Rook;
                    break;
                case 'q':
                    piece = Piece::Queen;
                    break;
                default:
                    piece = Piece::None;
                    break;
                }
                board[index++] = color | piece;
            }
        }
    }
};

} // namespace Chess