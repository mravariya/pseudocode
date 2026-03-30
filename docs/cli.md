# Command-line interface

The **`pseudocode`** program is the primary user-facing tool: it runs programs, starts the REPL, and performs static checks. This page documents **every invocation form**, **exit status**, and **environment variables**, in the spirit of Python’s `python --help` / `man python3` reference material.

**See also:** [Installation](installation.md) · [Error handling](error-handling.md) · [Package manager](package-manager.md) (`pkg` is a **separate** binary)

---

## Synopsis

```text
pseudocode                    # welcome screen (version, quick start, sample snippet)
pseudocode [option | command | file]
pseudocode run FILE
pseudocode example            # run built-in demo (reads one line from stdin for INPUT)
pseudocode check FILE
pseudocode repl
pseudocode version
pseudocode help
```

---

## Invoking a program by path

If the first argument ends with **`.pseudo`** or **`.pseudocode`**, it is treated as a **script path** (same as `run`):

```bash
pseudocode myprogram.pseudo
```

The interpreter:

1. Reads the entire file into memory.  
2. Tokenises and parses it.  
3. On success, executes from the first statement.  

Standard output carries **`OUTPUT`** results; standard error carries **diagnostics** and optional **trace** lines.

---

## Subcommands

### `run FILE`

Explicit run form. Equivalent to `pseudocode FILE` when `FILE` has a recognised extension.

```bash
pseudocode run examples/hello.pseudo
```

### `check FILE`

Parses **`FILE`** and runs a lightweight validation pass. Does **not** execute `INPUT`, file I/O, or user logic. Prints:

```text
check: OK (parsed successfully)
```

on success. Use in **CI** and **pre-commit** hooks to catch syntax errors early.

**Note:** `check` does **not** guarantee absence of **runtime** errors (e.g. division by zero).

### `repl`

Starts the **read–eval** loop. Type statements, then a **blank line** to execute the accumulated buffer. Type **`:quit`** to exit.

Behaviour is documented in [Getting started — REPL](getting-started.md) and the [Tutorial](tutorial/index.md#12-using-the-repl).

### `version` (aliases: `-v`)

Prints the interpreter version string (e.g. **`pseudocode 1.0.0`**), aligned with `PC_VERSION_STRING` in the source.

### `help` (aliases: `-h`, `--help`)

Prints a short usage summary and exits.

---

## Exit status

| Code | Meaning |
|------|---------|
| **0** | Success (run finished without parse/runtime error recorded; `check` passed). |
| **2** | Parse error. |
| **3** | Runtime error. |
| **4** | Host I/O error (e.g. cannot read file, file over size limit). |
| **5** | Usage: missing argument or unknown command / file. |

Messages on stderr include **`[PC-NNN]`** codes and **`[parse]`** / **`[runtime]`** / **`[io]`** categories. Full tables: [**Error codes and exit status**](error-codes.md).

Shell scripts can use:

```bash
pseudocode check "$file" || exit 1
pseudocode run "$file" || exit 1
```

---

## Environment variables

| Variable | Values | Effect |
|----------|--------|--------|
| **`PSEUDO_TRACE`** | unset / `0` / **non-empty** | When enabled, prints a coarse **execution trace** to **stderr** (one line per statement). |
| **`PSEUDO_NO_COLOR`** | unset / **set** | When set, disables **ANSI colour** in error messages (useful when piping stderr to a file). |

Example:

```bash
PSEUDO_TRACE=1 pseudocode program.pseudo
PSEUDO_NO_COLOR=1 pseudocode program.pseudo 2> errors.log
```

---

## Standard streams

| Stream | Content |
|--------|---------|
| **stdin** | Consumed by **`INPUT`** during execution. |
| **stdout** | **`OUTPUT`** writes here; values in one `OUTPUT` are separated by spaces, terminated by newline. |
| **stderr** | Parse/runtime **errors**, **notes**, and **trace** lines. |

---

## Working directory and paths

Relative paths in **`OPENFILE`**, `READFILE`, etc. are resolved against the **process current working directory**, not the script’s directory. Change directory before launch or use **absolute paths** in portable scripts.

---

## Embedding in Make / Ninja / CI

Minimal **Makefile** rule:

```makefile
%.out: %.pseudo
	pseudocode run $< > $@
```

GitHub Actions example:

```yaml
- run: ./scripts/build.sh
- run: ./build/pseudocode check examples/hello.pseudo
- run: ./build/pseudocode run examples/hello.pseudo
```

---

## Related tools

| Tool | Purpose |
|------|---------|
| **`pkg`** | Local package install/list/remove — see [Package manager](package-manager.md). |

There is no `pseudocode -m` or plugin flag in v1.0.
