#pragma once

namespace chess
{

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

constexpr int kNumRanks = 8; // x
constexpr int kNumFiles = 8; // y
constexpr int kNumSquares = kNumRanks * kNumFiles;
constexpr int kNumPieces = 12;

// bitboard
constexpr uint64_t kEmptyBitboard = 0ULL;

// zobrist
constexpr int kCastlingBits = 16;
constexpr int kEnPassentBits = 8;

} // namespace chess