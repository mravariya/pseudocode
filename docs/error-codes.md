# Exit status and error codes

The `pseudocode` CLI uses **small integer exit statuses** and prints **`[PC-NNN]`** numeric codes on stderr for parse, runtime, and I/O errors (for scripts, CI, and teaching).

**See also:** [CLI](cli.md) · [Error handling](error-handling.md)

---

## Exit status (`$?` / `%ERRORLEVEL%`)

| Code | Meaning |
|------|---------|
| **0** | Success. |
| **2** | Parse error (lexer/parser). |
| **3** | Runtime error (interpreter or built-ins during execution). |
| **4** | Host I/O error (e.g. cannot read source file, file too large). |
| **5** | Usage: missing argument, unknown subcommand, or unknown first argument. |

The REPL always exits with **0** when you quit normally.

---

## Message format

Errors look like:

```text
error [PC-101] [parse] file.pseudo:4:12: expected ...
error [PC-220] [runtime] file.pseudo:10:1: division by zero
error [PC-301] [io]: cannot read file ...
```

- **`[PC-NNN]`** — stable numeric code (see `src/pc_error_codes.h`).
- **`[parse]` / `[runtime]` / `[io]`** — category.
- **File:line:column** — present when the front end has location information.

ANSI colours apply on a TTY unless `PSEUDO_NO_COLOR=1`.

---

## Selected `PC-NNN` codes

| Range | Area |
|-------|------|
| 101–110 | Parse (expect token, syntax, EOF). |
| 201–230 | Runtime (undefined name, index, call, types, division by zero, OOM). |
| 251–253 | Built-in function argument/conversion/range. |
| 301 | Host file I/O (`pkg`, CLI read). |

The full list is in **`src/pc_error_codes.h`**.

---

## Limits

- Source files read by the CLI are capped at **`PC_MAX_SOURCE_BYTES`** (32 MiB by default; see `src/pc_common.h`) to avoid accidental huge allocations.
