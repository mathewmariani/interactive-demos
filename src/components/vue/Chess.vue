<template>
    <figure id="diagram1">
        <div class="btn-toolbar mb-3" role="toolbar" aria-label="Toolbar with button groups">
            <div class="btn-group me-2" role="group" aria-label="Reset and Clear">
                <button type="button" class="btn btn-primary" @click="onReset">Reset</button>
                <button type="button" class="btn btn-primary" @click="onClear">Clear</button>
            </div>
            <div class="btn-group me-2" role="group" aria-label="Reset and Clear">
                <button type="button" class="btn btn-primary" @click="onUndo">Undo</button>
                <button type="button" class="btn btn-primary" @click="onRedo">Redo</button>
            </div>
        </div>
        <div class="d-flex flex-column align-items-center">
            <svg ref="svgRef" viewBox="0 0 8 8" width="512" height="512" @pointermove="onPointerMove"
                @pointerup="onPointerUp" @pointercancel="onPointerUp">
                <!-- squares -->
                <g v-for="square in squares">
                    <rect :key="square.name" :x="square.x" :y="square.y" width="1" height="1" :class="square.color" />
                </g>

                <!-- highlights -->
                <template v-for="square in squares" :key="square.name + '-highlight'">
                    <!-- Only highlight if toggle is on for piece type -->
                    <rect v-if="square.isRook" v-show="showRooks" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(84, 160, 255, 0.85)" />
                    <rect v-if="square.isBishop" v-show="showBishops" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(255, 215, 84, 0.85)" />
                    <rect v-if="square.isQueen" v-show="showQueens" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(186, 85, 211, 0.85)" />
                    <rect v-if="square.isKnight" v-show="showKnights" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(50, 205, 50, 0.85)" />
                    <rect v-if="square.isPawn" v-show="showPawns" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(255, 107, 107, 0.85)" />
                    <rect v-if="square.isKing" v-show="showKings" :x="square.x" :y="square.y" width="1" height="1"
                        fill="rgba(255, 140, 0, 0.85)" />
                    <rect v-if="square.isAttacking" v-show="showAttacking" :x="square.x" :y="square.y" width="1"
                        height="1" fill="rgba(255, 107, 107, 0.85)" />
                </template>

                <!-- square name -->
                <text v-for="square in squares" :key="square.name + '-debug'" :x="square.x + 0.05" :y="square.y + 0.95"
                    font-size="0.2" fill="red">
                    {{ square.san || "" }}
                </text>

                <!-- pieces -->
                <template v-for="square in squares" :key="square.name + '-piece'">
                    <g v-if="square && square.piece && square.name !== draggingSquare" :key="square.name + '-piece'"
                        :transform="getPieceTransform(square)">
                        <component :is="pieceMap[square.piece]" />
                    </g>

                    <!-- interaction -->
                    <rect v-if="square && square.piece" :key="square.name + '-hover'" :x="square.x" :y="square.y"
                        width="1" height="1" fill="transparent" :style="{ cursor: getPieceCursor(square) }"
                        @pointerdown="onPointerDown(square, $event)" />
                </template>

                <template v-for="square in squares" :key="square.name + '-piece'">
                    <circle v-if="possibleMoves & (1n << BigInt(square.idx))" :cx="square.x + 0.5" :cy="square.y + 0.5"
                        r="0.20" fill="rgba(200, 214, 229, 0.85)" />
                </template>

                <!-- dragged -->
                <template v-for="square in squares">
                    <g v-if="square && square.piece && square.name === draggingSquare" :key="square.name + '-dragging'"
                        :transform="getPieceTransform(square)">
                        <component :is="pieceMap[square.piece]" />
                    </g>
                </template>
            </svg>
        </div>

        <!-- New toggle form -->
        <div class="mt-3">
            <h2>Bitboards</h2>
            <p>{{ getHash() }}</p>
            <p>Castling Rights: {{ getCastlingRights() }}</p>
            <div class="form-check">
                <input type="checkbox" id="showRooks" class="form-check-input" v-model="showRooks" />
                <label for="showRooks" class="form-check-label">Show Rooks</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showBishops" class="form-check-input" v-model="showBishops" />
                <label for="showBishops" class="form-check-label">Show Bishops</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showQueens" class="form-check-input" v-model="showQueens" />
                <label for="showQueens" class="form-check-label">Show Queens</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showKnights" class="form-check-input" v-model="showKnights" />
                <label for="showKnights" class="form-check-label">Show Knights</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showPawns" class="form-check-input" v-model="showPawns" />
                <label for="showPawns" class="form-check-label">Show Pawns</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showKings" class="form-check-input" v-model="showKings" />
                <label for="showKings" class="form-check-label">Show Kings</label>
            </div>
            <div class="form-check">
                <input type="checkbox" id="showAttacking" class="form-check-input" v-model="showAttacking" />
                <label for="showAttacking" class="form-check-label">Show Attacking</label>
            </div>
        </div>

    </figure>
</template>

<script>
import { shallowReactive, markRaw, reactive } from "vue";
import Module from "../../modules/demos.js";

import WhitePawn from "./WhitePawn.vue";
import WhiteKnight from "./WhiteKnight.vue";
import WhiteBishop from "./WhiteBishop.vue";
import WhiteRook from "./WhiteRook.vue";
import WhiteQueen from "./WhiteQueen.vue";
import WhiteKing from "./WhiteKing.vue";

import BlackPawn from "./BlackPawn.vue";
import BlackKnight from "./BlackKnight.vue";
import BlackBishop from "./BlackBishop.vue";
import BlackRook from "./BlackRook.vue";
import BlackQueen from "./BlackQueen.vue";
import BlackKing from "./BlackKing.vue";

const pieceMap = {
    "0_1": markRaw(WhitePawn),
    "0_2": markRaw(WhiteKnight),
    "0_3": markRaw(WhiteBishop),
    "0_4": markRaw(WhiteRook),
    "0_5": markRaw(WhiteQueen),
    "0_6": markRaw(WhiteKing),

    "8_1": markRaw(BlackPawn),
    "8_2": markRaw(BlackKnight),
    "8_3": markRaw(BlackBishop),
    "8_4": markRaw(BlackRook),
    "8_5": markRaw(BlackQueen),
    "8_6": markRaw(BlackKing),
};

const SAN = [
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
];

const FILES = ["a", "b", "c", "d", "e", "f", "g", "h"];
const boardSize = 8;

function squareNameToIndex(name) {
    const fileChar = name[0]; // 'e'
    const rankChar = parseInt(name[1]); // 2

    const file = FILES.indexOf(fileChar); // 4 for 'e'
    const rank = 8 - rankChar; // ranks count down from 8 at top

    if (file === -1 || rank < 0 || rank > 7) {
        throw new Error(`Invalid square name: ${name}`);
    }

    return rank * 8 + file;
}

const kEmptyBitboard = 0n;
const kPieceColorMask = 0b00001000; // 8
const kPieceTypeMask = 0b00000111;  // 7

const PieceColor = {
    WHITE: 0,
    BLACK: 1,
};

const PieceType = {
    PAWN: 1,
    KNIGHT: 2,
    BISHOP: 3,
    ROOK: 4,
    QUEEN: 5,
    KING: 6,
};

function getPieceColor(piece) {
    return (piece & kPieceColorMask) >> 3;
}

function getPieceType(piece) {
    return piece & kPieceTypeMask;
}

export default {
    data() {
        return {
            engine: null,
            draggingSquare: null,
            dragStartPointer: null,
            dragStartPos: null,
            dragPositions: reactive({}),
            pieceMap,
            boardVersion: 0, // reactive counter
            showRooks: true,
            showBishops: true,
            showQueens: true,
            showKnights: true,
            showPawns: true,
            showKings: true,
            showAttacking: true,
            possibleMoves: kEmptyBitboard,
        };
    },
    async beforeCreate() {
        const wasm = await Module();
        this.engine = shallowReactive(new wasm.Chess());
    },
    computed: {
        squares() {
            this.boardVersion;
            if (!this.engine) return [];
            const vec = this.engine.get_board();
            const rookBits = this.engine.get_rooks();
            const bishopBits = this.engine.get_bishops();
            const queenBits = this.engine.get_queens();
            const knightBits = this.engine.get_knights();
            const pawnBits = this.engine.get_pawns();
            const kingBits = this.engine.get_kings();
            const attackingBits = this.engine.attacking();
            const squares = [];
            for (let rank = 8; rank >= 1; rank--) {
                for (let file = 0; file < 8; file++) {
                    const idx = (8 - rank) * 8 + file;
                    const val = vec.get(idx);
                    const piece = val & 0b111;
                    const color = val & 0b11000;
                    const pieceKey = piece === 0 ? null : `${color}_${piece}`;

                    squares.push({
                        name: `${FILES[file]}${rank}`,
                        idx: idx,
                        san: SAN[idx],
                        x: file,
                        y: 8 - rank,
                        color: (file + rank) % 2 === 0 ? "light" : "dark",
                        piece: pieceKey,

                        // only highlight if corresponding toggle is true
                        isRook: this.showRooks && (rookBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isBishop: this.showBishops && (bishopBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isQueen: this.showQueens && (queenBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isKnight: this.showKnights && (knightBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isPawn: this.showPawns && (pawnBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isKing: this.showKings && (kingBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                        isAttacking: this.showAttacking && (attackingBits & (1n << BigInt(idx))) !== kEmptyBitboard,
                    });
                }
            }
            return squares;
        },
    },
    methods: {
        getPieceCursor(square) {
            if (!square || !square.piece) return 'default';
            return this.draggingSquare === square.name ? 'grabbing' : 'grab';
        },
        onUndo() {
            this.engine.undo();
            this.boardVersion++;
            this.$forceUpdate();
        },
        onRedo() {
            this.engine.redo();
            this.boardVersion++;
            this.$forceUpdate();
        },
        onReset() {
            this.engine.reset();
            this.boardVersion++;
            this.$forceUpdate();

            const moves = this.engine.moves();
            moves.forEach(m => console.log(m));
        },
        getHash() {
            return (this.engine) ? this.engine.zobrist() : '';
        },
        getCastlingRights() {
            return (this.engine) ? this.engine.castlingRights() : 'Never';
        },
        onClear() {
            this.engine.clear();
            this.boardVersion++;
            this.$forceUpdate();
        },
        selectPiece(square) {
            this.selectedSquare = square;
            const idx = squareNameToIndex(square.name);
            const val = this.engine.get_board().get(idx);

            if (this.engine.turn().value != getPieceColor(val)) {
                this.possibleMoves = kEmptyBitboard;
                return;
            }

            this.possibleMoves = this.engine.get_possible_moves(val, idx);

            const moves = this.engine.moves({ square: idx });
            console.log(moves)
            moves.forEach(m => console.log(m));
        },
        svgPoint(event) {
            const svg = this.$refs.svgRef;
            let pt = svg.createSVGPoint();
            pt.x = event.clientX;
            pt.y = event.clientY;
            return pt.matrixTransform(svg.getScreenCTM().inverse());
        },
        onPointerDown(square, event) {
            event.preventDefault();
            const pointer = this.svgPoint(event);

            this.selectPiece(square);

            this.draggingSquare = square.name;
            this.dragStartSquare = square;

            this.dragPositions[square.name] = {
                x: pointer.x - (square.x + 0.5),
                y: pointer.y - (square.y + 0.5),
            };

            event.target.setPointerCapture(event.pointerId);
        },
        onPointerMove(event) {
            if (!this.draggingSquare) return;

            const pointer = this.svgPoint(event);
            const square = this.dragStartSquare;
            if (!square) return;

            this.dragPositions[this.draggingSquare] = {
                x: pointer.x - (square.x + 0.5),
                y: pointer.y - (square.y + 0.5),
            };
        },
        onPointerUp(event) {
            if (!this.draggingSquare) return;

            const pointer = this.svgPoint(event);

            // Nearest square index:
            const newX = Math.min(boardSize - 1, Math.max(0, Math.floor(pointer.x)));
            const newY = Math.min(boardSize - 1, Math.max(0, Math.floor(pointer.y)));

            const draggingIndex = squareNameToIndex(this.draggingSquare);
            const newIndex = newY * 8 + newX;

            console.log(`Moved ${draggingIndex} to ${newIndex}`);

            if (this.engine.move(draggingIndex, newIndex)) {
                this.possibleMoves = kEmptyBitboard;
                this.boardVersion++;
                this.$forceUpdate();
            }

            delete this.dragPositions[this.draggingSquare];
            this.draggingSquare = null;
            this.dragStartSquare = null;
        },
        getPieceTransform(square) {
            const offset = this.dragPositions[square.name] || { x: 0, y: 0 };
            return `translate(${square.x + offset.x} ${square.y + offset.y}) scale(${1 / 45})`;
        },
        handlePieceMove(squareName, newPos) {
            console.log("Piece moved from", squareName, "to", newPos);
        },
    },
};
</script>

<style scoped>
.light {
    fill: #f0d9b5;
}

.dark {
    fill: #b58863;
}
</style>