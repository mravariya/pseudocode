# Getting started

This page is a **short on-ramp**. For a full narrative introduction, work through the [**Tutorial**](tutorial/index.md). For exhaustive rules, use the [**Language reference**](reference/language.md).

---

## Prerequisites

You need a built **`pseudocode`** binary. If you have not installed it yet, follow [**Installation**](installation.md).

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

## Your first program

Create **`hello.pseudo`**:

```text
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
4. **[CLI reference](cli.md)** — every subcommand and environment variable.  
5. **[FAQ](faq.md)** — exams, Python vs Pseudocode, common errors.

---

## Getting help

- Terminal: `pseudocode help`  
- This site: [Error handling](error-handling.md) for diagnostics  
- Project **README** and **AUTHORS** for maintainers and licence  
