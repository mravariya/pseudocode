# Pseudocode interpreter

A **Cambridge International** pseudocode interpreter aligned with **IGCSE Computer Science (0478 / CS0478)**, **AS & A Level Computer Science (9618 / CS9618)**, and **AS & A Level Information Technology (9626 / IT9626)** — implemented in **C** for a small, fast, cross-platform runtime. The project ships:

- **`pseudocode`** — run programs (`.pseudo` / `.pseudocode`), **`check`**, **`repl`**, **`version`**, **`help`**
- **`tools/pseudocode_idle.py`** — optional **IDLE-style GUI** (Python 3 + Tkinter): editor, F5 run, F6 check; uses the `pseudocode` binary on `PATH` or `build/pseudocode`
- **`pkg`** — local package layout under `~/.pseudocode` (with an [online registry design](registry/registry-schema.md))
- **VS Code** grammar + snippets ([`vscode/pseudocode`](vscode/pseudocode/))
- **Documentation** structured like major language manuals — start at **[Documentation home → docs/index.md](docs/index.md)**

**Website (GitHub Pages):** the repo includes a Markdown-driven static site in **`site/`** (Python + one dependency, **not** GitHub’s default Jekyll README theme) with a landing page, SEO metadata, sitemap, and colourful code snippets. **Live site:** [mravariya.github.io/pseudocode](https://mravariya.github.io/pseudocode/). If the page still looks like a **plain README**, GitHub is publishing Jekyll instead of `site/public` — fix: **[docs/github-pages.md](docs/github-pages.md)**. Build locally: `pip install -r site/requirements-site.txt && python3 site/build.py` — details in **[site/README.md](site/README.md)**.

**Author:** [Mahesh Ravariya](https://www.linkedin.com/in/mravariya/) · [GitHub](https://github.com/mravariya)  
**License:** [MIT](LICENSE)

---

## Quick start

```bash
./scripts/build.sh
./build/pseudocode examples/hello.pseudo
./build/pseudocode repl
```

**Try without installing locally:** open the repo in **[GitHub Codespaces](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mravariya%2Fpseudocode)** (browser + full Linux build) — see **[docs/try-online.md](docs/try-online.md)**.

**Developing (rebuild after editing `src/`):** from the repo root run **`./scripts/dev.sh`** — it rebuilds **`pseudocode`** and **`pkg`**, then runs a quick check. Same as **`make dev`** if you have Make. Use **`./scripts/dev.sh --site`** to also rebuild the static site. See [Contributing](CONTRIBUTING.md) and [Getting started — dev loop](docs/getting-started.md#development-rebuild-loop).

---

## Documentation (detailed)

The manual is organised similarly to **Python** and **Julia** official docs: a **hub page**, a **tutorial**, a **language reference**, a **library (built-ins) reference**, and **tooling guides**.

| Section | Description |
|---------|-------------|
| [**Documentation home**](docs/index.md) | Full table of contents and reading order |
| [**Installation**](docs/installation.md) | Platforms, CMake, PATH, troubleshooting — **includes [step-by-step macOS & Windows](docs/installation.md#step-by-step-macos)** |
| [**Tutorial**](docs/tutorial/index.md) | Hands-on introduction (~30 minutes) |
| [**Getting started (quick)**](docs/getting-started.md) | Minimal commands and links |
| [**Language reference**](docs/reference/language.md) | Complete syntax and semantics |
| [**Built-in functions**](docs/reference/builtins.md) | Signatures, behaviour, examples |
| [**Syntax cheat sheet**](docs/syntax-reference.md) | One-page summary |
| [**CLI**](docs/cli.md) | Subcommands, exit codes, environment |
| [**Package manager**](docs/package-manager.md) | `pkg` commands and layout |
| [**VS Code**](docs/vscode.md) | Extension, tasks, snippets |
| [**Error handling**](docs/error-handling.md) | Parse vs runtime, trace mode |
| [**Examples**](docs/examples.md) | Catalogue and patterns |
| [**Glossary**](docs/glossary.md) | Terms |
| [**FAQ**](docs/faq.md) | Exams, Python/Julia comparison |
| [**Implementation**](docs/implementation.md) | Architecture for contributors |

---

## Features (language)

- **Types:** `INTEGER`, `REAL`, `STRING`, `CHAR`, `BOOLEAN`, `DATE`
- **Control flow:** `IF`/`ENDIF`, `CASE`/`ENDCASE`, `FOR`/`NEXT`, `WHILE`/`ENDWHILE`, `REPEAT`/`UNTIL`
- **Subprograms:** `PROCEDURE`/`CALL`, `FUNCTION`/`RETURN`, `BYVAL`/`BYREF`
- **Arrays:** 1D and 2D, inclusive bounds
- **Files:** `OPENFILE`, `READFILE`, `WRITEFILE`, `CLOSEFILE`, `EOF`
- **Library:** `LENGTH`, `MID`/`SUBSTRING`, `RIGHT`, `CONCAT`, `INT`/`TOINTEGER`, `TOREAL`, `RAND`/`RANDOM`, `ROUND`, `UCASE`/`UPPERCASE`, `LCASE`/`LOWERCASE`, …

Assignment: **`←`** (UTF-8) or **`<-`**.

---

## Repository layout

| Path | Contents |
|------|----------|
| `src/` | Lexer, parser, AST, interpreter, stdlib, REPL, CLI |
| `examples/` | Sample programs — see [`examples/README.md`](examples/README.md) (`vanilla-pseudocode/`, `stdlib-demos/`, root smoke tests) |
| `docs/` | Manual (start at `docs/index.md`) |
| `vscode/pseudocode/` | Editor extension |
| `registry/` | Package registry **schema** (future HTTP) |
| `scripts/` | `build.sh`, platform installers |

---

## Version

**v1.0.0** — see `PC_VERSION_STRING` in `src/pc_common.h`.
