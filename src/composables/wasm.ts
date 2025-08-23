let _wasm: any;

// IMPORTANT: import the glue from ONE canonical path only.
import wasmFactory from '@/modules/demos';

export async function getWasm() {
  if (!_wasm) _wasm = await wasmFactory();
  return _wasm;
}

// Optional: keep it across HMR so the glue doesn't re-run
if (import.meta.hot) {
  // stash on window between reloads
  // @ts-ignore
  if (window.__wasmInstance) _wasm = window.__wasmInstance;
  else if (_wasm) (window as any).__wasmInstance = _wasm;
}
