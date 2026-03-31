#!/usr/bin/env bash
# Maintainer: set Formula sha256 to match the GitHub archive for an existing tag.
# Usage (from repo root; tag must exist on GitHub):
#   ./scripts/homebrew-sync-tag-sha.sh v1.0.0
set -euo pipefail

TAG="${1:?tag e.g. v1.0.0}"
REPO_SLUG="${GITHUB_REPOSITORY:-mravariya/pseudocode}"
URL="https://github.com/${REPO_SLUG}/archive/refs/tags/${TAG}.tar.gz"
FORMULA="Formula/pseudocode.rb"

echo "==> Fetching $URL"
SHA=$(curl -sL "$URL" | shasum -a 256 | awk '{print $1}')
echo "sha256: $SHA"

export SHA FORMULA
python3 <<'PY'
import os, pathlib, re
p = pathlib.Path(os.environ["FORMULA"])
sha = os.environ["SHA"]
text = p.read_text(encoding="utf-8")
text, n = re.subn(r'sha256 "[^"]*"', f'sha256 "{sha}"', text, count=1)
if n != 1:
    raise SystemExit("Could not find single sha256 line in Formula")
p.write_text(text, encoding="utf-8")
PY

echo "==> Updated $FORMULA"
grep -n "sha256" "$FORMULA"
