import { ref, computed, onUnmounted } from "vue"

export function useTimer() {
    const startTime = ref<number | null>(null)
    const elapsed = ref(0)

    let interval: number | null = null
    let started: boolean = false

    function start() {
        if (interval) return
        if (started) return

        started = true
        startTime.value = Date.now()

        interval = window.setInterval(() => {
            if (!startTime.value) return
            elapsed.value = Math.floor((Date.now() - startTime.value) / 1000)
        }, 250)
    }

    function stop() {
        if (interval) {
            clearInterval(interval)
            interval = null
        }
    }

    function reset() {
        stop()
        started = false
        elapsed.value = 0
        startTime.value = null
    }

    const display = computed(() => {
        const m = Math.floor(elapsed.value / 60)
        const s = elapsed.value % 60
        return `${m}:${s.toString().padStart(2, "0")}`
    })

    onUnmounted(stop)

    return {
        elapsed,
        display,
        start,
        stop,
        reset
    }
}