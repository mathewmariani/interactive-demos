<template>
    <circle :fill="color" :r="size" :cx="modelValue.x + 0.5" :cy="modelValue.y + 0.5" @touchstart.prevent=""
        @dragstart.prevent="" @pointerdown.left="pointerDown" @pointerup="pointerUp" @pointercancel="pointerUp"
        @pointermove="pointerMove" />
</template>

<script setup>
import { ref } from "vue";

// FIXME: show be a composable
function eventToSvgCoordinates(event) {
    const svg = event.currentTarget.ownerSVGElement;
    let p = svg.createSVGPoint();
    p.x = event.clientX;
    p.y = event.clientY;
    p = p.matrixTransform(svg.getScreenCTM().inverse());
    return p;
}

const props = defineProps({
    modelValue: { type: Object, required: true },
    color: { type: String, default: "black" },
    size: { type: Number, default: 5 },
});

const emit = defineEmits(["update:modelValue"]);
const dragging = ref(false);

function pointerDown(event) {
    dragging.value = true;
    event.currentTarget.setPointerCapture(event.pointerId);
}

function pointerUp() {
    dragging.value = false;
}

function pointerMove(event) {
    if (!dragging.value) return;
    const { x, y } = eventToSvgCoordinates(event);
    emit("update:modelValue", { x, y });
}
</script>