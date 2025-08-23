<template>
  <figure id="diagram1">
    <div class="d-flex flex-column align-items-center">
      <button type="button" class="btn btn-primary btn-lg btn-block mb-4" @click="reset">
        {{ getStatus }}
      </button>
      <svg :viewBox="`${0} ${0} ${getWidth} ${getHeight}`" @contextmenu.prevent>
        <g v-for="loc in locations" @click="explore(loc)" @contextmenu="flag(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y" width="1" height="1"></rect>
          <text v-if="isExplored(loc) && !isMine(loc)" text-anchor="middle" :font-size="0.45" :x="loc.x + 0.5"
            :y="loc.y + 0.5" :dy="0.15" :fill="getNumberColor(getMineCount(loc))" font-weight="bolder">
            {{ getMineCount(loc) }}
          </text>
          <circle v-if="isExplored(loc) && isMine(loc)" :cx="loc.x + 0.5" :cy="loc.y + 0.5" r="0.25"
            fill="rgb(229, 74, 58)"></circle>
          <g v-if="isFlag(loc)" stroke="rgb(51, 151, 217)" stroke-width="0.1">
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.2" :x2="loc.x + 0.8" :y2="loc.y + 0.8"></line>
            <line :x1="loc.x + 0.2" :y1="loc.y + 0.8" :x2="loc.x + 0.8" :y2="loc.y + 0.2"></line>
          </g>
        </g>
      </svg>
    </div>
  </figure>
</template>

<script setup>
import { ref, computed, onMounted } from "vue";
import Module from "@/modules/demos.js";

let board = null;
const gameover = ref(false);
const hasWon = ref(false);
const tick = ref(0);

onMounted(async () => {
  const wasm = await Module();
  board = new wasm.Minesweeper();
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

const getStatus = computed(() =>
  gameover.value ? (hasWon.value ? "😎" : "😵") : "🙂"
);

function reset() {
  if (!board) return;
  board.reset();
  gameover.value = false;
  hasWon.value = false;
  tick.value++;
}

function flag(location) {
  if (!board || gameover.value || board.isExplored(location)) return;
  board.toggleFlag(location);
  if (board.checkWin()) {
    hasWon.value = true;
    gameover.value = true;
  }
  tick.value++;
}

function explore(location) {
  if (!board || gameover.value) return;
  board.explore(location);
  if (board.isMine(location)) {
    gameover.value = true;
  } else if (board.checkWin()) {
    hasWon.value = true;
    gameover.value = true;
  }
  tick.value++;
}

function getMineCount(location) {
  tick.value;
  return board?.getMineCount(location) || "";
}

function isMine(location) {
  tick.value;
  return board?.isMine(location) || false;
}

function isExplored(location) {
  tick.value;
  return board?.isExplored(location) || false;
}

function isFlag(location) {
  tick.value;
  return board?.isFlag(location) || false;
}

function classFor(location) {
  tick.value;
  const checkered = (location.x + location.y) % 2 === 0 ? "dark" : "light";
  const explored = board?.isExplored(location) ? "explored" : "";
  return `${explored} ${checkered}`.trim();
}

function getNumberColor(count) {
  const colorMap = {
    1: "#0000FF",
    2: "#008000",
    3: "#FF0000",
    4: "#000080",
    5: "#800000",
    6: "#008080",
    7: "#000000",
    8: "#808080",
  };
  return colorMap[count] || "#000000";
}
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