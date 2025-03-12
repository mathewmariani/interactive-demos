<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <div class="btn-group mb-3" role="group" aria-label="Game of Life Controls">
        <button type="button" class="btn btn-primary" @click="generate">Generate 🎲</button>
      </div>
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g v-for="loc in locations" @click="toggle(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y" width="1" height="1"></rect>
          <text text-anchor="middle" :font-size="0.45" :x="loc.x + 0.5" :y="loc.y + 0.5" :dy="0.15"
            font-weight="bolder">
            {{ hasIconography(loc) }}
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

let gridWorld = new wasmModule.GridWorld(11, 11);
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
      let isRoom = this.isSpawn(location) || this.isItem(location) || this.isShop(location) || this.isSecret(location) || this.isBoss(location) || this.isNormal(location);
      return isRoom ? "room" : "empty";
    },
    hasIconography(location) {
      if (this.isSpawn(location)) return "👼";
      if (this.isItem(location)) return "👑";
      if (this.isShop(location)) return "🪙";
      if (this.isSecret(location)) return "❓";
      if (this.isSuperSecret(location)) return "⁉️";
      if (this.isBoss(location)) return "💀";
      return "";
    },
    generate() {
      this.dungeon.clear();
      this.dungeon.generate();
      this.$forceUpdate();
    },
    isSpawn(location) { return this.dungeon.isSpawn(location); },
    isNormal(location) { return this.dungeon.isNormal(location); },
    isItem(location) { return this.dungeon.isItem(location); },
    isShop(location) { return this.dungeon.isShop(location); },
    isSecret(location) { return this.dungeon.isSecret(location); },
    isSuperSecret(location) { return this.dungeon.isSuperSecret(location); },
    isBoss(location) { return this.dungeon.isBoss(location); },
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
  fill: #646464;
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