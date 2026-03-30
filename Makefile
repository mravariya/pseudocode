# Convenience targets for local development (requires bash, same as scripts/build.sh).
.PHONY: dev build site help

help:
	@echo "Targets:"
	@echo "  make dev   — rebuild C tools + smoke check (same as ./scripts/dev.sh)"
	@echo "  make build — rebuild only (same as ./scripts/build.sh)"
	@echo "  make site  — rebuild site/public (requires: pip install -r site/requirements-site.txt)"

dev:
	@chmod +x scripts/dev.sh scripts/build.sh 2>/dev/null || true
	@./scripts/dev.sh

build:
	@chmod +x scripts/build.sh 2>/dev/null || true
	@./scripts/build.sh

site:
	@python3 site/build.py
