<template>
  <figure id="diagram1">
    <button type="button" class="btn btn-primary mb-4" @click="reset">
      Reset
    </button>
    <main class="d-flex justify-content-center">
      <svg :view-box.camel="`0 0 ${getWidth} ${getHeight}`">
        <g v-for="loc in locations" @click="set(loc)">
          <rect class="cell" :x="loc.x" :y="loc.y" width="1" height="1"></rect>

          <circle v-if="get(loc) === 2" :cx="loc.x + 0.5" :cy="loc.y + 0.5" r="0.4" fill="none"
            stroke="rgb(229, 74, 58)" stroke-width="0.1"></circle>

          <g v-if="get(loc) === 1" stroke="rgb(51, 151, 217)" stroke-width="0.1">
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.2" :x2="loc.x + 0.8" :y2="loc.y + 0.8"></line>
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.8" :x2="loc.x + 0.8" :y2="loc.y + 0.2"></line>
          </g>
        </g>
      </svg>
    </main>
  </figure>
</template>

<script>
import { shallowReactive } from 'vue';
import Module from '@/modules/demos.js'
const wasmModule = await Module()

let gridWorld = new wasmModule.GridWorld(3, 3)
let board = new wasmModule.TicTacToe()
let bot = new wasmModule.Bot(board, 2)

export default {
  data: () => ({
    gridWorld: shallowReactive(gridWorld),
    board: shallowReactive(board),
    bot: shallowReactive(bot),
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
  },
  methods: {
    get(location) {
      return this.board.get(location.x, location.y)
    },
    set(location) {
      let who = this.board.has_winner();
      if (who != 0 || this.board.is_complete()) {
        return;
      }

      let where = this.board.get(location.x, location.y)
      if (where != 0) {
        return;
      }

      this.board.set(location.x, location.y, 1)
      this.bot.best_move()
      this.$forceUpdate()
    },
    reset() {
      this.board.reset()
      this.$forceUpdate()
    },
  }
};
</script>

<style scoped>
.cell {
  fill: hsl(60, 10%, 90%);
  stroke: hsl(60, 0%, 100%);
  stroke-width: 0.02px;
}

svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  /* Optional: Makes it responsive */
  height: auto;
}
</style>
