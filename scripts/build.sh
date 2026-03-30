#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
mkdir -p "$ROOT/build"
cd "$ROOT"
if command -v cmake >/dev/null 2>&1; then
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build
else
  echo "cmake not found; compiling with cc"
  cc -std=c11 -Wall -Wextra -O2 -o build/pseudocode \
    src/main.c src/pc_common.c src/pc_token.c src/pc_lexer.c src/pc_error.c \
    src/pc_ast.c src/pc_parser.c src/pc_value.c src/pc_interp.c src/pc_stdlib.c \
    src/pc_pkg_shared.c src/pc_repl.c src/pc_welcome.c -lm
  cc -std=c11 -Wall -Wextra -O2 -DPC_BUILD_PKG=1 -o build/pkg \
    src/pkg_main.c src/pc_common.c src/pc_token.c src/pc_lexer.c src/pc_error.c \
    src/pc_ast.c src/pc_parser.c src/pc_value.c src/pc_interp.c src/pc_stdlib.c \
    src/pc_pkg_shared.c -lm
fi
echo "Built: build/pseudocode and build/pkg"
