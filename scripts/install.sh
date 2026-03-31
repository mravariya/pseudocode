#!/usr/bin/env bash
# Install pseudocode + pkg onto your PATH (macOS and Linux).
# Default: $HOME/.local/bin  (override with PREFIX=/path)
# Delegates to install-macos.sh (same script; name kept for compatibility).
set -euo pipefail
cd "$(dirname "$0")"
exec ./install-macos.sh
