<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <div
        class="btn-group mb-3"
        role="group"
        aria-label="Game of Life Controls"
      >
        <button type="button" class="btn btn-primary" @click="step">▶️</button>
        <button type="button" class="btn btn-primary" @click="reset">🔄</button>
        <button type="button" class="btn btn-primary" @click="clear">🗑️</button>
      </div>
      <svg :viewBox="`0 0 ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g
          v-for="loc in locations"
          :key="`${loc.x}-${loc.y}`"
          @click="toggle(loc)"
        >
          <rect
            :class="'cell ' + classFor(loc)"
            :x="loc.x"
            :y="loc.y"
            width="1"
            height="1"
          />
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

<script setup>
  import { ref, computed, onMounted } from 'vue';
  import Module from '@/modules/demos.js';

  let life = null;
  const tick = ref(0);

  onMounted(async () => {
    const wasm = await Module();
    life = new wasm.Life();
    tick.value++;
  });

  const getWidth = computed(() => 10);
  const getHeight = computed(() => 8);

  const locations = computed(() => {
    const arr = [];
    const w = getWidth.value;
    const h = getHeight.value;
    for (let y = 0; y < h; y++) {
      for (let x = 0; x < w; x++) {
        arr.push({ x, y });
      }
    }
    return arr;
  });

  function step() {
    if (!life) return;
    life.step();
    tick.value++;
  }

  function reset() {
    if (!life) return;
    life.reset();
    tick.value++;
  }

  function clear() {
    if (!life) return;
    life.clear();
    tick.value++;
  }

  function toggle(location) {
    if (!life) return;
    life.toggle(location);
    tick.value++;
  }

  function countNeighbors(location) {
    tick.value;
    return life?.countNeighbors(location) ?? '';
  }

  function classFor(location) {
    tick.value;
    return life?.isAlive(location) ? 'alive' : 'dead';
  }
</script>

<style scoped>
  .cell {
    stroke: #ffffff;
    stroke-width: 0.02px;
    transition: fill 0.2s ease;
    cursor: pointer;
  }

  .alive {
    fill: #ffd700;
  }

  .dead {
    fill: #0f1419;
  }

  svg {
    max-width: 512px;
    max-height: 512px;
    width: 100%;
    height: auto;
  }
</style>
