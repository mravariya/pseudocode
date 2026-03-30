#!/usr/bin/env bash
# Rebuild pseudocode + pkg after editing C sources, then run a quick smoke check.
# From repo root:  ./scripts/dev.sh
# With docs site:   ./scripts/dev.sh --site
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

WITH_SITE=false
for arg in "$@"; do
  case "$arg" in
    --site|-s) WITH_SITE=true ;;
    -h|--help)
      echo "Usage: ./scripts/dev.sh [--site|-s]"
      echo "  Rebuild build/pseudocode and build/pkg, then run hello.pseudo check."
      echo "  --site   also run: pip install -r site/requirements-site.txt && python3 site/build.py"
      exit 0
      ;;
  esac
done

echo "==> dev: rebuild C binaries"
bash "$ROOT/scripts/build.sh"

echo "==> dev: smoke (version + check examples/hello.pseudo)"
"$ROOT/build/pseudocode" version
"$ROOT/build/pseudocode" check "$ROOT/examples/hello.pseudo"
"$ROOT/build/pkg" version >/dev/null

if $WITH_SITE; then
  echo "==> dev: rebuild static site (site/public)"
  if ! command -v python3 >/dev/null 2>&1; then
    echo "dev: python3 not found; skip site (install Python 3 or run without --site)" >&2
    exit 1
  fi
  python3 -m pip install -q -r site/requirements-site.txt
  python3 site/build.py
fi

echo ""
echo "OK — run from repo:  ./build/pseudocode run examples/hello.pseudo"
echo "If you use an install script (e.g. scripts/install-macos.sh), run it again to refresh copies on PATH."
