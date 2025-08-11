#include "chess.h"
#include "fen.h"
#include "zobrist.h"
#include <algorithm>
#include <bit>
#include <sstream>
#include <string>
#include <vector>

namespace chess
{

Chess::Chess()
{
    Reset();
}

const char* Chess::GetSAN(const uint8_t square) const
{
    return kSANPositions[square];
}

void Chess::Reset()
{
    Load(kDefaultPosition);
}

void Chess::Clear()
{
    Load(KEmptyPostion);
}

void Chess::Load(const std::string& fen)
{
    for (auto i = 0; i < kNumSquares; i++)
    {
        squares[i] = kNullPiece;
    }

    std::fill(std::begin(rooks), std::end(rooks), kEmptyBitboard);
    std::fill(std::begin(bishops), std::end(bishops), kEmptyBitboard);
    std::fill(std::begin(queens), std::end(queens), kEmptyBitboard);
    std::fill(std::begin(knights), std::end(knights), kEmptyBitboard);
    std::fill(std::begin(pawns), std::end(pawns), kEmptyBitboard);
    std::fill(std::begin(kings), std::end(kings), kEmptyBitboard);

    undo_stack.clear();
    redo_stack.clear();

    loadFromFEN(fen, this);

    hash = ComputeZobristHash(squares, turn, castlingRights, 0);
}

bool Chess::IsValidMove(int from, int to) const
{
    auto piece = GetPiece(from);
    auto possible = GetPossibleMoves(piece, from);
    return (possible & (1ULL << to)) != 0;
}

bool Chess::IsCastlingMove(uint8_t from, uint8_t to, Piece movingPiece)
{
    if (GetPieceType(movingPiece) != PieceType::King)
    {
        return false;
    }

    auto diff = to - from;
    return (diff == 2 || diff == -2);
}

bool Chess::MovePiece(uint8_t from, uint8_t to)
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

    // Move the king
    RemovePiece(to);
    RemovePiece(from);
    PutPiece(moving, to);

    if (IsCastlingMove(from, to, moving))
    {
        if (turn == PieceColor::White)
        {
            if (to % 8 == 6) // king-side castle (king moves to G1 = 6)
            {
                RemovePiece(H1);
                auto piece = MakePiece(PieceColor::White, PieceType::Rook);
                PutPiece(piece, F1);
            }
            else if (to % 8 == 2) // queen-side castle (king moves to C1 = 2)
            {
                RemovePiece(A1);
                auto piece = MakePiece(PieceColor::White, PieceType::Rook);
                PutPiece(piece, D1);
            }
        }
        else if (turn == PieceColor::Black)
        {
            if (to % 8 == 6) // king-side castle (king moves to G8 = 62 % 8 = 6)
            {
                RemovePiece(H8);
                auto piece = MakePiece(PieceColor::Black, PieceType::Rook);
                PutPiece(piece, F8);
            }
            else if (to % 8 == 2) // queen-side castle (king moves to C8 = 58 % 8 = 2)
            {
                RemovePiece(A8);
                auto piece = MakePiece(PieceColor::Black, PieceType::Rook);
                PutPiece(piece, D8);
            }
        }
    }

    // castling rights
    const auto type = GetPieceType(moving);
    const auto color = GetPieceColor(moving);
    const auto prevCastlingRights = castlingRights;
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

    // cache move
    undo_stack.push_back(chess::Undo{{moving, from, to}, captured, prevCastlingRights, castlingRights});
    redo_stack.clear();

    UpdateZobristMove(moving, from, to, captured, castlingRights, prevCastlingRights);

    return true;
}

Piece Chess::GetPiece(uint8_t square) const
{
    return squares[square];
}

void Chess::PutPiece(Piece piece, uint8_t square)
{
    if (GetPieceType(piece) == PieceType::None)
    {
        return;
    }

    squares[square] = piece;

    auto type = GetPieceType(piece);
    auto color = to_index(GetPieceColor(piece));
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

void Chess::RemovePiece(uint8_t square)
{
    auto piece = squares[square];
    if (GetPieceType(piece) == PieceType::None)
    {
        return;
    }

    squares[square] = kNullPiece;

    auto type = GetPieceType(piece);
    auto color = to_index(GetPieceColor(piece));
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

void Chess::Undo(void)
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
    PutPiece(prev.move.piece, prev.move.from);

    if (GetPieceType(prev.captured) != PieceType::None)
    {
        PutPiece(prev.captured, prev.move.to);
    }

    // game state
    castlingRights = prev.oldCastlingRights;
    turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;

    UpdateZobristMove(prev.move.piece, prev.move.from, prev.move.to, prev.captured, prev.newCastlingRights, prev.oldCastlingRights);

    redo_stack.push_back(prev);
}

void Chess::Redo(void)
{
    if (redo_stack.empty())
    {
        return;
    }

    auto next = redo_stack.back();
    redo_stack.pop_back();

    // move pieces
    RemovePiece(next.move.from);
    RemovePiece(next.move.to);
    PutPiece(next.move.piece, next.move.to);

    // game state
    castlingRights = next.newCastlingRights;
    turn = (turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;

    // update the hash
    UpdateZobristMove(next.move.piece, next.move.from, next.move.to, next.captured, next.oldCastlingRights, next.newCastlingRights);

    undo_stack.push_back(next);
}

const std::vector<Piece> Chess::GetBoard() const
{
    return std::vector<Piece>(squares, squares + 64);
}

const std::string Chess::GetZobrist() const
{
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

const Bitboard Chess::GetRooks(void) const { return rooks[kWhiteIndex] | rooks[kBlackIndex]; }
const Bitboard Chess::GetBishops(void) const { return bishops[kWhiteIndex] | bishops[kBlackIndex]; }
const Bitboard Chess::GetQueens(void) const { return queens[kWhiteIndex] | queens[kBlackIndex]; }
const Bitboard Chess::GetKnights(void) const { return knights[kWhiteIndex] | knights[kBlackIndex]; }
const Bitboard Chess::GetPawns(void) const { return pawns[kWhiteIndex] | pawns[kBlackIndex]; }
const Bitboard Chess::GetKings(void) const { return kings[kWhiteIndex] | kings[kBlackIndex]; }

const Bitboard Chess::GetOccupied(const PieceColor color) const
{
    auto idx = to_index(color);
    return rooks[idx] | bishops[idx] | queens[idx] |
           knights[idx] | pawns[idx] | kings[idx];
}

const Bitboard Chess::GetAttacking(void) const
{
    Bitboard attacks = kEmptyBitboard;

    auto occupancy = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

    for (auto i = 0; i < kNumSquares; ++i)
    {
        auto piece = GetPiece(i);
        if (GetPieceType(piece) == PieceType::None)
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

const Bitboard Chess::GetPossibleMoves(const Piece piece, uint8_t square) const
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

bool Chess::InCheck(void) const
{
    auto color = to_index(GetTurn());
    auto attacking = GetAttacking();
    auto king = kings[color];
    return (king & attacking) != 0;
}

bool Chess::InCheckmate(void) const
{
    if (!InCheck())
    {
        return false;
    }

    auto color = to_index(GetTurn());
    auto square = MoveFromBitboard(kings[color]);
    auto king_moves = KingMasks[square] & ~GetOccupied(GetTurn());
    auto safe_moves = king_moves & ~GetAttacking();

    // FIXME: need to know if we can capture the piece checking us

    return safe_moves == kEmptyBitboard;
}

const std::vector<chess::Move> Chess::Moves(void) const
{
    std::vector<chess::Move> moves = {};
    for (auto sq = 0; sq < kNumSquares; sq++)
    {
        auto piece = GetPiece(sq);
        auto color = GetPieceColor(piece);
        if (GetPieceType(piece) == PieceType::None || color != GetTurn())
        {
            continue;
        }

        auto possible = GetPossibleMoves(piece, sq);
        while (possible)
        {
            auto to = MoveFromBitboard(possible);
            possible &= possible - 1;

            moves.push_back((chess::Move){
                .from = (uint8_t)sq,
                .to = to,
            });
        }
    }
    return moves;
}

const std::vector<chess::Move> Chess::MovesFromSquare(uint8_t square) const
{
    std::vector<chess::Move> moves = {};
    auto piece = GetPiece(square);
    auto color = GetPieceColor(piece);
    if (GetPieceType(piece) == PieceType::None || color != GetTurn())
    {
        return moves;
    }

    auto possible = GetPossibleMoves(piece, square);
    while (possible)
    {
        auto to = MoveFromBitboard(possible);
        possible &= possible - 1;

        moves.push_back((chess::Move){
            .from = square,
            .to = to,
        });
    }

    return moves;
}

const std::vector<chess::Move> Chess::MovesForPiece(Piece piece) const
{
    std::vector<chess::Move> moves = {};
    const auto type = GetPieceColor(piece);
    const auto color = GetPieceColor(piece);

    for (auto sq = 0; sq < kNumSquares; sq++)
    {
        if (piece != GetPiece(sq))
        {
            continue;
        }
        auto color = GetPieceColor(piece);
        if (GetPieceType(piece) == PieceType::None || color != GetTurn())
        {
            continue;
        }

        auto possible = GetPossibleMoves(piece, sq);
        while (possible)
        {
            auto to = MoveFromBitboard(possible);
            possible &= possible - 1;

            moves.push_back((chess::Move){
                .from = (uint8_t)sq,
                .to = to,
            });
        }
    }
    return moves;
}

void Chess::UpdateZobristMove(Piece moving,
                              uint8_t from,
                              uint8_t to,
                              Piece captured,
                              CastlingRights prevCastlingRights,
                              CastlingRights newCastlingRights)
{
    hash ^= zobrist.side;              // flip side
    hash ^= zobrist.psq[moving][from]; // remove piece from old square
    hash ^= zobrist.psq[moving][to];   // add piece to new square
    if (GetPieceType(captured) != PieceType::None)
    {
        hash ^= zobrist.psq[captured][to]; // remove captured piece
    }
    hash ^= zobrist.castling[static_cast<uint8_t>(prevCastlingRights)]; // remove old castling rights
    hash ^= zobrist.castling[static_cast<uint8_t>(newCastlingRights)];  // add new castling rights
}

} // namespace chess
