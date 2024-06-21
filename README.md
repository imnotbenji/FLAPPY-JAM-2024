# FLAPPY-JAM-2024

> my submission to Flappy Jam 2024, play the game here: https://notabenji.itch.io/missing-mailwing

## setup

make a `.env` file based on `.env.template` to configure your build target. e.g.

```
TARGET  = Windows
CONFIG  = Release
PROJECT = FlappyJam2024

CC  = gcc
CXX = g++
```

for web builds, use [Emscripten](https://emscripten.org/) (emcc and em++).
