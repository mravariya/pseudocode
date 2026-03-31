#!/usr/bin/env bash
# Install pseudocode and pkg onto PATH (macOS and Linux).
# Runs scripts/build.sh, then copies binaries to $PREFIX/bin (default PREFIX=$HOME/.local → ~/.local/bin).
# System-wide example: sudo env PREFIX=/usr/local ./scripts/install-macos.sh
# Wrapper: ./scripts/install.sh (same behaviour).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
"$ROOT/scripts/build.sh"
PREFIX="${PREFIX:-$HOME/.local}"
BINDIR="$PREFIX/bin"
mkdir -p "$BINDIR"
cp "$ROOT/build/pseudocode" "$BINDIR/"
cp "$ROOT/build/pkg" "$BINDIR/"
echo "Installed to $BINDIR — ensure this directory is on your PATH."
