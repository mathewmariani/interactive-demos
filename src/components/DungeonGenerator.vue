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
          <text
            text-anchor="middle"
            :font-size="0.45"
            :x="loc.x + 0.5"
            :y="loc.y + 0.5"
            :dy="0.15"
            font-weight="bolder"
          >
            {{ countNeighbors(loc) }}
          </text>
        </g>

      </svg>
    </div>
  </figure>
</template>

<script>
import { shallowReactive } from 'vue';
import Module from '../modules/demos.js';
const wasmModule = await Module();

let gridWorld = new wasmModule.GridWorld(10, 8);
let dungeon = new wasmModule.DungeonGenerator();

export default {
  data: () => ({
    gridWorld: shallowReactive(gridWorld),
    dungeon: shallowReactive(dungeon),
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
    countNeighbors(location) { return 0 },
    classFor(location) {
      return "empty";
    },
    isSpawn(location) { return this.gridWorld.isSpawn(location); },
    isNormal(location) { return this.gridWorld.isNormal(location); },
    isItem(location) { return this.gridWorld.isItem(location); },
    isShop(location) { return this.gridWorld.isShop(location); },
    isSecret(location) { return this.gridWorld.isSecret(location); },
    isBoss(location) { return this.gridWorld.isBoss(location); },
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

.room {
  fill: #FFD700;
}

.empty {
  fill: #0F1419;
}

svg {
  max-width: 512px;
  max-height: 512px;
  width: 100%;
  height: auto;
}
</style>