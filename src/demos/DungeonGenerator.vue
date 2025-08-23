<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <div
        class="btn-group mb-3"
        role="group"
        aria-label="Game of Life Controls"
      >
        <button type="button" class="btn btn-primary" @click="generate">
          Generate 🎲
        </button>
      </div>
      <svg :viewBox="`0 0 ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g v-for="loc in locations" :key="`${loc.x}-${loc.y}-${tick.value}`">
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
            {{ hasIconography(loc) }}
          </text>
        </g>
      </svg>
    </div>
  </figure>
</template>

<script setup>
  import { ref, computed, onMounted } from 'vue';
  import Module from '@/modules/demos.js';

  let dungeon = null;
  const tick = ref(0);

  onMounted(async () => {
    let wasm = await Module();
    dungeon = new wasm.DungeonGenerator();
  });

  const getWidth = computed(() => 11);
  const getHeight = computed(() => 11);

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

  // methods
  function generate() {
    if (!dungeon) return;
    console.log('here');
    dungeon.clear();
    dungeon.generate();
    tick.value++;
  }

  function classFor(location) {
    let isRoom =
      isSpawn(location) ||
      isItem(location) ||
      isShop(location) ||
      isSecret(location) ||
      isBoss(location) ||
      isNormal(location);
    return isRoom ? 'room' : 'empty';
  }

  function hasIconography(location) {
    if (isSpawn(location)) return '👼';
    if (isItem(location)) return '👑';
    if (isShop(location)) return '🪙';
    if (isSecret(location)) return '❓';
    if (isSuperSecret(location)) return '⁉️';
    if (isBoss(location)) return '💀';
    return '';
  }

  function isSpawn(loc) {
    return dungeon?.isSpawn(loc) || false;
  }

  function isNormal(loc) {
    return dungeon?.isNormal(loc) || false;
  }

  function isItem(loc) {
    return dungeon?.isItem(loc) || false;
  }

  function isShop(loc) {
    return dungeon?.isShop(loc) || false;
  }

  function isSecret(loc) {
    return dungeon?.isSecret(loc) || false;
  }

  function isSuperSecret(loc) {
    return dungeon?.isSuperSecret(loc) || false;
  }

  function isBoss(loc) {
    return dungeon?.isBoss(loc) || false;
  }
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
    fill: #0f1419;
  }

  svg {
    max-width: 512px;
    max-height: 512px;
    width: 100%;
    height: auto;
  }
</style>
