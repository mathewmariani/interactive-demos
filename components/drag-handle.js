import { defineComponent } from 'https://cdn.jsdelivr.net/npm/vue@3/dist/vue.esm-browser.js';

function eventToSvgCoordinates(event) {
  const svg = event.currentTarget.ownerSVGElement;
  let p = svg.createSVGPoint();
  p.x = event.clientX;
  p.y = event.clientY;
  p = p.matrixTransform(svg.getScreenCTM().inverse());
  return p;
}

export default defineComponent({
  name: 'DragHandle',
  props: ['modelValue', 'color', 'size'],
  template: `<circle
              :fill="color" :r="size" :cx="modelValue.x + 0.5" :cy="modelValue.y + 0.5"
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
    pointerUp() {
      this.dragging = false;
    },
    pointerMove(event) {
      if (!this.dragging) return;
      let { x, y } = eventToSvgCoordinates(event);
      this.$emit('update:modelValue', { x, y });
    },
  },
});
