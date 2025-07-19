#pragma once

#include "chess.h"
#include "piece.h"
#include <string>

namespace Chess
{

static constexpr const char* KEmptyPostion = "8/8/8/8/8/8/8/8 w - - 0 1";
static constexpr const char* kDefaultPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static void loadFromFEN(const std::string& fen, Chess* chess)
{
    auto index = 0;
    for (auto i = 0; i < fen.size() && index < 64; ++i)
    {
        auto c = fen[i];
        // if (c == ' ')
        // {
        //     break;
        // }

        if (c == '/')
        {
            continue;
        }
        else if (std::isdigit(c))
        {
            auto emptyCount = c - '0';
            for (auto j = 0; j < emptyCount; ++j)
            {
                chess->Put(PieceType::None, index++);
            }
        }
        else
        {
            auto lower = std::tolower(c);
            auto type = PieceType::None;
            auto color = std::isupper(c) ? PieceColor::White : PieceColor::Black;

            switch (lower)
            {
            case kPawn:
                type = PieceType::Pawn;
                break;
            case kKnight:
                type = PieceType::Knight;
                break;
            case kBishop:
                type = PieceType::Bishop;
                break;
            case kRook:
                type = PieceType::Rook;
                break;
            case kQueen:
                type = PieceType::Queen;
                break;
            case kKing:
                type = PieceType::King;
                break;
            default:
                type = PieceType::None;
                break;
            }

            auto piece = MakePiece(color, type);
            chess->Put(piece, index++);
        }
    }
}

} // namespace Chess
