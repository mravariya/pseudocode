# Contributing

Thank you for helping improve the **Pseudocode** interpreter or its documentation.

## Ways to contribute

- **Report issues** — unexpected parse errors, wrong runtime behaviour, or unclear docs (open a GitHub issue with a minimal `.pseudo` example when possible).
- **Submit pull requests** — bug fixes, tests, doc corrections, or small, focused features that match Cambridge 9618 style.
- **Documentation** — the manual lives in `docs/` (Markdown). The public website is generated from those files plus `site/content/index.md`; see `site/README.md`.

## Project layout

| Area | Path |
|------|------|
| Interpreter (C) | `src/` |
| Examples | `examples/` |
| Manual | `docs/` |
| VS Code extension | `vscode/pseudocode/` |
| Site generator | `site/build.py` |
| Build script | `scripts/build.sh` |
| Dev rebuild + smoke | `scripts/dev.sh` (or **`make dev`**) |

## Building and running

**After you change C code** in `src/`, rebuild and sanity-check in one step:

```bash
chmod +x scripts/dev.sh scripts/build.sh   # once
./scripts/dev.sh
```

This runs `scripts/build.sh`, then `pseudocode version`, `pseudocode check examples/hello.pseudo`, and `pkg version`. To also regenerate the docs website into `site/public`:

```bash
./scripts/dev.sh --site
```

If you use **GNU Make**, the same workflow is available as **`make dev`** (see the root `Makefile`).

If you previously **installed** binaries to `PATH` (e.g. `scripts/install-macos.sh`), run that install step again after `./scripts/dev.sh` so the global `pseudocode` matches your latest build.

Manual build only (no smoke test):

```bash
./scripts/build.sh
./build/pseudocode run examples/hello.pseudo
./build/pseudocode repl
```

## Style (code)

- Match existing **C** style in `src/` (naming, error handling, no large unrelated refactors).
- Prefer **small commits** with clear messages.
- If you change language behaviour, update the matching section in `docs/reference/language.md` and any affected tutorial text.

## License

By contributing, you agree your contributions are licensed under the same terms as the project (**MIT** — see `LICENSE`).
