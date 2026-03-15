<template>

  <figure id="diagram1" class="card">
    <div class="card-header">
      <div class="hstack justify-content-between">
        <span class="badge text-bg-danger">
          <i class="bi bi-stopwatch"></i> : {{ timer.display }}
        </span>
        <button type="button" class="btn btn-dark" @click="reset">
          {{ getStatus }}
        </button>
        <span class="badge text-bg-danger">
          <i class="bi bi-flag-fill"></i> x {{ board?.getFlagCount() }}
        </span>
      </div>
    </div>
    <div class="vstack align-items-center">
      <svg :viewBox="viewBox" :style="svgStyle" @contextmenu.prevent>
        <g v-for="loc in locations" @click="explore(loc)" @contextmenu="flag(loc)">
          <rect :class="'cell ' + classFor(loc)" :x="loc.x + 0.05" :y="loc.y + 0.05" width="0.9" height="0.9"></rect>
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
    <div class="card-footer">
      <div class="dropdown">
        <button class="btn btn-dark dropdown-toggle" type="button" data-bs-toggle="dropdown" aria-expanded="false">
          <i class="bi bi-gear-fill"></i>
        </button>
        <ul class="dropdown-menu">
          <li><a class="dropdown-item" @click="setDifficulty('easy')">Easy</a></li>
          <li><a class="dropdown-item" @click="setDifficulty('medium')">Medium</a></li>
          <li><a class="dropdown-item" @click="setDifficulty('hard')">Hard</a></li>
        </ul>
      </div>
    </div>
  </figure>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import { getWasm } from '@/composables/wasm.ts';

import { useTimer } from "@/composables/useTimer"
const timer = useTimer();

const difficulties = {
  easy: { width: 10, height: 8, mines: 10 },
  medium: { width: 18, height: 14, mines: 40 },
  hard: { width: 24, height: 20, mines: 99 }
}

let board = null;
const gameover = ref(false);
const hasWon = ref(false);
const difficulty = ref("easy")
const tick = ref(0);

const config = computed(() => difficulties[difficulty.value])
const width = computed(() => config.value.width)
const height = computed(() => config.value.height)

const cellSize = 32
const viewBox = computed(() => `0 0 ${width.value} ${height.value}`)
const svgWidth = computed(() => width.value * cellSize)
const svgHeight = computed(() => height.value * cellSize)
const svgStyle = computed(() => ({
  width: `${svgWidth.value}px`,
  height: `${svgHeight.value}px`
}))

onMounted(async () => {
  const wasm = await getWasm();
  board = new wasm.Minesweeper();
  tick.value++;
});

const locations = computed(() => {
  const arr = [];
  const w = width.value;
  const h = height.value;
  for (let y = 0; y < h; y++) {
    for (let x = 0; x < w; x++) {
      arr.push({ x, y });
    }
  }
  return arr;
});

const getStatus = computed(() =>
  gameover.value ? (hasWon.value ? '😎' : '😵') : '🙂'
);

function reset() {
  if (!board) return;
  board.reset();
  timer.reset();
  gameover.value = false;
  hasWon.value = false;
  tick.value++;
}

function setDifficulty(level) {
  difficulty.value = level;
  this.board.setConfig(config.value);
  reset();
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
  timer.start();

  board.explore(location);
  if (board.isMine(location)) {
    gameover.value = true;
    timer.stop();
  } else if (board.checkWin()) {
    hasWon.value = true;
    gameover.value = true;
    timer.stop();
  }
  tick.value++;
}

function getMineCount(location) {
  tick.value;
  return board?.getMineCount(location) || '';
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
  const checkered = (location.x + location.y) % 2 === 0 ? 'dark' : 'light';
  const explored = board?.isExplored(location) ? 'explored' : '';
  return `${explored} ${checkered}`.trim();
}

function getNumberColor(count) {
  const colorMap = {
    1: '#0000FF',
    2: '#008000',
    3: '#FF0000',
    4: '#000080',
    5: '#800000',
    6: '#008080',
    7: '#000000',
    8: '#808080',
  };
  return colorMap[count] || '#000000';
}
</script>

<style scoped>
.cell {
  stroke: #ffffff;
  stroke-width: 0.02px;
  transition: fill 0.2s ease;
  border-radius: var(--bs-border-radius-lg);
  rx: 0.15;
  ry: 0.15;
  width: "0.9";
  height: "0.9";
}

.light {
  fill: #a7d948;
}

.dark {
  fill: #8ecc39;
}

.light.explored {
  fill: #e5c29f;
}

.dark.explored {
  fill: #d7b899;
}

.cell:hover:not(.explored) {
  fill: #b8e663;
  cursor: pointer;
}
</style>
