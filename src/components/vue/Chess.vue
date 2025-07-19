<template>
    <figure id="diagram1">
        <div class="d-flex flex-column align-items-center">
            <svg ref="svgRef" viewBox="0 0 8 8" width="512" height="512" @pointermove="onPointerMove"
                @pointerup="onPointerUp" @pointercancel="onPointerUp">
                <!-- Board squares -->
                <rect v-for="square in squares" :key="square.name" :x="square.x" :y="square.y" width="1" height="1"
                    :class="square.color" />

                <!-- Pieces and highlights -->
                <template v-for="square in squares" :key="square.name + '-piece'">

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

                    <g v-if="square && square.piece" :transform="getPieceTransform(square)"
                        @pointerdown="startDrag(square, $event)" style="cursor: grab;">
                        <component :is="pieceMap[square.piece]" :x="square.x" :y="square.y"
                            @move="handlePieceMove(square.name, $event)" />
                    </g>
                </template>

                <!-- debug -->
                <text v-for="square in squares" :key="square.name + '-debug'" :x="square.x + 0.05" :y="square.y + 0.95"
                    font-size="0.2" fill="red">
                    {{ square.piece || "" }}
                </text>
            </svg>
        </div>

        <!-- New toggle form -->
        <div class="mt-3">
            <h2>Bitboards</h2>
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

export const pieceMap = {
    "0_1": markRaw(WhiteKing),
    "0_2": markRaw(WhitePawn),
    "0_3": markRaw(WhiteKnight),
    "0_4": markRaw(WhiteBishop),
    "0_5": markRaw(WhiteRook),
    "0_6": markRaw(WhiteQueen),

    "8_1": markRaw(BlackKing),
    "8_2": markRaw(BlackPawn),
    "8_3": markRaw(BlackKnight),
    "8_4": markRaw(BlackBishop),
    "8_5": markRaw(BlackRook),
    "8_6": markRaw(BlackQueen),
};

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
                        x: file,
                        y: 8 - rank,
                        color: (file + rank) % 2 === 0 ? "light" : "dark",
                        piece: pieceKey,

                        // only highlight if corresponding toggle is true
                        isRook: this.showRooks && (rookBits & (1n << BigInt(idx))) !== 0n,
                        isBishop: this.showBishops && (bishopBits & (1n << BigInt(idx))) !== 0n,
                        isQueen: this.showQueens && (queenBits & (1n << BigInt(idx))) !== 0n,
                        isKnight: this.showKnights && (knightBits & (1n << BigInt(idx))) !== 0n,
                        isPawn: this.showPawns && (pawnBits & (1n << BigInt(idx))) !== 0n,
                        isKing: this.showKings && (kingBits & (1n << BigInt(idx))) !== 0n,
                    });
                }
            }
            return squares;
        },
    },
    methods: {
        svgPoint(event) {
            const svg = this.$refs.svgRef;
            let pt = svg.createSVGPoint();
            pt.x = event.clientX;
            pt.y = event.clientY;
            return pt.matrixTransform(svg.getScreenCTM().inverse());
        },
        startDrag(square, event) {
            event.preventDefault();
            this.draggingSquare = square.name;
            this.dragStartPointer = this.svgPoint(event);
            this.dragStartPos = { x: square.x, y: square.y };
            this.dragPositions[square.name] = { x: 0, y: 0 };
            event.target.setPointerCapture(event.pointerId);
        },
        onPointerMove(event) {
            if (!this.draggingSquare) return;
            const currentPointer = this.svgPoint(event);
            const startPointer = this.dragStartPointer;
            const offsetX = currentPointer.x - startPointer.x;
            const offsetY = currentPointer.y - startPointer.y;
            this.dragPositions[this.draggingSquare] = { x: offsetX, y: offsetY };
        },
        onPointerUp(event) {
            if (!this.draggingSquare) return;

            // Snap to nearest square:
            const pos = this.dragPositions[this.draggingSquare];
            const newX = Math.min(boardSize - 1, Math.max(0, Math.round(this.dragStartPos.x + pos.x)));
            const newY = Math.min(boardSize - 1, Math.max(0, Math.round(this.dragStartPos.y + pos.y)));

            const draggingIndex = squareNameToIndex(this.draggingSquare);
            const newIndex = newY * 8 + newX;

            console.log(`Moved ${draggingIndex} to board index: ${newIndex}`);

            this.engine.move(draggingIndex, newIndex);
            this.boardVersion++;
            this.$forceUpdate(); // Force Vue to re-render squares

            delete this.dragPositions[this.draggingSquare];
            this.draggingSquare = null;
            this.dragStartPointer = null;
            this.dragStartPos = null;
        },
        getPieceTransform(square) {
            const offset = this.dragPositions[square.name] || { x: 0, y: 0 };
            return `translate(${square.x + offset.x} ${square.y + offset.y}) scale(${1 / 45})`;
        },
        handlePieceMove(squareName, newPos) {
            console.log("Piece moved from", squareName, "to", newPos);
        },
        reset() {
            if (this.engine) {
                this.engine.reset();
                this.$forceUpdate();
            }
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