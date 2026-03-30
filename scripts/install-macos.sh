#!/usr/bin/env bash
# Install pseudocode and pkg into /usr/local/bin (requires sudo) or $HOME/.local/bin
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
"$ROOT/scripts/build.sh"
PREFIX="${PREFIX:-$HOME/.local}"
BINDIR="$PREFIX/bin"
mkdir -p "$BINDIR"
cp "$ROOT/build/pseudocode" "$BINDIR/"
cp "$ROOT/build/pkg" "$BINDIR/"
echo "Installed to $BINDIR — ensure this directory is on your PATH."
