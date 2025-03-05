<template>
  
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <button type="button" class="btn btn-primary btn-lg btn-block mb-4" v-on:click.native="reset">
        {{ getStatus }}
      </button>
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`" @contextmenu.prevent="handleRightClick">
        <g v-for="loc in locations" @click.left="explore(loc)" @click.right="flag(loc)">
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
            fill="rgb(229, 74, 58)"></circle>

            <g
            v-if="isFlag(loc)"
            stroke="rgb(51, 151, 217)"
            stroke-width="0.1"
          >
            <line
              :x1="loc.x + 0.2"
              :y1="loc.y + 0.2"
              :x2="loc.x + 0.8"
              :y2="loc.y + 0.8"></line>
            <line
              :x1="loc.x + 0.2"
              :y1="loc.y + 0.8"
              :x2="loc.x + 0.8"
              :y2="loc.y + 0.2"></line>
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
      return this.gridWorld.width_readonly
    },
    getHeight() {
      return this.gridWorld.height_readonly
    },
    locations() {
      const keys = this.gridWorld.locations().keys()
      const result = []
      for (let i = 0; i < keys.size(); i++) {
        const location = keys.get(i)
        result.push(location)
      }
      return result
    },
    getStatus() {
      return this.gameover ? (this.has_won ? "😎" : "😵") : "🙂"
    }
  },
  methods: {
    reset() {
      this.board.reset()
      this.gameover = false;
      this.has_won = false;
      this.$forceUpdate()
    },
    flag(location) {
      if (this.gameover || this.board.isExplored(location)) {
        console.log("here")
        return;
      }

      this.board.toggleFlag(location)
      if (this.board.checkWin()) {
        this.has_won = true
        this.gameover = true
      }
      this.$forceUpdate()
    },
    explore(location) {
      if (this.gameover) {
        return;
      }

      this.board.explore(location)

      if (this.board.isMine(location)) {
        this.gameover = true
      }
      else if (this.board.checkWin()) {
        this.has_won = true
        this.gameover = true
      }
      this.$forceUpdate()
    },
    getMineCount(location) {
      let n = this.board.getMineCount(location)
      return n == 0 ? "" : n
    },
    isMine(location) {
      return this.board.isMine(location)
    },
    isExplored(location) {
      return this.board.isExplored(location)
    },
    isFlag(location) {
      return this.board.isFlag(location)
    },
    classFor(location) {
      return this.board.isExplored(location) ? "explored" : "";
    },
  }
};
</script>

<style scoped>
.cell {
  fill: #a2d149;
  stroke: hsl(60, 0%, 100%);
  stroke-width: 0.02px;
}

.explored {
  fill: hsl(45, 20%, 70%);
}

svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  height: auto;
}
</style>
