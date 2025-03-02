'use strict'

import { createApp, reactive, shallowReactive } from 'https://cdn.jsdelivr.net/npm/vue@3/dist/vue.esm-browser.js'
import DragHandle from './components/drag-handle.js'
import GridWorld from './components/grid-world.js'
import Module from './modules/gpr340.js'

function clamp(value, lo, hi) {
    return value < lo ? lo : value > hi ? hi : value;
}

(async () => {
    const wasmModule = await Module()

    let gridWorld = new wasmModule.GridWorld(23, 9)
    let startId = new wasmModule.GridLocation(5, 2)

    const app = createApp({
        components: {
            DragHandle,
            GridWorld,
        },
        data: () => ({
            gridWorld: shallowReactive(gridWorld),
            startId: shallowReactive(startId),
            stepLimit: 0,

            center: reactive({ x: 5, y: 2 }),
            goal: reactive({ x: 16, y: 2 }),
            scale: 0.5,
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
            centerPosition: {
                get() {
                    return {
                        x: (this.center.x),// * this.scale,
                        y: (this.center.y),// * this.scale,
                    };
                },
                set({ x, y }) {
                    let q = Math.round(x - 0.5)// / this.scale - 0.5);
                    let r = Math.round(y - 0.5)// / this.scale - 0.5);
                    this.center.x = clamp(q, 0, this.getWidth - 1)
                    this.center.y = clamp(r, 0, this.getHeight - 1)
                },
            },
            goalPosition: {
                get() {
                    return {
                        x: (this.goal.x),// * this.scale,
                        y: (this.goal.y),// * this.scale,
                    };
                },
                set({ x, y }) {
                    let q = Math.round(x - 0.5)// / this.scale - 0.5);
                    let r = Math.round(y - 0.5)// / this.scale - 0.5);
                    this.goal.x = clamp(q, 0, this.getWidth - 1)
                    this.goal.y = clamp(r, 0, this.getHeight - 1)
                },
            },
            goalToGrid() {
                return new wasmModule.GridLocation(this.center.x, this.center.y)
            },
            bfsResults() {
                const ref = wasmModule.BreadthFirstSearch(this.gridWorld, this.goalToGrid, this.stepLimit)
                return { frontier: Array.from({ length: ref.first.size() }, (_, i) => ref.first.get(i)), came_frome: ref.second }
            },
        },
        methods: {
            toggleWall(location) {
                this.gridWorld.toggleWall(location)
                this.$forceUpdate()
            },
            classFor(location) {
                const bfsResult = this.bfsResults;
                return this.gridWorld.isWall(location) ? "wall"
                    : bfsResult.frontier.some(item => (item.x == location.x) && (item.y == location.y)) ? "frontier"
                        : bfsResult.came_frome.get(location) ? "explored"
                            : ""
            },
        },
    })

    app.provide("gpr340", wasmModule)
    app.mount(document.querySelector("#diagram1"))
})()