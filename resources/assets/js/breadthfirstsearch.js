'use strict'

import { createApp, defineComponent, shallowReactive } from 'https://cdn.jsdelivr.net/npm/vue@3/dist/vue.esm-browser.js'
import Module from './gpr340.js'

function clamp(value, lo, hi) {
    return value < lo ? lo : value > hi ? hi : value;
}

function eventToSvgCoordinates(event) {
    const svg = event.currentTarget.ownerSVGElement;
    let p = svg.createSVGPoint();
    p.x = event.clientX;
    p.y = event.clientY;
    p = p.matrixTransform(svg.getScreenCTM().inverse());
    return p;
}

const DragHandle = defineComponent({
    name: "DragHandle",
    props: ['modelValue', 'color', 'size'],
    template: `<circle :fill="color" :r="size"
                 :cx="modelValue.x + 0.5" :cy="modelValue.y + 0.5"
                 @touchstart.prevent=""
                 @dragstart.prevent=""
                 @pointerdown.left="pointerDown"
                 @pointerup="pointerUp"
                 @pointercancel="pointerUp"
                 @pointermove="pointerMove" />`,
    data: () => ({ dragging: false }),
    methods: {
        pointerDown(event) {
            this.dragging = true;
            event.currentTarget.setPointerCapture(event.pointerId);
        },
        pointerUp(event) {
            this.dragging = false;
        },
        pointerMove(event) {
            if (!this.dragging) return;
            let { x, y } = eventToSvgCoordinates(event);
            this.$emit('update:modelValue', { x, y });
        },
    },
});


async function main() {
    const wasmModule = await Module()

    let gridWorld = new wasmModule.GridWorld(23, 9)
    let startId = new wasmModule.GridLocation(5, 2)

    createApp({
        components: {
            DragHandle,
        },
        data: () => ({
            gridWorld: shallowReactive(gridWorld),
            startId: shallowReactive(startId),
            stepLimit: 0,

            center: { x: 5, y: 2 },
            goal: { x: 16, y: 2 },
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
                const result = [];
                for (let i = 0; i < keys.size(); i++) {
                    const location = keys.get(i)
                    result.push(location)
                }
                return result
            },
            goalToGrid() {
                return new wasmModule.GridLocation(this.center.x, this.center.y)
            },
            bfsResults() {
                const t = new wasmModule.GridLocation(Math.floor(this.center.x), Math.floor(this.center.y))
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
    }).mount(document.querySelector("#diagram1"))
}

main()