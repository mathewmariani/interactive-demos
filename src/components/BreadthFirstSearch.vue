<template>
  <figure id="diagram1">
    <main class="d-flex justify-content-center">
      <svg :view-box.camel="`0 0 ${getWidth} ${getHeight}`">
        <rect
          v-for="loc in locations"
          :class="'cell ' + classFor(loc)"
          :x="loc.x"
          :y="loc.y"
          :width="1"
          :height="1"
          @click="toggleWall(loc)"></rect>
        <!-- <drag-handle v-model="centerPosition" color="yellow" :size="0.5"
        ></drag-handle>
        <drag-handle v-model="goalPosition" color="red" :size="0.5"></drag-handle> -->
      </svg>
    </main>
    <p>
        Time: <input
          v-model.number="stepLimit"
          type="range"
          min="0"
          :max="getWidth * getHeight"
        />
      </p>
	</figure>
</template>

<script>
  function clamp(value, lo, hi) {
    return value < lo ? lo : value > hi ? hi : value;
  }

  import { reactive, shallowReactive } from 'vue';
  import Module from '../modules/demos.js'
  const wasmModule = await Module()

  let gridWorld = new wasmModule.GridWorld(23, 9)
  let startId = new wasmModule.GridLocation(5, 2)

  export default {
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
      goalToGrid() {
        return new wasmModule.GridLocation(this.center.x, this.center.y)
      },
      bfsResults() {
        const ref = wasmModule.BreadthFirstSearch(this.gridWorld, this.goalToGrid, this.stepLimit)
        return { frontier: Array.from({ length: ref.first.size() }, (_, i) => ref.first.get(i)), came_frome: ref.second }
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
    }
  };
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
