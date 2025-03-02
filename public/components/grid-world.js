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
  name: "GridWorld",
  props: ['locations'],
  template: `<rect v-for="loc in locations" :class="'cell'"
              :x="loc.x"
              :y="loc.y"
              :width="1"
              :height="1" />`,
  data: () => ({

  }),
  methods: {
  },
});