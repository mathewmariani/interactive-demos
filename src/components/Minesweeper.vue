<!-- minesweeper.vue -->
<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <button type="button" class="btn btn-primary btn-lg btn-block mb-4" @click="reset">
        {{ getStatus }}
      </button>
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g v-for="loc in locations" @click="explore(loc)" @contextmenu="flag(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y" width="1" height="1"></rect>
          <text
            v-if="isExplored(loc) && !isMine(loc)"
            text-anchor="middle"
            :font-size="0.45"
            :x="loc.x + 0.5"
            :y="loc.y + 0.5"
            :dy="0.15"
          >
            {{ getMineCount(loc) }}
          </text>
          <circle
            v-if="isExplored(loc) && isMine(loc)"
            :cx="loc.x + 0.5"
            :cy="loc.y + 0.5"
            r="0.25"
            fill="rgb(229, 74, 58)"
          ></circle>
          <g v-if="isFlag(loc)" stroke="rgb(51, 151, 217)" stroke-width="0.1">
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.2" :x2="loc.x + 0.8" :y2="loc.y + 0.8"></line>
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.8" :x2="loc.x + 0.8" :y2="loc.y + 0.2"></line>
          </g>
        </g>
      </svg>
    </div>
  </figure>
</template>

<script>
import { shallowReactive, onMounted } from 'vue';
import Module from '../modules/demos.js';

export default {
  setup() {
    const state = shallowReactive({
      gridWorld: null,
      board: null,
      gameover: false,
      has_won: false,
    });

    onMounted(async () => {
      try {
        const wasmModule = await Module();
        state.gridWorld = new wasmModule.GridWorld(10, 8);
        state.board = new wasmModule.Minesweeper();
      } catch (e) {
        console.error('Failed to load WebAssembly:', e);
      }
    });

    return {
      state,
    };
  },
  computed: {
    getWidth() {
      return this.state.gridWorld?.width_readonly || 0;
    },
    getHeight() {
      return this.state.gridWorld?.height_readonly || 0;
    },
    locations() {
      if (!this.state.gridWorld) return [];
      const keys = this.state.gridWorld.locations().keys();
      return Array.from({ length: keys.size() }, (_, i) => keys.get(i));
    },
    getStatus() {
      return this.state.gameover ? (this.state.has_won ? "😎" : "😵") : "🙂";
    },
  },
  methods: {
    reset() {
      if (!this.state.board) {
        return;
      }
      this.state.board.reset();
      this.state.gameover = false;
      this.state.has_won = false;
      this.$forceUpdate();
    },
    flag(location) {
      if (!this.state.board || this.state.gameover || this.state.board.isExplored(location)) {
        return;
      }
      this.state.board.toggleFlag(location);
      if (this.state.board.checkWin()) {
        this.state.has_won = true;
        this.state.gameover = true;
      }
      this.$forceUpdate();
    },
    explore(location) {
      if (!this.state.board || this.state.gameover) {
        return;
      }
      this.state.board.explore(location);
      if (this.state.board.isMine(location)) {
        this.state.gameover = true;
      } else if (this.state.board.checkWin()) {
        this.state.has_won = true;
        this.state.gameover = true;
      }
      this.$forceUpdate();
    },
    getMineCount(location) {
      return this.state.board?.getMineCount(location) || "";
    },
    isMine(location) {
      return this.state.board?.isMine(location) || false;
    },
    isExplored(location) {
      return this.state.board?.isExplored(location) || false;
    },
    isFlag(location) {
      return this.state.board?.isFlag(location) || false;
    },
    classFor(location) {
      return this.state.board?.isExplored(location) ? "explored" : "";
    },
  },
};
</script>

<style scoped>
.cell {
  fill: #a2d149;
  stroke: hsl(60, 0%, 100%);
  stroke-width: 0.02px;
  transition: fill 0.2s ease;
}
.explored {
  fill: hsl(45, 20%, 70%);
}
.cell:hover:not(.explored) {
  fill: #b8e663; /* Lighter green on hover for unexplored cells */
  cursor: pointer; /* Indicates the cell is clickable */
}
svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  height: auto;
}
</style>