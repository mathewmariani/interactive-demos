<template>
  <button type="button" class="btn btn-primary mb-4" v-on:click.native="reset">
    Reset
  </button>
  <figure id="diagram1">
    <main class="d-flex justify-content-center">
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`">
        <g v-for="loc in locations" @click="explore(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y" width="1" height="1"></rect>
          <text
            v-if="isExplored(loc)"
            text-anchor="middle"
            :font-size="0.45"
            :x="loc.x + 0.5"
            :y="loc.y + 0.5"
            :dy="0.15"
          >
            {{ getMineCount(loc) }}
          </text>

          <circle
            v-if="isMine(loc)"
            :cx="loc.x + 0.5"
            :cy="loc.y + 0.5"
            r="0.25"
            fill="rgb(229, 74, 58)"></circle>
        </g>
      </svg>
    </main>
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
    reset() {
      console.log('reset clicked:', location)
      this.board.reset()
    },
    explore(location) {
      if (this.board.isMine(location.x, location.y)) {
        console.log('Game Over!')
      }
      else
      {
        // let count = this.board.getMineCount(location.x, location.y)
        // if (count > 0) {
        //   console.log('Location clicked:', location)
        //   return;
        // }
        // console.log('Cascade event:', location)
        console.log('Location clicked:', location)
        this.board.explore(location.x, location.y)
      }
      this.$forceUpdate()
    },
    getMineCount(location) {
      return this.board.getMineCount(location.x, location.y)
    },
    isMine(location) {
      return this.board.isMine(location.x, location.y)
    },
    isExplored(location) {
      return this.board.isExplored(location.x, location.y)
    },
    classFor(location) {
      return this.board.isExplored(location.x, location.y) ? "explored" : "";
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
