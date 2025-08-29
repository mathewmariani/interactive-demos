<template>
  <figure id="diagram1">
    <main class="d-flex justify-content-center">
      <svg :view-box.camel="`0 0 ${getWidth} ${getHeight}`">
        <rect v-for="loc in locations" :key="`${loc.x}-${loc.y}`" :class="'cell ' + classFor(loc)" :x="loc.x" :y="loc.y"
          :width="1" :height="1" @click="toggleWall(loc)" />
        <DragHandle v-model="centerPosition" color="yellow" :size="0.5" />
        <!-- <DragHandle v-model="goalPosition" color="red" :size="0.5" /> -->
      </svg>
    </main>
    <div class="container py-3">
      <!-- Controls -->
      <div class="mb-3">
        <!-- Generator selector -->
        <div class="mb-3">
          <label for="generatorSelect" class="form-label">Generator</label>
          <select id="generatorSelect" v-model="ruleIndex" class="form-select">
            <option v-for="(name, idx) in generatorNames" :key="idx" :value="idx">
              {{ name }}
            </option>
          </select>
        </div>

        <!-- Step slider -->
        <div class="mb-3">
          <label for="stepRange" class="form-label">Time: {{ stepLimit }}</label>
          <input id="stepRange" v-model.number="stepLimit" type="range" class="form-range" min="0"
            :max="getWidth * getHeight" />
        </div>
      </div>

    </div>
  </figure>
</template>

<script setup>
import { ref, computed } from "vue";
import Module from "@/modules/demos.js";
import DragHandle from "@/components/vue/DragHandle.vue";

function clamp(value, lo, hi) {
  return value < lo ? lo : value > hi ? hi : value;
}

const wasmModule = await Module();
const gridWorld = new wasmModule.GridWorld(23, 9);

const generators = [];
const generatorNames = ref([]);
const ruleIndex = ref(0);

generators.push(new wasmModule.HuntAndKill());
generators.push(new wasmModule.RandomWalk());
generators.push(new wasmModule.RecursiveBacktracker());

generatorNames.value.push(
  'AStar Search',
  'Breadth First Search',
  'Dijkstra Search',
  'Greedy Search',
);


const stepLimit = ref(0);
const center = ref({ x: 5, y: 2 });
const goal = ref({ x: 16, y: 2 });
const renderKey = ref(0);

const getWidth = computed(() => 23);
const getHeight = computed(() => 9);

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

const goalToGrid = computed(() => ({
  x: center.value.x,
  y: center.value.y,
}));

const bfsResults = computed(() => {
  return wasmModule.BreadthFirstSearch(gridWorld, goalToGrid.value, stepLimit.value);
});

const centerPosition = computed({
  get: () => ({ x: center.value.x, y: center.value.y }),
  set: ({ x, y }) => {
    const q = Math.round(x - 0.5);
    const r = Math.round(y - 0.5);
    center.value.x = clamp(q, 0, getWidth.value - 1);
    center.value.y = clamp(r, 0, getHeight.value - 1);
  },
});

const goalPosition = computed({
  get: () => ({ x: goal.value.x, y: goal.value.y }),
  set: ({ x, y }) => {
    const q = Math.round(x - 0.5);
    const r = Math.round(y - 0.5);
    goal.value.x = clamp(q, 0, getWidth.value - 1);
    goal.value.y = clamp(r, 0, getHeight.value - 1);
  },
});

function toggleWall(location) {
  gridWorld.toggleWall(location);
  renderKey.value++;
}

function classFor(location) {
  renderKey.value;
  if (gridWorld.isWall(location)) return "wall";

  const bfs = bfsResults.value;

  if (bfs.frontier.has(location))
    return "frontier";

  if (bfs.came_from.has(location))
    return "explored";

  return "";
}
</script>

<style scoped>
.cell {
  fill: hsl(60, 10%, 90%);
  stroke: hsl(60, 0%, 100%);
  stroke-width: 0.02px;
}

.wall {
  fill: hsl(30, 20%, 40%);
  stroke: hsl(30, 50%, 40%);
}

.explored {
  fill: hsl(45, 20%, 70%);
}

.frontier {
  fill: hsl(220, 50%, 70%);
}
</style>
