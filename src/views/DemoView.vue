<template>
    <Suspense>
        <template #default>
            <component :is="matchedComponent" v-if="matchedComponent" />
            <p v-else>404 – Not Found</p>
        </template>

        <template #fallback>
            <p>Loading demo…</p>
        </template>
    </Suspense>
</template>

<script setup lang="ts">
import { computed } from "vue";

const props = defineProps<{ slug: string }>();

// Import your possible views
import Chess from '@/demos/Chess.vue'
import DungeonGenerator from '@/demos/DungeonGenerator.vue'
import JohnConwayLife from '@/demos/JohnConwayLife.vue'
import MazeGenerator from '@/demos/MazeGenerator.vue'
import Minesweeper from '@/demos/Minesweeper.vue'
import Pathfinding from '@/demos/Pathfinding.vue'
import TicTacToe from '@/demos/TicTacToe.vue'

const componentMap: Record<string, any> = {
    "chess": Chess,
    "dungeon-generator": DungeonGenerator,
    "john-conway-life": JohnConwayLife,
    "maze-generator": MazeGenerator,
    "minesweeper": Minesweeper,
    "pathfinding": Pathfinding,
    "tic-tac-toe": TicTacToe,
};

const matchedComponent = computed(() => componentMap[props.slug] || null);
</script>