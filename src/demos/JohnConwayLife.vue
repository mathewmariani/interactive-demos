<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <div class="btn-group mb-3" role="group" aria-label="Game of Life Controls">
        <button type="button" class="btn btn-primary" @click="step">▶️</button>
        <button type="button" class="btn btn-primary" @click="reset">🔄</button>
        <button type="button" class="btn btn-primary" @click="clear">🗑️</button>
      </div>
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g v-for="loc in locations" @click="toggle(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y" width="1" height="1"></rect>
          <text text-anchor="middle" :font-size="0.45" :x="loc.x + 0.5" :y="loc.y + 0.5" :dy="0.15"
            font-weight="bolder">
            {{ countNeighbors(loc) }}
          </text>
        </g>

      </svg>
    </div>
  </figure>
</template>

<script>
import { shallowReactive } from 'vue';
import Module from '@/modules/demos.js';
const wasmModule = await Module();

let gridWorld = new wasmModule.GridWorld(10, 8);
let life = new wasmModule.Life();

export default {
  data: () => ({
    gridWorld: shallowReactive(gridWorld),
    life: shallowReactive(life),
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
  },
  methods: {
    step() { this.life.step(); this.$forceUpdate(); },
    reset() { this.life.reset(); this.$forceUpdate(); },
    clear() { this.life.clear(); this.$forceUpdate(); },
    countNeighbors(location) { return this.life.countNeighbors(location); },

    toggle(location) {
      this.life.toggle(location);
      this.$forceUpdate();
    },
    classFor(location) {
      return this.life.isAlive(location) ? "alive" : "dead";
    },
  },
};
</script>

<style scoped>
.cell {
  stroke: #ffffff;
  stroke-width: 0.02px;
  transition: fill 0.2s ease;
  cursor: pointer;
}

.alive {
  fill: #FFD700;
}

.dead {
  fill: #0F1419;
}

svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  height: auto;
}
</style>