#!/bin/sh
clang -O3 -flto --target=wasm32 -nostdlib -Wl,--allow-undefined-file=wasm/wasm.syms -Wl,--lto-O3 -Wl,--no-entry -Wl,--export-all -o main.wasm main.c

# switch for bigger stack size
#-Wl,-z, stack-size=$[8 * 1024 * 1024]
