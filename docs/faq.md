# Frequently asked questions

### Is this exactly what Cambridge will mark in the exam?

**No automated tool replaces the mark scheme.** The **Pseudocode Guide for Teachers** is authoritative for what you write on paper. This interpreter is **aligned** with that guide so you can run and test similar code. If the guide and the interpreter disagree, **follow the guide** in examinations.

---

### How is this different from Python?

Python uses **indentation** for blocks and `def` for functions; Pseudocode uses **keywords** (`ENDIF`, `ENDWHILE`, …) and `PROCEDURE` / `FUNCTION`. Assignment is `<-` or `←`, not `=`. Types are **declared** explicitly. Many details (string indexing base, `DIV`/`MOD`, `REPEAT`/`UNTIL` semantics) differ. See the [Tutorial](tutorial/index.md) and [Language reference](reference/language.md).

---

### How is this different from Julia?

Julia is a **general-purpose** language with multiple dispatch and rich numerics. Pseudocode here is **educational** and **fixed** to exam-style constructs. There is no REPL history integration on par with Julia’s, and no package ecosystem comparable to `Pkg.jl` yet—only **`pkg`** for local folders plus a [registry design](../registry/registry-schema.md).

---

### Can I use this in the exam room?

Usually **no**—exams are on paper unless your centre runs a practical on approved software. Use this for **learning**, **homework**, and **mock practice** unless your teacher says otherwise.

---

### Why does `check` say OK but my program still crashes?

`pseudocode check` currently verifies **parsing** (and basic program shape). It does **not** prove absence of **runtime** errors (division by zero, bad array index, missing file). Run the program and read [Error handling](error-handling.md).

---

### Why does the REPL say “redeclaration”?

The REPL keeps one global environment for the session. You cannot `DECLARE` the same name twice in that environment. Use a new name, or restart the REPL.

---

### Does `RANDOM` match Excel / Python’s `random`?

No guarantees. The implementation uses C `rand()` with a time seed. Distributions are **uniform** in the documented ranges only. For reproducible lessons, fix the seed (feature request) or avoid randomness in tests.

---

### UTF-8 and special quotes

Source files should be **UTF-8**. The Cambridge PDF sometimes shows **typographic quotes**; in this interpreter use **ASCII** `'` and `"` for char and string literals unless your editor normalises them.

---

### Where do I report bugs?

Open an issue on the project repository (see [README](../README.md)) with a **minimal `.pseudo` file**, expected output, and actual output.

---

### Can I embed Pseudocode in Markdown?

Not built-in. Copy code into fenced blocks and run the file separately, or use a notebook workflow your school provides.
