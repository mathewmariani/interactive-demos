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
            :fill="getNumberColor(getMineCount(loc))"
            font-weight="bolder"
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
import { shallowReactive } from 'vue';
import Module from '../modules/demos.js'
const wasmModule = await Module()

let gridWorld = new wasmModule.GridWorld(10, 8)
let board = new wasmModule.Minesweeper()

export default {
  data: () => ({
    gridWorld: shallowReactive(gridWorld),
    board: shallowReactive(board),
    gameover: false,
    has_won: false,
  }),
  computed: {
    getWidth() {
      return this.gridWorld?.width_readonly || 0;
    },
    getHeight() {
      return this.gridWorld?.height_readonly || 0;
    },
    locations() {
      if (!this.gridWorld) return [];
      const keys = this.gridWorld.locations().keys();
      return Array.from({ length: keys.size() }, (_, i) => keys.get(i));
    },
    getStatus() {
      return this.gameover ? (this.has_won ? "😎" : "😵") : "🙂";
    },
  },
  methods: {
    reset() {
      if (!this.board) {
        return;
      }
      this.board.reset();
      this.gameover = false;
      this.has_won = false;
      this.$forceUpdate();
    },
    flag(location) {
      if (!this.board || this.gameover || this.board.isExplored(location)) {
        return;
      }
      this.board.toggleFlag(location);
      if (this.board.checkWin()) {
        this.has_won = true;
        this.gameover = true;
      }
      this.$forceUpdate();
    },
    explore(location) {
      if (!this.board || this.gameover) {
        return;
      }
      this.board.explore(location);
      if (this.board.isMine(location)) {
        this.gameover = true;
      } else if (this.board.checkWin()) {
        this.has_won = true;
        this.gameover = true;
      }
      this.$forceUpdate();
    },
    getMineCount(location) {
      return this.board?.getMineCount(location) || "";
    },
    isMine(location) {
      return this.board?.isMine(location) || false;
    },
    isExplored(location) {
      return this.board?.isExplored(location) || false;
    },
    isFlag(location) {
      return this.board?.isFlag(location) || false;
    },
    classFor(location) {
      let checkered = ((location.x + location.y) % 2 === 0) ? "dark" : "light";
      let explored = this.board?.isExplored(location) ? "explored" : "";
      return `${explored} ${checkered}`.trim();
    },
    getNumberColor(count) {
      const colorMap = {
        1: '#0000FF', // Blue
        2: '#008000', // Green
        3: '#FF0000', // Red
        4: '#000080', // Dark Blue
        5: '#800000', // Maroon
        6: '#008080', // Teal
        7: '#000000', // Black
        8: '#808080', // Gray
      };
      return colorMap[count] || '#000000';
    },
  },
};
</script>

<style scoped>
.cell {
  stroke: #ffffff;
  stroke-width: 0.02px;
  transition: fill 0.2s ease;
}

.light {
  fill: #A7D948;
}

.dark {
  fill: #8ECC39;
}

.light.explored {
  fill: #E5C29F;
}

.dark.explored {
  fill: #D7B899;
}

.cell:hover:not(.explored) {
  fill: #b8e663;
  cursor: pointer;
}

svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  height: auto;
}
</style>