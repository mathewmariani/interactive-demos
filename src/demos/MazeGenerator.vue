<template>
    <div class="container py-3">
        <!-- Controls -->
        <div class="mb-3">
            <h5>Maze Simulation</h5>
            <div class="btn-group mb-2">
                <button class="btn btn-primary" @click="step">Step</button>
                <button class="btn btn-success" @click="start">Start</button>
                <button class="btn btn-warning" @click="pause">Pause</button>
                <button class="btn btn-danger" @click="clear">Clear</button>
            </div>

            <div class="mb-2">
                <label>Tick Rate: {{ tickRate.toFixed(2) }}s</label>
                <input type="range" min="0.05" max="1" step="0.01" v-model.number="tickRate" class="form-range" />
            </div>

            <div class="mb-2">
                <label>Width: {{ currentSize }}</label>
                <input type="range" min="5" max="49" step="2" v-model.number="currentSize" @change="resize"
                    class="form-range" />
            </div>

            <div class="mb-2">
                <label>Generator</label>
                <select v-model="ruleIndex" class="form-select">
                    <option v-for="(name, idx) in generatorNames" :key="idx" :value="idx">{{ name }}</option>
                </select>
            </div>
        </div>

        <!-- Maze SVG -->
        <figure id="diagram1">
            <svg :viewBox="`0 0 ${getWidth} ${getHeight}`" width="512" height="512">
                <template v-for="loc in locations" :key="`${loc.x},${loc.y}-${refreshKey}`">
                    <rect class="cell" :class="cellClasses[`${loc.x},${loc.y}`]" :x="loc.x" :y="loc.y" :width="1"
                        :height="1" />

                    <line v-if="getNorth(loc)" :x1="loc.x" :y1="loc.y" :x2="loc.x + 1" :y2="loc.y" />
                    <line v-if="getWest(loc)" :x1="loc.x" :y1="loc.y" :x2="loc.x" :y2="loc.y + 1" />
                </template>
            </svg>
        </figure>
    </div>
</template>

<script setup>
import { ref, computed, onMounted } from "vue";
import { getWasm } from "@/composables/wasm.ts";

let wasm = null;
let maze = null;
let running = false;
const generators = [];
const generatorNames = ref([]);
const ruleIndex = ref(0);
const currentSize = ref(20);
const tickRate = ref(0.1);
const refreshKey = ref(0);

// Maze dimensions
let getWidth = ref(currentSize.value);
let getHeight = ref(currentSize.value);

// Generate flattened grid for SVG
const locations = computed(() => {
    const arr = [];
    const w = currentSize.value;
    const h = currentSize.value;
    for (let y = 0; y < h; y++) {
        for (let x = 0; x < w; x++) {
            arr.push({ x, y });
        }
    }
    return arr;
});

const cellClasses = computed(() => {
    refreshKey.value;
    if (!maze || !wasm) return {};
    const map = {};
    for (let loc of locations.value) {
        let node = maze.node(loc);
        switch (node.type) {
            case wasm.NodeType.Unvisited:
                map[`${loc.x},${loc.y}`] = "unvisited";
                break;
            case wasm.NodeType.Frontier:
                map[`${loc.x},${loc.y}`] = "frontier";
                break;
            case wasm.NodeType.Visited:
                map[`${loc.x},${loc.y}`] = "visited";
                break;
            default:
                map[`${loc.x},${loc.y}`] = "unvisited";
        }
    }
    return map;
});

onMounted(async () => {
    wasm = await getWasm();

    maze = new wasm.Maze();
    maze.resize(currentSize.value, currentSize.value);

    generators.push(new wasm.HuntAndKill());
    generators.push(new wasm.RandomWalk());
    generators.push(new wasm.RecursiveBacktracker());

    generatorNames.value.push("HuntAndKill", "RandomWalk", "RecursiveBacktracker");
});

function step() {
    if (!maze) return;
    const gen = generators[ruleIndex.value];
    try {
        gen.step(maze);
        refreshKey.value++;
    } catch (e) {
        console.error("WASM step error:", e);
    }
}

function loop() {
    if (!running) return;
    step();
    setTimeout(loop, tickRate.value * 1000);
}

function start() {
    if (running) return;
    running = true;
    loop();
}

function pause() {
    running = false;
}

function clear() {
    if (!maze) return;
    const gen = generators[ruleIndex.value];
    gen.clear();
    resize();
    running = false;

    refreshKey.value++;
}

function resize() {
    maze.resize(currentSize.value, currentSize.value);
}

function getNorth(loc) {
    if (!maze) return false;
    return maze.getNorth(loc);
}

function getWest(loc) {
    if (!maze) return false;
    return maze.getWest(loc);
}
</script>

<style scoped>
.cell {
    stroke: #ffffff;
    fill: hsl(60, 10%, 90%);
    stroke-width: 0.01px;
    transition: fill 0.2s ease;
    cursor: pointer;
}

.unvisited {
    fill: hsl(30, 20%, 40%);
}

.visited {
    fill: hsl(45, 20%, 70%);
}

.frontier {
    fill: hsl(220, 50%, 70%);
}

line {
    stroke: black;
    stroke-width: 0.1px;
}
</style>