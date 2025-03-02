'use strict'

import { createApp, reactive, shallowReactive } from 'https://cdn.jsdelivr.net/npm/vue@3/dist/vue.esm-browser.js'
import Module from './modules/demos.js'

(async () => {
  const wasmModule = await Module()

  let gridWorld = new wasmModule.GridWorld(3, 3)
  let board = new wasmModule.TicTacToe()
  let bot = new wasmModule.Bot(board, 2)

  const app = createApp({
    data: () => ({
      gridWorld: shallowReactive(gridWorld),
      board: shallowReactive(board),
      bot: shallowReactive(bot),
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
      get(location) {
        return this.board.get(location.x, location.y)
      },
      set(location) {
        let who = this.board.has_winner();
        if (who != 0 || this.board.is_complete()) {
          return;
        }

        let where = this.board.get(location.x, location.y)
        if (where != 0) {
          return;
        }

        this.board.set(location.x, location.y, 1)
        this.bot.best_move()
        this.$forceUpdate()
      },
      reset() {
        this.board.reset()
        this.$forceUpdate()
      },

    },
  })

  app.provide("demos", wasmModule)
  app.mount(document.querySelector("#diagram1"))
})()