// @ts-check
import { defineConfig } from 'astro/config';
import { symlinkIntegration } from './src/integrations/symlink';

import vue from '@astrojs/vue';

// https://astro.build/config
export default defineConfig({
  integrations: [symlinkIntegration(), vue()],
  site: 'https://www.mathewmariani.com/',
  base: '/interactive-demos/',
  vite: {
    resolve: {
      preserveSymlinks: true,
    },
  },
});