<template>
    <figure id="diagram1">
        <div class="d-flex flex-column align-items-center">
            <svg ref="svgRef" viewBox="0 0 8 8" width="512" height="512" @pointermove="onPointerMove"
                @pointerup="onPointerUp" @pointercancel="onPointerUp">
                <!-- Board squares -->
                <rect v-for="square in squares" :key="square.name" :x="square.x" :y="square.y" width="1" height="1"
                    :class="square.color" />

                <!-- Pieces -->
                <template v-for="square in squares" :key="square.name + '-piece'">
                    <g v-if="square && square.piece" :transform="getPieceTransform(square)"
                        @pointerdown="startDrag(square, $event)" style="cursor: grab;">
                        <component :is="pieceMap[square.piece]" :x="square.x" :y="square.y"
                            @move="handlePieceMove(square.name, $event)" />
                    </g>
                </template>

                <!-- debug -->
                <text v-for="square in squares" :key="square.name + '-debug'" :x="square.x + 0.05" :y="square.y + 0.95"
                    font-size="0.2" fill="red">
                    {{ square.piece || '' }}
                </text>
            </svg>
        </div>
    </figure>
</template>

<script>
import { shallowReactive, markRaw, reactive, ref } from "vue";
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
    "8_1": markRaw(WhiteKing),
    "8_2": markRaw(WhitePawn),
    "8_3": markRaw(WhiteKnight),
    "8_4": markRaw(WhiteBishop),
    "8_5": markRaw(WhiteRook),
    "8_6": markRaw(WhiteQueen),

    "16_1": markRaw(BlackKing),
    "16_2": markRaw(BlackPawn),
    "16_3": markRaw(BlackKnight),
    "16_4": markRaw(BlackBishop),
    "16_5": markRaw(BlackRook),
    "16_6": markRaw(BlackQueen),
};

const FILES = ["a", "b", "c", "d", "e", "f", "g", "h"];
const boardSize = 8;

function squareNameToIndex(name) {
    const fileChar = name[0];               // 'e'
    const rankChar = parseInt(name[1]);    // 2

    const file = FILES.indexOf(fileChar);  // 4 for 'e'
    const rank = 8 - rankChar;              // ranks count down from 8 at top

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
            boardVersion: 0,  // reactive counter
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
            this.$forceUpdate();  // Force Vue to re-render squares

            delete this.dragPositions[this.draggingSquare];
            this.draggingSquare = null;
            this.dragStartPointer = null;
            this.dragStartPos = null;
        },
        getPieceTransform(square) {
            // If dragging, offset the piece by dragPositions:
            const offset = this.dragPositions[square.name] || { x: 0, y: 0 };

            // Because pieces are scaled by 1/45, we translate before scaling:
            // Move piece by (square.x + offset.x, square.y + offset.y)
            // Then scale down
            return `translate(${square.x + offset.x} ${square.y + offset.y}) scale(${1 / 45
                })`;
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
.cell {
    stroke: #000;
    stroke-width: 0px;
}

.light {
    fill: #f0d9b5;
}

.dark {
    fill: #b58863;
}

svg {
    max-width: 512px;
    width: 100%;
    height: auto;
}
</style>