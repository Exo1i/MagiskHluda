import {defineConfig} from 'vite';

export default defineConfig({
    build: {
        outDir: '../module_template/webroot', minify: 'terser', sourcemap: false,
    }, base: './'
});
