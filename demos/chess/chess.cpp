#include "chess.h"
#include "bitboard.h"
#include "fen.h"
#include "zobrist.h"
#include <algorithm>
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
        board[i] = kNullPiece;
    }

    for (auto c = 0; c < kNumColors; c++)
    {
        for (auto p = 0; p < kNumColors; p++)
        {
            std::fill(std::begin(pieces[c]), std::end(pieces[c]), kEmptyBitboard);
        }
    }

    undoStack.clear();
    redoStack.clear();

    loadFromFEN(fen, this);

    hash = ComputeZobristHash(board, turn, castlingRights, 0);
}

bool Chess::IsValidMove(int from, int to) const
{
    auto piece = GetPiece(from);
    auto possible = GenerateMovesForPieceAt(piece, from);
    return (possible & MaskFromSquare(to)) != 0;
}

bool Chess::IsCastlingMove(uint8_t from, uint8_t to, Piece movingPiece) const
{
    if (GetPieceType(movingPiece) != PieceType::King)
    {
        return false;
    }

    auto diff = to - from;
    return (diff == 2 || diff == -2);
}

bool Chess::MoveLeadsToCheck(uint8_t from, uint8_t to) const
{
    const Bitboard fromMask = MaskFromSquare(from);
    const Bitboard toMask = MaskFromSquare(to);

    auto movePiece = [&](Bitboard& bb)
    {
        if (bb & fromMask)
        {
            bb &= ~fromMask;
            bb |= toMask;
            return true;
        }
        return false;
    };

    auto capturePiece = [&](Bitboard& bb)
    {
        if (bb & toMask)
        {
            bb &= ~toMask;
        }
    };

    auto side = GetTurn();
    auto opponent = GetOpponent();

    Bitboard a_pawns = GetPawns(side);
    Bitboard a_knights = GetKnights(side);
    Bitboard a_bishops = GetBishops(side);
    Bitboard a_rooks = GetRooks(side);
    Bitboard a_queens = GetQueens(side);
    Bitboard a_kings = GetKings(side);

    Bitboard b_pawns = GetPawns(opponent);
    Bitboard b_knights = GetKnights(opponent);
    Bitboard b_bishops = GetBishops(opponent);
    Bitboard b_rooks = GetRooks(opponent);
    Bitboard b_queens = GetQueens(opponent);
    Bitboard b_kings = GetKings(opponent);

    capturePiece(b_pawns);
    capturePiece(b_knights);
    capturePiece(b_bishops);
    capturePiece(b_rooks);
    capturePiece(b_queens);
    capturePiece(b_kings);

    // short circuit if we found a move
    movePiece(a_pawns) || movePiece(a_knights) || movePiece(a_bishops) ||
        movePiece(a_rooks) || movePiece(a_queens) || movePiece(a_kings);

    Bitboard occ = a_pawns | a_knights | a_bishops | a_rooks | a_queens | a_kings |
                   b_pawns | b_knights | b_bishops | b_rooks | b_queens | b_kings;

    auto a_king = MoveFromBitboard(a_kings);

    Bitboard attackers = 0ULL;
    attackers |= (opponent == PieceColor::White)
                     ? (WhitePawnCaptureMasks[a_king] & b_pawns)
                     : (BlackPawnCaptureMasks[a_king] & b_pawns);
    attackers |= KnightMasks[a_king] & b_knights;
    attackers |= KingMasks[a_king] & b_kings;
    attackers |= BishopMask(a_king, occ) & (b_bishops | b_queens);
    attackers |= RookMask(a_king, occ) & (b_rooks | b_queens);

    return attackers != 0ULL;
}

bool Chess::MovePiece(uint8_t from, uint8_t to)
{
    if ((from == to) || (from < 0 || from >= kNumSquares || to < 0 || to >= kNumSquares))
    {
        return false;
    }

    auto moving = board[from];
    auto captured = board[to];

    if (GetPieceColor(moving) != GetTurn())
    {
        return false;
    }

    if (!IsValidMove(from, to))
    {
        return false;
    }

    const auto type = GetPieceType(moving);
    const auto color = GetPieceColor(moving);

    auto isEnPassant = (type == PieceType::Pawn && to == enPassantSquare && GetPieceType(captured) == PieceType::None);
    if (isEnPassant)
    {
        auto sq = (color == PieceColor::White) ? to + kNumRanks : to - kNumRanks;
        captured = board[sq];
        RemovePiece(sq);
    }

    // Move the king
    RemovePiece(to);
    RemovePiece(from);
    PutPiece(moving, to);

    if (IsCastlingMove(from, to, moving))
    {
        if (turn == PieceColor::White)
        {
            auto piece = MakePiece(PieceColor::White, PieceType::Rook);
            if (to % 8 == 6) // king-side castle (king moves to G1 = 6)
            {
                RemovePiece(H1);
                PutPiece(piece, F1);
            }
            else if (to % 8 == 2) // queen-side castle (king moves to C1 = 2)
            {
                RemovePiece(A1);
                PutPiece(piece, D1);
            }
        }
        else if (turn == PieceColor::Black)
        {
            auto piece = MakePiece(PieceColor::Black, PieceType::Rook);
            if (to % 8 == 6) // king-side castle (king moves to G8 = 62 % 8 = 6)
            {
                RemovePiece(H8);
                PutPiece(piece, F8);
            }
            else if (to % 8 == 2) // queen-side castle (king moves to C8 = 58 % 8 = 2)
            {
                RemovePiece(A8);
                PutPiece(piece, D8);
            }
        }
    }

    // castling rights
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

    // enpassent
    auto prevEnPassant = enPassantSquare;
    enPassantSquare = kNullSquare;
    if (type == PieceType::Pawn)
    {
        if (abs((int)to - (int)from) == 16)
        {
            enPassantSquare = (from + to) / 2;
        }
    }

    turn = GetOpponent();

    // cache move
    redoStack.clear();
    undoStack.push_back(chess::Undo{
        .move = {moving, from, to},
        .captured = captured,
        .oldEnPassant = prevEnPassant,
        .newEnPassant = enPassantSquare,
        .oldCastlingRights = prevCastlingRights,
        .newCastlingRights = castlingRights,
    });

    UpdateZobristMove(moving, from, to, captured, 0x00, castlingRights, prevCastlingRights);

    return true;
}

Piece Chess::GetPiece(uint8_t square) const
{
    return board[square];
}

void Chess::PutPiece(Piece piece, uint8_t square)
{
    if (GetPieceType(piece) == PieceType::None)
    {
        return;
    }

    board[square] = piece;

    auto type = static_cast<uint8_t>(GetPieceType(piece));
    auto color = static_cast<uint8_t>(GetPieceColor(piece));
    pieces[color][type] |= MaskFromSquare(square);
}

void Chess::RemovePiece(uint8_t square)
{
    auto piece = board[square];
    if (GetPieceType(piece) == PieceType::None)
    {
        return;
    }

    board[square] = kNullPiece;

    auto type = static_cast<uint8_t>(GetPieceType(piece));
    auto color = static_cast<uint8_t>(GetPieceColor(piece));
    pieces[color][type] &= ~MaskFromSquare(square);
}

void Chess::Undo(void)
{
    if (undoStack.empty())
    {
        return;
    }

    auto prev = undoStack.back();
    undoStack.pop_back();

    RemovePiece(prev.move.to);

    auto isEnPassant = (GetPieceType(prev.move.piece) == PieceType::Pawn &&
                        prev.move.to == prev.newEnPassant &&
                        GetPieceType(prev.captured) != PieceType::None);

    if (isEnPassant)
    {
        auto sq = (GetPieceColor(prev.move.piece) == PieceColor::White) ? prev.move.to + kNumFiles : prev.move.to - kNumFiles;
        PutPiece(prev.captured, sq);
    }
    else if (GetPieceType(prev.captured) != PieceType::None)
    {
        PutPiece(prev.captured, prev.move.to);
    }

    PutPiece(prev.move.piece, prev.move.from);

    // game state
    enPassantSquare = prev.oldEnPassant;
    castlingRights = prev.oldCastlingRights;
    turn = GetOpponent();

    UpdateZobristMove(prev.move.piece, prev.move.from, prev.move.to, prev.captured, prev.newEnPassant, prev.newCastlingRights, prev.oldCastlingRights);

    redoStack.push_back(prev);
}

void Chess::Redo(void)
{
    if (redoStack.empty())
    {
        return;
    }

    auto next = redoStack.back();
    redoStack.pop_back();

    auto isEnPassant = (GetPieceType(next.move.piece) == PieceType::Pawn &&
                        next.move.to == next.newEnPassant &&
                        GetPieceType(next.captured) != PieceType::None);

    // move pieces
    RemovePiece(next.move.from);
    if (isEnPassant)
    {
        int sq = (GetPieceColor(next.move.piece) == PieceColor::White) ? next.move.to - kNumFiles : next.move.to + kNumFiles;
        RemovePiece(sq);
    }
    else
    {
        RemovePiece(next.move.to);
    }
    PutPiece(next.move.piece, next.move.to);

    // game state
    enPassantSquare = next.newEnPassant;
    castlingRights = next.newCastlingRights;
    turn = GetOpponent();

    // update the hash
    UpdateZobristMove(next.move.piece, next.move.from, next.move.to, next.captured, 0x00, next.oldCastlingRights, next.newCastlingRights);

    undoStack.push_back(next);
}

const std::vector<Piece> Chess::GetBoard() const
{
    return std::vector<Piece>(board, board + 64);
}

const std::string Chess::GetZobrist() const
{
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

const Bitboard Chess::GetOccupied(const PieceColor color) const
{
    const auto pawns = GetPawns(color);
    const auto knights = GetKnights(color);
    const auto bishops = GetBishops(color);
    const auto queens = GetQueens(color);
    const auto kings = GetKings(color);
    return pawns | knights | bishops | queens | kings;
}

const Bitboard Chess::GetAttacks(PieceColor from) const
{
    Bitboard attacks = kEmptyBitboard;
    const Bitboard occupancy = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

    for (auto i = 0; i < kNumSquares; ++i)
    {
        auto piece = GetPiece(i);
        if ((GetPieceType(piece) == PieceType::None) || (GetPieceColor(piece) != from))
        {
            continue;
        }

        switch (GetPieceType(piece))
        {
        case PieceType::Pawn:
            attacks |= (from == PieceColor::White)
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

const Bitboard Chess::GetAttacksOnSquare(uint8_t square, PieceColor from) const
{
    Bitboard attackers = kEmptyBitboard;

    const Bitboard occupancy = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

    attackers |= (from == PieceColor::White)
                     ? (WhitePawnCaptureMasks[square] & GetPawns(from))
                     : (BlackPawnCaptureMasks[square] & GetPawns(from));
    attackers |= KnightMasks[square] & GetKnights(from);
    attackers |= KingMasks[square] & GetKings(from);
    attackers |= BishopMask(square, occupancy) & (GetBishops(from) | GetQueens(from));
    attackers |= RookMask(square, occupancy) & (GetRooks(from) | GetQueens(from));

    return attackers;
}

bool Chess::InCheck(PieceColor turn) const
{
    const auto king = MoveFromBitboard(GetKings(turn));
    const auto attacking = CountPieces(GetAttacksOnSquare(king, GetOpposite(turn)));
    return attacking >= 1;
}

bool Chess::InCheckmate(void) const
{
    if (!InCheck(GetTurn()))
    {
        return false;
    }

    const auto turn = GetTurn();
    const auto kingSquare = MoveFromBitboard(GetKings(turn));
    const Bitboard kingMask = KingMasks[kingSquare] & ~GetOccupied(turn);
    const Bitboard safeMoves = kingMask & ~GetAttacks(GetOpponent());

    // const Bitboard attackers = GetAttacking(king_square);

    // const auto is_double_check = CountPieces(attackers)
    // auto num_of_attackers =

    // FIXME: need to know if we can capture the piece checking us

    return safeMoves == kEmptyBitboard;
}

const std::vector<chess::Move> Chess::Moves(void) const
{
    std::vector<chess::Move> moves = {};
    for (auto from = 0; from < kNumSquares; from++)
    {
        auto piece = GetPiece(from);
        auto color = GetPieceColor(piece);
        if (GetPieceType(piece) == PieceType::None || color != GetTurn())
        {
            continue;
        }

        auto possibleMoves = GenerateMovesForPieceAt(piece, from);
        while (possibleMoves)
        {
            auto to = MoveFromBitboard(possibleMoves);
            possibleMoves &= possibleMoves - 1;

            moves.push_back((chess::Move){
                .from = (uint8_t)from,
                .to = to,
            });
        }
    }
    return moves;
}

const std::vector<chess::Move> Chess::MovesFromSquare(uint8_t from) const
{
    std::vector<chess::Move> moves = {};
    auto piece = GetPiece(from);
    auto color = GetPieceColor(piece);
    if (GetPieceType(piece) == PieceType::None || color != GetTurn())
    {
        return moves;
    }

    auto possibleMoves = GenerateMovesForPieceAt(piece, from);
    while (possibleMoves)
    {
        auto to = MoveFromBitboard(possibleMoves);
        possibleMoves &= possibleMoves - 1;

        moves.push_back((chess::Move){
            .from = (uint8_t)from,
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

    for (auto from = 0; from < kNumSquares; from++)
    {
        if (piece != GetPiece(from))
        {
            continue;
        }
        auto color = GetPieceColor(piece);
        if (GetPieceType(piece) == PieceType::None || color != GetTurn())
        {
            continue;
        }

        auto possibleMoves = GenerateMovesForPieceAt(piece, from);
        while (possibleMoves)
        {
            auto to = MoveFromBitboard(possibleMoves);
            possibleMoves &= possibleMoves - 1;

            moves.push_back((chess::Move){
                .from = (uint8_t)from,
                .to = to,
            });
        }
    }
    return moves;
}

const Bitboard Chess::GeneratePawnMoves(uint8_t square) const
{
    const Bitboard empty = ~GetOccupied(PieceColor::White) & ~GetOccupied(PieceColor::Black);
    const Bitboard enemy = GetOccupied(GetOpponent());

    Bitboard push = kEmptyBitboard;
    Bitboard double_push = kEmptyBitboard;
    Bitboard captures = kEmptyBitboard;

    switch (GetTurn())
    {
    case PieceColor::White:
        push = WhitePawnPushMasks[square];
        double_push = WhitePawnDoublePushMasks[square];
        captures = WhitePawnCaptureMasks[square] & enemy;

        if (enPassantSquare != kNullSquare &&
            (WhitePawnCaptureMasks[square] & MaskFromSquare(enPassantSquare)))
        {
            captures |= MaskFromSquare(enPassantSquare);
        }
        break;
    case PieceColor::Black:
        push = BlackPawnPushMasks[square];
        double_push = BlackPawnDoublePushMasks[square];
        captures = BlackPawnCaptureMasks[square] & enemy;

        if (enPassantSquare != kNullSquare &&
            (BlackPawnCaptureMasks[square] & MaskFromSquare(enPassantSquare)))
        {
            captures |= MaskFromSquare(enPassantSquare);
        }
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

    return (push_valid | double_push | captures);
}

const Bitboard Chess::GenerateKnightMoves(uint8_t square) const
{
    return KnightMasks[square];
}

const Bitboard Chess::GenerateBishopMoves(uint8_t square, const Bitboard blockers) const
{
    return BishopMask(square, blockers);
}

const Bitboard Chess::GenerateRookMoves(uint8_t square, const Bitboard blockers) const
{
    return RookMask(square, blockers);
}

const Bitboard Chess::GenerateQueenMoves(uint8_t square, const Bitboard blockers) const
{
    return QueenMask(square, blockers);
}

const Bitboard Chess::GenerateKingMoves(uint8_t square) const
{
    Bitboard possibleMoves = KingMasks[square];
    possibleMoves &= ~GetAttacks(GetOpponent());

    const Bitboard occupied = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

    auto isSafe = [&](uint8_t sq) -> bool
    {
        return (GetAttacksOnSquare(sq, GetOpponent()) == 0);
    };

    switch (GetTurn())
    {
    case PieceColor::White:
        if (Has(castlingRights, CastlingRights::WhiteKingSide) &&
            !(occupied & (MaskFromSquare(F1) | MaskFromSquare(G1))) && // no pieces in between
            isSafe(E1) && isSafe(F1) && isSafe(G1))                    // can't pass through check
        {
            possibleMoves |= MaskFromSquare(G1);
        }
        if (Has(castlingRights, CastlingRights::WhiteQueenSide) &&
            !(occupied & (MaskFromSquare(D1) | MaskFromSquare(C1) | MaskFromSquare(B1))) && // no pieces in between
            isSafe(E1) && isSafe(D1) && isSafe(C1))
        {
            possibleMoves |= MaskFromSquare(C1);
        }
        break;

    case PieceColor::Black:
        if (Has(castlingRights, CastlingRights::BlackKingSide) &&
            !(occupied & (MaskFromSquare(F8) | MaskFromSquare(G8))) &&
            isSafe(E8) && isSafe(F8) && isSafe(G8))
        {
            possibleMoves |= MaskFromSquare(G8);
        }
        if (Has(castlingRights, CastlingRights::BlackQueenSide) &&
            !(occupied & (MaskFromSquare(D8) | MaskFromSquare(C8) | MaskFromSquare(B8))) &&
            isSafe(E8) && isSafe(D8) && isSafe(C8))
        {
            possibleMoves |= MaskFromSquare(C8);
        }
        break;
    }

    return possibleMoves;
}

const Bitboard Chess::GenerateMovesForPieceAt(const Piece piece, uint8_t square) const
{
    Bitboard possibleMoves = kEmptyBitboard;

    const auto type = GetPieceType(piece);
    const auto color = GetPieceColor(piece);

    const auto king = MoveFromBitboard(GetKings(GetTurn()));
    const auto num_attackers = CountPieces(GetAttacksOnSquare(king, GetOpponent()));

    if (num_attackers > 0 && type == PieceType::King)
    {
        if (num_attackers <= 2)
        {
            possibleMoves = GenerateKingMoves(square);
        }
    }
    else
    {
        const Bitboard blockers = GetOccupied(PieceColor::White) | GetOccupied(PieceColor::Black);

        switch (type)
        {
        case PieceType::None:
            break;
        case PieceType::Pawn:
            possibleMoves = GeneratePawnMoves(square);
            break;
        case PieceType::Knight:
            possibleMoves = GenerateKnightMoves(square);
            break;
        case PieceType::Bishop:
            possibleMoves = GenerateBishopMoves(square, blockers);
            break;
        case PieceType::Rook:
            possibleMoves = GenerateRookMoves(square, blockers);
            break;
        case PieceType::Queen:
            possibleMoves = GenerateQueenMoves(square, blockers);
            break;
        case PieceType::King:
            possibleMoves = GenerateKingMoves(square);
            break;
        }
    }

    // Exclude board occupied by friendly pieces
    possibleMoves &= ~GetOccupied(color);

    // filter out moves that lead to check
    Bitboard legal_moves = kEmptyBitboard;
    for (auto to = 0; to < kNumSquares; ++to)
    {
        if (possibleMoves & MaskFromSquare(to) && !MoveLeadsToCheck(square, to))
        {
            legal_moves |= MaskFromSquare(to);
        }
    }

    return legal_moves;
}

void Chess::UpdateZobristMove(Piece moving,
                              uint8_t from,
                              uint8_t to,
                              Piece captured,
                              uint8_t enPassant,
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
