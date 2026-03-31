#!/usr/bin/env bash
# Remove pseudocode + pkg from typical local installs (this project only).
# Does not remove Homebrew Cellar — use: brew uninstall <formula>
set -euo pipefail

echo "==> Removing ~/.local/bin/pseudocode and pkg (if present)"
rm -f "$HOME/.local/bin/pseudocode" "$HOME/.local/bin/pkg"

echo "==> Removing optional system copies (if present; may need sudo for /usr/local)"
rm -f /usr/local/bin/pseudocode /usr/local/bin/pkg 2>/dev/null || true
rm -f /opt/homebrew/bin/pseudocode /opt/homebrew/bin/pkg 2>/dev/null || true

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
echo "==> Removing $ROOT/build/pseudocode and pkg (if present)"
rm -f "$ROOT/build/pseudocode" "$ROOT/build/pkg" \
      "$ROOT/build/Release/pseudocode.exe" "$ROOT/build/Release/pkg.exe" \
      "$ROOT/build/pseudocode.exe" "$ROOT/build/pkg.exe" 2>/dev/null || true

echo "==> Remove pkg user data under ~/.pseudocode? (installed stubs, registry.txt)"
read -r -p "Type YES to delete ~/.pseudocode: " ans
if [ "$ans" = "YES" ]; then
  rm -rf "$HOME/.pseudocode"
  echo "Removed ~/.pseudocode"
else
  echo "Skipped ~/.pseudocode"
fi

echo ""
echo "Done. If you used Homebrew:  brew uninstall mravariya/pseudocode/pseudocode"
echo "Windows: delete %LOCALAPPDATA%\\Pseudocode\\bin\\pseudocode.exe and pkg.exe and remove that folder from PATH."
echo "Rebuild later:  ./scripts/build.sh"
