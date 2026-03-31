#!/usr/bin/env bash
# Simple Homebrew helpers for this project (like a tiny "python -m pip" style UX).
# Requires Homebrew. Stable install uses the tagged tarball in Formula/pseudocode.rb.
set -euo pipefail

TAP="mravariya/pseudocode"
TAP_URL="https://github.com/mravariya/pseudocode"
FULL_FORMULA="${TAP}/pseudocode"

usage() {
  cat <<'EOF'
Usage: brew-pseudocode.sh <command>

  install       Tap repo + brew install (stable tarball; default)
  install-head  Tap + brew install --HEAD (latest main)
  uninstall     brew uninstall + brew untap (ignore errors)
  reinstall     Tap + brew reinstall (stable)
  upgrade       Tap + brew upgrade (if installed from tap)
  run           Run pseudocode with remaining args (must be on PATH)
  pkg           Run pkg with remaining args
  help          This message

Examples:
  ./scripts/brew-pseudocode.sh install
  ./scripts/brew-pseudocode.sh run version
  ./scripts/brew-pseudocode.sh run examples/hello.pseudo
  ./scripts/brew-pseudocode.sh pkg available

After homebrew-core merge, plain:  brew install pseudocode
EOF
}

ensure_tap() {
  brew tap "$TAP" "$TAP_URL"
}

cmd="${1:-help}"
shift || true

case "$cmd" in
  install)
    ensure_tap
    brew install "$FULL_FORMULA"
    ;;
  install-head)
    ensure_tap
    brew install --HEAD "$FULL_FORMULA"
    ;;
  uninstall)
    brew uninstall "$FULL_FORMULA" 2>/dev/null || true
    brew untap "$TAP" 2>/dev/null || true
    echo "Removed tap formula (if it was installed). Core formula: brew uninstall pseudocode"
    ;;
  reinstall)
    ensure_tap
    brew reinstall "$FULL_FORMULA"
    ;;
  upgrade)
    ensure_tap
    brew update
    brew upgrade "$FULL_FORMULA" || brew reinstall "$FULL_FORMULA"
    ;;
  run)
    exec pseudocode "$@"
    ;;
  pkg)
    exec pkg "$@"
    ;;
  help|-h|--help)
    usage
    ;;
  *)
    echo "Unknown command: $cmd" >&2
    usage >&2
    exit 1
    ;;
esac
