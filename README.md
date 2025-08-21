# Spin the wheel

<!-- markdownlint-disable MD033 MD013 -->

<img width="1112" height="840" alt=" 2025-08-21 at 08 29 18" src="https://github.com/user-attachments/assets/09fd6aba-1889-43e3-8c92-c8e6b3a5dcc5" />

## Build for Desktop (with cmake)

```bash
cmake -S . -B build && cmake --build build
./build/game/game
```

## Build for WebAssembly

Firstly read [Raylib Docs](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))

Install [emscripten toolchain](https://emscripten.org/docs/getting_started/downloads.html).
If you are on macOS, you can install Emscripten using Homebrew: `brew install emscripten`

Build game with camke

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/leoua7/Apps/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web
make
```

## Features

🔥 **In game terminal**

<img width="2224" height="1680" alt=" 2025-08-21 at 08 31 09 compressed" src="https://github.com/user-attachments/assets/339d0616-f37c-413e-a0e1-86ea0a952ea7" />

<img width="2224" height="1680" alt=" 2025-08-21 at 08 30 27 compressed" src="https://github.com/user-attachments/assets/4c555bc0-db32-4188-b767-88f0d17616b3" />
