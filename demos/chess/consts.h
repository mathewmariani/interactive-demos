#pragma once

namespace chess
{

using Bitboard = uint64_t;

constexpr const char* KEmptyPostion = "8/8/8/8/8/8/8/8 w - - 0 1";
constexpr const char* kDefaultPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

constexpr const char kPawn = 'p';
constexpr const char kKnight = 'n';
constexpr const char kBishop = 'b';
constexpr const char kRook = 'r';
constexpr const char kQueen = 'q';
constexpr const char kKing = 'k';

constexpr const char kWhite = 'w';
constexpr const char kWhiteKing = 'K';
constexpr const char kWhiteQueen = 'Q';

constexpr const char kBlack = 'b';
constexpr const char kBlackKing = 'k';
constexpr const char kBlackQueen = 'q';

constexpr const int kWhiteIndex = 0;
constexpr const int kBlackIndex = 1;
constexpr const uint8_t kNullPiece = 0b00000000;
constexpr const uint8_t kPieceTypeMask = 0b00000111;
constexpr const uint8_t kPieceColorMask = 0b00001000;

constexpr const int kNumRanks = 8; // x
constexpr const int kNumFiles = 8; // y
constexpr const int kNumSquares = kNumRanks * kNumFiles;
constexpr const int kNumPieces = 12;
constexpr const int kNumColors = 2;

// bitboard
constexpr const uint64_t kEmptyBitboard = 0ULL;

// zobrist
constexpr const int kCastlingBits = 16;
constexpr const int kEnPassentBits = 8;

// SAN
constexpr const char* kSANPositions[] = {
    // clang-format off
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    // clang-format on
};

constexpr const uint8_t A8 = 0;
constexpr const uint8_t B8 = 1;
constexpr const uint8_t C8 = 2;
constexpr const uint8_t D8 = 3;
constexpr const uint8_t E8 = 4;
constexpr const uint8_t F8 = 5;
constexpr const uint8_t G8 = 6;
constexpr const uint8_t H8 = 7;

constexpr const uint8_t A7 = 8;
constexpr const uint8_t B7 = 9;
constexpr const uint8_t C7 = 10;
constexpr const uint8_t D7 = 11;
constexpr const uint8_t E7 = 12;
constexpr const uint8_t F7 = 13;
constexpr const uint8_t G7 = 14;
constexpr const uint8_t H7 = 15;

constexpr const uint8_t A6 = 16;
constexpr const uint8_t B6 = 17;
constexpr const uint8_t C6 = 18;
constexpr const uint8_t D6 = 19;
constexpr const uint8_t E6 = 20;
constexpr const uint8_t F6 = 21;
constexpr const uint8_t G6 = 22;
constexpr const uint8_t H6 = 23;

constexpr const uint8_t A5 = 24;
constexpr const uint8_t B5 = 25;
constexpr const uint8_t C5 = 26;
constexpr const uint8_t D5 = 27;
constexpr const uint8_t E5 = 28;
constexpr const uint8_t F5 = 29;
constexpr const uint8_t G5 = 30;
constexpr const uint8_t H5 = 31;

constexpr const uint8_t A4 = 32;
constexpr const uint8_t B4 = 33;
constexpr const uint8_t C4 = 34;
constexpr const uint8_t D4 = 35;
constexpr const uint8_t E4 = 36;
constexpr const uint8_t F4 = 37;
constexpr const uint8_t G4 = 38;
constexpr const uint8_t H4 = 39;

constexpr const uint8_t A3 = 40;
constexpr const uint8_t B3 = 41;
constexpr const uint8_t C3 = 42;
constexpr const uint8_t D3 = 43;
constexpr const uint8_t E3 = 44;
constexpr const uint8_t F3 = 45;
constexpr const uint8_t G3 = 46;
constexpr const uint8_t H3 = 47;

constexpr const uint8_t A2 = 48;
constexpr const uint8_t B2 = 49;
constexpr const uint8_t C2 = 50;
constexpr const uint8_t D2 = 51;
constexpr const uint8_t E2 = 52;
constexpr const uint8_t F2 = 53;
constexpr const uint8_t G2 = 54;
constexpr const uint8_t H2 = 55;

constexpr const uint8_t A1 = 56;
constexpr const uint8_t B1 = 57;
constexpr const uint8_t C1 = 58;
constexpr const uint8_t D1 = 59;
constexpr const uint8_t E1 = 60;
constexpr const uint8_t F1 = 61;
constexpr const uint8_t G1 = 62;
constexpr const uint8_t H1 = 63;

} // namespace chess