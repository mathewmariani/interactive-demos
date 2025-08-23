<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <svg
        ref="svgRef"
        viewBox="0 0 8 8"
        width="512"
        height="512"
        @pointermove="onPointerMove"
        @pointerup="onPointerUp"
        @pointercancel="onPointerUp"
      >
        <!-- squares -->
        <g v-for="square in squares">
          <rect
            :key="square.name"
            :x="square.x"
            :y="square.y"
            width="1"
            height="1"
            :class="square.color"
          />
        </g>

        <!-- square name -->
        <text
          v-for="square in squares"
          :key="square.name + '-debug'"
          :x="square.x + 0.05"
          :y="square.y + 0.95"
          font-size="0.2"
          fill="red"
        >
          {{ square.san || '' }}
        </text>

        <!-- pieces -->
        <template v-for="square in squares" :key="square.name + '-piece'">
          <g
            v-if="square && square.piece && square.name !== draggingSquare"
            :key="square.name + '-piece'"
            :transform="getPieceTransform(square)"
          >
            <g v-html="pieceMap[square.piece]"></g>
          </g>

          <!-- interaction -->
          <rect
            v-if="square && square.piece"
            :key="square.name + '-hover'"
            :x="square.x"
            :y="square.y"
            width="1"
            height="1"
            fill="transparent"
            :style="{ cursor: getPieceCursor(square) }"
            @pointerdown="onPointerDown(square, $event)"
          />
        </template>

        <template
          v-for="move in possibleMoves"
          :key="`${move.from}-${move.to}`"
        >
          <circle
            :cx="indexToCoords(move.to).x + 0.5"
            :cy="indexToCoords(move.to).y + 0.5"
            r="0.20"
            fill="rgba(200, 214, 229, 0.85)"
          />
        </template>

        <!-- dragged -->
        <template v-for="square in squares">
          <g
            v-if="square && square.piece && square.name === draggingSquare"
            :key="square.name + '-dragging'"
            :transform="getPieceTransform(square)"
          >
            <g v-html="pieceMap[square.piece]"></g>
          </g>
        </template>
      </svg>
    </div>

    <!-- New toggle form -->
    <div class="mt-3">
      <div
        class="btn-toolbar mb-3"
        role="toolbar"
        aria-label="Toolbar with button groups"
      >
        <div class="btn-group me-2" role="group" aria-label="Reset and Clear">
          <button type="button" class="btn btn-primary" @click="onReset">
            Reset
          </button>
          <button type="button" class="btn btn-primary" @click="onClear">
            Clear
          </button>
        </div>
        <div class="btn-group me-2" role="group" aria-label="Reset and Clear">
          <button type="button" class="btn btn-primary" @click="onUndo">
            Undo
          </button>
          <button type="button" class="btn btn-primary" @click="onRedo">
            Redo
          </button>
        </div>
      </div>
      <div class="input-group mb-3">
        <button class="btn btn-primary" @click="loadFEN" type="button">
          Load FEN
        </button>
        <input
          type="text"
          class="form-control"
          placeholder="Enter FEN"
          v-model="fenInput"
          @keyup.enter="loadFEN"
        />
      </div>
      <h2>Details:</h2>
      <p>Zobrist Hash: {{ getHash() }}</p>
      <p>Castling Rights: {{ getCastlingRights() }}</p>
      <p>Turn: {{ getTurn() }}</p>
      <p>Check: {{ inCheck() }}</p>
      <p>Checkmate: {{ isCheckmate() }}</p>
    </div>
  </figure>
</template>

<script>
  import { shallowReactive, reactive } from 'vue';
  import Module from '@/modules/demos.js';

  import WhitePawn from '@/assets/chess/Chess_plt45.svg?raw';
  import WhiteKnight from '@/assets/chess/Chess_nlt45.svg?raw';
  import WhiteBishop from '@/assets/chess/Chess_blt45.svg?raw';
  import WhiteRook from '@/assets/chess/Chess_rlt45.svg?raw';
  import WhiteQueen from '@/assets/chess/Chess_qlt45.svg?raw';
  import WhiteKing from '@/assets/chess/Chess_klt45.svg?raw';

  import BlackPawn from '@/assets/chess/Chess_pdt45.svg?raw';
  import BlackKnight from '@/assets/chess/Chess_ndt45.svg?raw';
  import BlackBishop from '@/assets/chess/Chess_bdt45.svg?raw';
  import BlackRook from '@/assets/chess/Chess_rdt45.svg?raw';
  import BlackQueen from '@/assets/chess/Chess_qdt45.svg?raw';
  import BlackKing from '@/assets/chess/Chess_kdt45.svg?raw';

  const pieceMap = {
    '0_1': WhitePawn,
    '0_2': WhiteKnight,
    '0_3': WhiteBishop,
    '0_4': WhiteRook,
    '0_5': WhiteQueen,
    '0_6': WhiteKing,

    '8_1': BlackPawn,
    '8_2': BlackKnight,
    '8_3': BlackBishop,
    '8_4': BlackRook,
    '8_5': BlackQueen,
    '8_6': BlackKing,
  };

  const SAN = [
    'a8',
    'b8',
    'c8',
    'd8',
    'e8',
    'f8',
    'g8',
    'h8',
    'a7',
    'b7',
    'c7',
    'd7',
    'e7',
    'f7',
    'g7',
    'h7',
    'a6',
    'b6',
    'c6',
    'd6',
    'e6',
    'f6',
    'g6',
    'h6',
    'a5',
    'b5',
    'c5',
    'd5',
    'e5',
    'f5',
    'g5',
    'h5',
    'a4',
    'b4',
    'c4',
    'd4',
    'e4',
    'f4',
    'g4',
    'h4',
    'a3',
    'b3',
    'c3',
    'd3',
    'e3',
    'f3',
    'g3',
    'h3',
    'a2',
    'b2',
    'c2',
    'd2',
    'e2',
    'f2',
    'g2',
    'h2',
    'a1',
    'b1',
    'c1',
    'd1',
    'e1',
    'f1',
    'g1',
    'h1',
  ];

  const FILES = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];
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
  const kPieceTypeMask = 0b00000111; // 7

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
        possibleMoves: {},
        fenInput: '',
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
              idx: idx,
              san: SAN[idx],
              x: file,
              y: 8 - rank,
              color: (file + rank) % 2 === 0 ? 'light' : 'dark',
              piece: pieceKey,
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
      loadFEN() {
        this.onReset();
        this.engine.load(this.fenInput);
        this.boardVersion++;
        this.$forceUpdate();
      },
      onUndo() {
        this.engine.undo();
        this.possibleMoves = {};
        this.boardVersion++;
        this.$forceUpdate();
      },
      onRedo() {
        this.engine.redo();
        this.possibleMoves = {};
        this.boardVersion++;
        this.$forceUpdate();
      },
      onReset() {
        this.engine.reset();
        this.possibleMoves = {};
        this.boardVersion++;
        this.$forceUpdate();

        const moves = this.engine.moves();
        moves.forEach((m) => console.log(m));
      },
      getHash() {
        return this.engine ? this.engine.zobrist() : '';
      },
      getCastlingRights() {
        return this.engine ? this.engine.castlingRights() : 'Never';
      },
      getTurn() {
        return this.engine
          ? this.engine.turn().value
            ? 'Black'
            : 'White'
          : '';
      },
      inCheck() {
        return this.engine ? (this.engine.inCheck() ? 'True' : 'False') : '';
      },
      isCheckmate() {
        return this.engine
          ? this.engine.isCheckmate()
            ? 'True'
            : 'False'
          : '';
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
          this.possibleMoves = {};
          return;
        }

        this.possibleMoves = this.engine.moves({ square: idx });
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
        const newX = Math.min(
          boardSize - 1,
          Math.max(0, Math.floor(pointer.x))
        );
        const newY = Math.min(
          boardSize - 1,
          Math.max(0, Math.floor(pointer.y))
        );

        const draggingIndex = squareNameToIndex(this.draggingSquare);
        const newIndex = newY * 8 + newX;

        if (this.engine.move(draggingIndex, newIndex)) {
          this.possibleMoves = {};
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
        console.log('Piece moved from', squareName, 'to', newPos);
      },
      indexToCoords(idx) {
        return {
          x: idx % 8,
          y: Math.floor(idx / 8),
        };
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
