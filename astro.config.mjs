// @ts-check
import { defineConfig } from 'astro/config';
import { symlinkIntegration } from './src/integrations/symlink';

// https://astro.build/config
export default defineConfig({
  integrations: [symlinkIntegration()],
  site: 'https://www.mathewmariani.com',
  base: '/interactive-demos/',
  vite: {
    resolve: {
      preserveSymlinks: true,
    },
  },
});