<template>
  <figure id="diagram1">
    <button type="button" class="btn btn-primary mb-4" @click="reset">
      Reset
    </button>
    <main class="d-flex justify-content-center">
      <svg :viewBox="`0 0 ${getWidth} ${getHeight}`">
        <g v-for="loc in locations" @click="set(loc)">
          <rect class="cell" :x="loc.x" :y="loc.y" width="1" height="1"></rect>

          <circle
            v-if="get(loc) === 2"
            :cx="loc.x + 0.5"
            :cy="loc.y + 0.5"
            r="0.4"
            fill="none"
            stroke="rgb(229, 74, 58)"
            stroke-width="0.1"
          ></circle>

          <g
            v-if="get(loc) === 1"
            stroke="rgb(51, 151, 217)"
            stroke-width="0.1"
          >
            <line
              :x1="loc.x + 0.2"
              :y1="loc.y + 0.2"
              :x2="loc.x + 0.8"
              :y2="loc.y + 0.8"
            ></line>
            <line
              :x1="loc.x + 0.2"
              :y1="loc.y + 0.8"
              :x2="loc.x + 0.8"
              :y2="loc.y + 0.2"
            ></line>
          </g>
        </g>
      </svg>
    </main>
  </figure>
</template>

<script setup>
  import { shallowReactive, ref, computed, onMounted } from 'vue';
  import { getWasm } from '@/composables/wasm.ts';

  const state = shallowReactive({
    tictactoe: null,
    bot: null,
  });

  const tick = ref(0);

  onMounted(async () => {
    const wasm = await getWasm();
    state.tictactoe = shallowReactive(new wasm.TicTacToe());
    state.bot = new wasm.Bot(state.tictactoe, 2);
    tick.value++;
  });

  const getWidth = computed(() => 3);
  const getHeight = computed(() => 3);

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

  function get(loc) {
    tick.value;
    return state.tictactoe?.get(loc.x, loc.y) ?? 0;
  }

  function set(loc) {
    if (!state.tictactoe) return;

    let who = state.tictactoe.has_winner();
    if (who !== 0 || state.tictactoe.is_complete()) return;

    let where = state.tictactoe.get(loc.x, loc.y);
    if (where !== 0) return;

    state.tictactoe.set(loc.x, loc.y, 1);
    state.bot.best_move();

    tick.value++;
  }

  function reset() {
    if (!state.tictactoe) return;

    state.tictactoe.reset();
    tick.value++;
  }
</script>

<style scoped>
  .cell {
    fill: hsl(60, 10%, 90%);
    stroke: hsl(60, 0%, 100%);
    stroke-width: 0.02px;
  }

  svg {
    max-width: 512px;
    max-height: 512px;
    width: 100%;
    height: auto;
  }
</style>
