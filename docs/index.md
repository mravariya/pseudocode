# The Pseudocode documentation

Welcome to the documentation for the **Pseudocode** language and the **pseudocode** reference interpreter. This layout is inspired by the structure of major language manuals: a **hands-on tutorial**, a **language reference**, a **library (built-ins) reference**, and **guides** for tools and workflows.

If you are new here, start with [**Tutorial: Pseudocode in 30 minutes**](tutorial/index.md), then skim the [**Language reference**](reference/language.md) when you need exact rules.

**Web mirror:** the same manual is published as a static site (search-friendly pages, sitemap) from this repository — see the project README under *GitHub Pages* for the live URL once Pages is enabled.

---

## Suggested learning order

1. [Installation](installation.md) — build or install the `pseudocode` binary.  
2. [Getting started](getting-started.md) — `run`, `check`, `repl` in one screen.  
3. [Tutorial](tutorial/index.md) — guided tour of the language.  
4. [Language reference](reference/language.md) — authoritative syntax and semantics.  
5. [Built-in functions](reference/builtins.md) and [Standard library topics](stdlib/index.md) — library calls.  
6. [FAQ](faq.md) — exams, Python comparison, pitfalls.

---

## First steps

| Document | What you will learn |
|----------|---------------------|
| [Installation](installation.md) | Supported platforms, compilers, CMake vs plain `cc`, PATH, troubleshooting |
| [Tutorial](tutorial/index.md) | Variables, control flow, procedures, arrays, files, running programs |
| [Getting started (quick)](getting-started.md) | Build, run, check, REPL in one page |

---

## Language

| Document | Contents |
|----------|----------|
| [Language reference](reference/language.md) | Lexical structure, types, declarations, expressions (precedence), every statement form, subprograms, files, alignment with Cambridge 9618 |
| [Strings and concatenation](reference/strings-and-concatenation.md) | `&`, `+`, comma on assignment-style RHS, `OUTPUT` vs concat, pitfalls, exam notes |
| [Arrays (1D and 2D)](reference/arrays.md) | Declaration, indexing, bounds, whole-array copy, patterns, limits |
| [OOP and user-defined types](reference/oop-and-types.md) | Classes/RECORD/pointers: syllabus vs interpreter; workarounds |
| [Built-in functions](reference/builtins.md) | Every pre-defined function: signatures, arguments, return types, errors, examples |
| [Standard library (Python-style topics)](stdlib/index.md) | `math`, `random`, `time`, `os`, `system` builtins and Python comparison tables |
| [Syntax quick reference](syntax-reference.md) | One-page cheat sheet (links to the full reference) |

---

## Tools and ecosystem

| Document | Contents |
|----------|----------|
| [Command-line interface](cli.md) | `pseudocode` subcommands, exit codes, environment variables, batch use |
| [Package manager (`pkg`)](package-manager.md) | Local packages, layout under `~/.pseudocode`, registry design |
| [Visual Studio Code](vscode.md) | Extension, tasks, snippets, recommended workflow |
| [Error handling](error-handling.md) | Parse vs runtime errors, trace mode, messages, teaching notes |
| [Exit status & PC codes](error-codes.md) | `$?` values, `[PC-NNN]` list, file size limit |

---

## More

| Document | Contents |
|----------|----------|
| [Examples catalogue](examples.md) | Annotated samples and patterns |
| [Glossary](glossary.md) | Terms used in the manual and in 9618 papers |
| [FAQ](faq.md) | Common questions (exam alignment, differences from Python, etc.) |
| [Implementation notes](implementation.md) | How the interpreter is structured (read the source with context) |

---

## Resources outside this tree

- **Source code:** `src/` (lexer, parser, AST, interpreter, standard library).
- **Registry schema (future online packages):** [../registry/registry-schema.md](../registry/registry-schema.md).
- **License:** [../LICENSE](../LICENSE).
- **Authors:** [../AUTHORS.md](../AUTHORS.md).

---

## Conventions in this manual

- **Syntax** blocks use `text` or pseudocode; **placeholders** appear as `*italic*` or `⟨name⟩` in prose.
- **Keywords** are written in capitals where that matches exam style; the interpreter accepts them **case-insensitively**.
- **See also** links connect related sections (similar to cross-references on [docs.python.org](https://docs.python.org/3/) or the [Julia manual](https://docs.julialang.org/)).

---

*Documentation version: 1.0 — matches interpreter v1.0.x.*
