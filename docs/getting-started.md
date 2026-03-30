# Getting started

This page is a **short on-ramp**. For a full narrative introduction, work through the [**Tutorial**](tutorial/index.md). For exhaustive rules, use the [**Language reference**](reference/language.md).

---

## Prerequisites

You need a built **`pseudocode`** binary (and optionally **`pkg`**, built in the same step). If you have not installed them yet, follow [**Installation**](installation.md).

**No install?** Use a browser workspace: [**Try online**](try-online.md) (GitHub Codespaces or Gitpod).

---

## 60-second workflow

```bash
# 1. Build (from repository root; skip if already installed)
./scripts/build.sh

# 2. Run a sample program
./build/pseudocode examples/hello.pseudo

# 3. Parse-only check (no execution)
./build/pseudocode check examples/hello.pseudo

# 4. Interactive mode
./build/pseudocode repl
```

If `pseudocode` is on your `PATH`, replace `./build/pseudocode` with `pseudocode`.

---

## Development rebuild loop

When you **change C source** under **`src/`** and want to test without remembering every step:

```bash
# From repository root (once: chmod +x scripts/dev.sh scripts/build.sh)
./scripts/dev.sh
```

This **rebuilds** `build/pseudocode` and `build/pkg`, then runs **`pseudocode version`**, **`pseudocode check examples/hello.pseudo`**, and verifies **`pkg`**. If you use **Make**: **`make dev`** does the same.

- **Also rebuild the docs website** (`site/public`): `./scripts/dev.sh --site` (needs Python 3 + `pip install -r site/requirements-site.txt`).
- If you **installed** the binaries onto your PATH (e.g. macOS **`scripts/install-macos.sh`**), run that **install script again** after `dev.sh` so the shell finds the new build.

---

## Optional: packages (`pkg`)

The **`pkg`** tool copies optional **`.pseudo`** bundles into **`~/.pseudocode/packages/`** (see [**Package manager**](package-manager.md)).

**Install `pkg`:** same build as the interpreter — you already have **`./build/pkg`** after **`./scripts/build.sh`**.

**Common commands:**

| Goal | Command |
|------|---------|
| Help | `pkg help` or `pkg -h` / `pkg --help` |
| Version | `pkg version` or `pkg -v` / `pkg --version` |
| List catalog names (e.g. `math`, `random`) | `pkg available` |
| Install a catalog bundle | **`pkg install math`** or short **`pkg -i math`** (same as **`pkg --install math`**) |
| Install a folder of `.pseudo` files | `pkg install ./my-lib [optional-name]` |
| See installed packages | `pkg list` |
| Remove | `pkg remove name` |

Full command table (short vs long): [**Package manager — Commands: short and full forms**](package-manager.md#commands-short-and-full-forms).

---

## Your first program

Create **`hello.pseudo`**:

```pseudocode
OUTPUT "Hello, world"
```

Run:

```bash
pseudocode hello.pseudo
```

---

## Key ideas (one glance)

| Topic | Pseudocode | Doc |
|--------|------------|-----|
| Assignment | `x <- 5` or `x ← 5` | [Language §4.1](reference/language.md#41-assignment) |
| Types | `DECLARE n : INTEGER` | [Language §2](reference/language.md#2-types-and-values) |
| Decisions | `IF` … `ENDIF` | [Tutorial §5](tutorial/index.md#5-making-decisions-if) |
| Loops | `FOR` / `WHILE` / `REPEAT` | [Tutorial §7](tutorial/index.md#7-loops) |
| Library | `LENGTH`, `MID`, … | [Built-ins](reference/builtins.md) |

---

## Documentation map

1. **[Documentation home](index.md)** — full table of contents.  
2. **[Tutorial](tutorial/index.md)** — learn by doing.  
3. **[Language reference](reference/language.md)** — complete syntax and semantics.  
4. **[CLI reference](cli.md)** — every `pseudocode` subcommand, flags, and environment variables.  
5. **[Package manager](package-manager.md)** — `pkg` install, **`pkg -i`**, catalog, `list` / `remove`.  
6. **[FAQ](faq.md)** — exams, Python vs Pseudocode, common errors.

---

## Getting help

- Terminal: `pseudocode help` · `pkg help` (and `pkg -h`)  
- This site: [Error handling](error-handling.md) for diagnostics  
- Project **README** and **AUTHORS** for maintainers and licence  
