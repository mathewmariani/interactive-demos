#include <emscripten/bind.h>

#include "chess.h"

namespace chess
{

emscripten::val w_ChessMove(Chess& self, emscripten::val opts)
{
    std::vector<Move> moves;

    if (opts.isUndefined() || opts.isNull())
    {
        moves = self.Moves();
    }
    else if (opts.hasOwnProperty("square"))
    {
        auto square = opts["square"].as<uint8_t>();
        moves = self.MovesFromSquare(square);
    }
    else if (opts.hasOwnProperty("piece"))
    {
        auto piece = opts["piece"].as<uint8_t>();
        moves = self.MovesForPiece(piece);
    }
    else
    {
        throw std::invalid_argument("Invalid moves() argument");
    }

    return emscripten::val::array(moves);
}
EMSCRIPTEN_BINDINGS(chess_module)
{
    emscripten::enum_<PieceType>("PieceType")
        .value("None", PieceType::None)
        .value("King", PieceType::King)
        .value("Pawn", PieceType::Pawn)
        .value("Knight", PieceType::Knight)
        .value("Bishop", PieceType::Bishop)
        .value("Rook", PieceType::Rook)
        .value("Queen", PieceType::Queen);

    emscripten::enum_<PieceColor>("PieceColor")
        .value("White", PieceColor::White)
        .value("Black", PieceColor::Black);

    emscripten::register_vector<Move>("MoveList");

    emscripten::value_object<Move>("Move")
        .field("from", &Move::from)
        .field("to", &Move::to);

    emscripten::class_<Chess>("Chess")
        .constructor<>()
        .function("get_board", &Chess::GetBoard)
        .function("get_rooks", &Chess::GetRooks)
        .function("get_bishops", &Chess::GetBishops)
        .function("get_queens", &Chess::GetQueens)
        .function("get_knights", &Chess::GetKnights)
        .function("get_pawns", &Chess::GetPawns)
        .function("get_kings", &Chess::GetKings)

        .function("zobrist", &Chess::GetZobrist)
        .function("undo", &Chess::Undo)
        .function("redo", &Chess::Redo)

        .function("turn", &Chess::GetTurn)
        .function("setTurn", &Chess::SetTurn)

        .function("moves", w_ChessMove)
        .function("get_possible_moves", &Chess::GetPossibleMoves)

        .function("board", &Chess::GetBoard)
        .function("clear", &Chess::Clear)
        .function("load", &Chess::Load)
        .function("move", &Chess::Move)
        .function("put", &Chess::Put)
        .function("remove", &Chess::Remove)
        .function("reset", &Chess::Reset);

    emscripten::register_vector<uint8_t>("VectorUint8");
    emscripten::register_vector<int>("VectorInt");
}

} // namespace chess
