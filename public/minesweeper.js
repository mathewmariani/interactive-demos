'use strict'

import { createApp, reactive, shallowReactive } from 'https://cdn.jsdelivr.net/npm/vue@3/dist/vue.esm-browser.js'
import Module from './modules/demos.js'

(async () => {
  const wasmModule = await Module()

  let gridWorld = new wasmModule.GridWorld(10, 9)
  let board = new wasmModule.Minesweeper()

  const app = createApp({
    data: () => ({
      gridWorld: shallowReactive(gridWorld),
      board: shallowReactive(board),
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
    },
    methods: {
      reset() {
        this.board.reset()
        this.$forceUpdate()
      },
    },
  })

  app.provide("demos", wasmModule)
  app.mount(document.querySelector("#diagram1"))
})()