import { babel } from '@rollup/plugin-babel'
import {nodeResolve} from '@rollup/plugin-node-resolve'
import typescript from '@rollup/plugin-typescript'

export default {
  input: './src/editor.mts',
  output: {
    file: './dist/editor.bundle.js',
    format: 'iife',
    name: 'codeMirrorEditor'
  },
  plugins: [nodeResolve(), typescript(), babel({
    babelHelpers: 'bundled',
    presets: ['@babel/preset-env'],
  })]
}
