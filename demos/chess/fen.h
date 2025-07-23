#pragma once

#include "chess.h"
#include "piece.h"
#include <string>

namespace Chess
{

static void loadFromFEN(const std::string& fen, Chess* chess)
{
    auto index = 0;
    auto i = 0;

    // Parse piece placement
    for (; i < fen.size() && index < 64; ++i)
    {
        auto c = fen[i];
        if (c == ' ')
        {
            break;
        }

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
            auto type = PieceType::None;
            auto color = std::isupper(c) ? PieceColor::White : PieceColor::Black;

            auto lower = std::tolower(c);
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

    if (fen[i] == ' ')
    {
        ++i;
    }

    // Parse active color
    char sideToMove = fen[i];
    if (sideToMove == kWhite)
    {
        printf("Side to move: White\n");
    }
    else if (sideToMove == kBlack)
    {
        printf("Side to move: Black\n");
    }
    ++i; // move past 'w' or 'b'

    if (fen[i] == ' ')
    {
        ++i;
    }

    // Parse castling rights
    std::string castlingRights;
    while (i < fen.size() && fen[i] != ' ')
    {
        castlingRights += fen[i++];
    }

    printf("Castling rights: %s\n", castlingRights.c_str());
    printf("White can castle kingside: %d\n", (castlingRights.find(kWhiteKing) != std::string::npos));
    printf("White can castle queenside: %d\n", (castlingRights.find(kWhiteQueen) != std::string::npos));
    printf("Black can castle kingside: %d\n", (castlingRights.find(kBlackKing) != std::string::npos));
    printf("Black can castle queenside: %d\n", (castlingRights.find(kBlackQueen) != std::string::npos));

    if (fen[i] == ' ')
    {
        ++i;
    }

    // Parse en passant target
    std::string epSquare;
    while (i < fen.size() && fen[i] != ' ')
    {
        epSquare += fen[i++];
    }

    printf("En passant target: %s\n", epSquare.c_str());

    if (fen[i] == ' ')
    {
        ++i;
    }

    // Parse halfmove clock
    std::string halfmoveStr;
    while (i < fen.size() && fen[i] != ' ')
    {
        halfmoveStr += fen[i++];
    }

    printf("Halfmove clock: %d\n", std::stoi(halfmoveStr));

    if (fen[i] == ' ')
    {
        ++i;
    }

    // Parse fullmove number
    std::string fullmoveStr;
    while (i < fen.size() && fen[i] != ' ')
    {
        fullmoveStr += fen[i++];
    }

    printf("Fullmove number: %d\n", std::stoi(fullmoveStr));
}

} // namespace Chess
