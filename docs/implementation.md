# Implementation notes

This chapter is aimed at **contributors**, **teachers** who explain how interpreters work, and **students** doing A Level projects. It mirrors the role of sections such as *Python interpreter* internals or *Julia* lowering documentation at a **high level** only—the authoritative specification of behaviour remains the [Language reference](reference/language.md) plus the source code.

---

## Architecture overview

The **pseudocode** executable is a single process with no JIT:

1. **Read** the source file into memory (`main.c`).
2. **Tokenise** (`pc_lexer.c`) — UTF-8 aware for `←`, string escapes, date-like numeric tokens.
3. **Parse** (`pc_parser.c`) — recursive descent producing an **AST** (`pc_ast.c`).
4. **Execute** (`pc_interp.c`) — tree walk with **environments** (symbol tables) and **call frames** for procedures/functions.
5. **Built-ins** (`pc_stdlib.c` + small hooks in the interpreter) handle library calls.

There is **no bytecode** and **no separate optimiser** in v1.0.

---

## Daily development workflow

After editing files in **`src/`**, from the repository root run:

```bash
./scripts/dev.sh
```

This invokes **`scripts/build.sh`** and a small smoke test (`version` + `check` on `examples/hello.pseudo`). Equivalent: **`make dev`** using the root **`Makefile`**. Use **`./scripts/dev.sh --site`** to regenerate **`site/public`** as well. See [Contributing](../CONTRIBUTING.md) and [Getting started — Development rebuild loop](getting-started.md#development-rebuild-loop).

---

## Major source files

| File | Responsibility |
|------|----------------|
| `pc_token.h` / `pc_token.c` | Token kind enum and debug names |
| `pc_lexer.h` / `pc_lexer.c` | Scanning, keywords, literals, save/restore for `CASE` parsing |
| `pc_parser.h` / `pc_parser.c` | Statements, expressions, precedence |
| `pc_ast.h` / `pc_ast.c` | Node kinds, allocation, recursive free |
| `pc_value.h` / `pc_value.c` | Runtime tagged values (`PcValue`), `PcType`, copy/destroy |
| `pc_interp.c` | Evaluation, I/O, files, `CALL`, `RETURN`, control flow |
| `pc_stdlib.c` | Pure built-in functions |
| `pc_error.c` | Diagnostics and optional ANSI colours |
| `pc_repl.c` | Interactive loop (optional GNU readline) |
| `main.c` | CLI routing |
| `pkg_main.c` / `pc_pkg_shared.c` | Package manager |

---

## Data model

- **Scalars** live inside `PcValue`: small struct with `type` pointer and union payload (`i`, `r`, `s`, `b`, `c`, …).
- **Arrays** allocate a flat `cells[]` of `PcValue`; 2D layout is row-major with bounds stored on `PcType`.
- **Strings** are C strings (`char *`) with an `owned_str` flag for freeing.

---

## Name resolution

- **`sym_find`** walks the current environment chain (local → … → global).
- **`sym_find_local`** is used for redeclaration checks on `DECLARE` / `CONSTANT`.
- **BYREF** parameters store an `alias_of` pointer to the caller’s symbol instead of copying a value.

---

## Procedures and functions

- Top-level `PROCEDURE` / `FUNCTION` nodes are registered in linked lists (`NamedAst`) before execution begins, so **forward calls** are allowed.
- User **functions** run in a fresh local environment parented to **global** (not full lexical closures over nested procedures in v1.0).

---

## Files

Open files are tracked in a small dynamic array keyed by **path string**. `READFILE` / `WRITEFILE` look up the `FILE*` by exact string match after `OPENFILE`.

---

## Extensibility hooks

Reasonable next steps:

- `INCLUDE "path"` preprocessing or AST merge ( **`IMPORT numpy AS np`** for built-in library namespaces is implemented).
- `TYPE` / `RECORD` / field access in the parser and `PcValue`.
- Bytecode or AST optimisation for large batch runs.
- Proper **line maps** for Pseudocode-level stack traces.

Patches should update [Built-in functions](reference/builtins.md) and [Language reference](reference/language.md) when user-visible behaviour changes.

---

## Testing

The repository includes sample programs under **`examples/`** — see **[`examples/README.md`](../examples/README.md)** for **`vanilla-pseudocode/`**, **`stdlib-demos/`**, and root smoke tests. CI (`.github/workflows/build.yml`) compiles on **Ubuntu** and **macOS**. Add regression tests as small `.pseudo` files plus expected stdout when you introduce a test runner.
