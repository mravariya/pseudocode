#!/usr/bin/env bash
# Check (without logging in) which workflows exist on GitHub and whether the live
# site looks like Jekyll/README or the custom site from site/build.py.
#
# Usage:
#   ./scripts/check-pages-remote.sh
#   GITHUB_REPOSITORY=owner/repo ./scripts/check-pages-remote.sh
set -euo pipefail

OWNER_REPO="${GITHUB_REPOSITORY:-mravariya/pseudocode}"
OWNER="${OWNER_REPO%%/*}"
NAME="${OWNER_REPO#*/}"
API="https://api.github.com/repos/${OWNER_REPO}/contents/.github/workflows"
SITE_URL="https://${OWNER}.github.io/${NAME}/"

echo "Repository: ${OWNER_REPO}"
echo "Pages URL:  ${SITE_URL}"
echo ""
echo "=== Workflow files on default branch (public API) ==="
if ! out=$(curl -fsS "$API"); then
  echo "  (could not fetch — check repo name or network)"
  exit 1
fi
echo "$out" | python3 -c "
import json, sys
d = json.load(sys.stdin)
if isinstance(d, list):
    for x in sorted(d, key=lambda z: z['name']):
        print(' ', x['name'])
else:
    print(' ', d.get('message', d))
"
echo ""
echo "=== Live home page ==="
html=$(curl -fsS "$SITE_URL" || true)
if echo "$html" | grep -q 'name="generator" content="Jekyll'; then
  echo "  PROBLEM: GitHub is serving Jekyll (README/theme), NOT the Python-built site."
  echo ""
  echo "  This is NOT fixed by deleting workflow files — the API only shows:"
  echo "    build.yml, pages.yml"
  echo "  The usual cause is Pages still using \"Deploy from a branch\" in Settings."
  echo ""
  echo "  Fix (you must be logged in as a repo admin):"
  echo "    https://github.com/${OWNER_REPO}/settings/pages"
  echo "  Under \"Build and deployment\" → Source → choose \"GitHub Actions\"."
  echo "  Remove any \"Deploy from a branch\" (main / root) if it is still selected."
  exit 1
elif echo "$html" | grep -q 'lang="en-GB"' && echo "$html" | grep -q 'page-landing'; then
  echo "  OK: Looks like the custom landing page (en-GB + page-landing)."
  exit 0
else
  echo "  Unsure — open the URL and View Page Source:"
  echo "    - Bad: <meta name=\"generator\" content=\"Jekyll"
  echo "    - Good: lang=\"en-GB\" and class=\"page-landing\""
  exit 2
fi
