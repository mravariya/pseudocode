# Errors and diagnostics

This chapter describes **what can go wrong**, **how the interpreter reports it**, and **how to recover**—similar in role to sections of the **Python tutorial** on exceptions and the **Julia** manual on stack traces (here, messages are textual and line-based rather than a full stack for Pseudocode source).

**See also:** [CLI — exit status](cli.md#exit-status) · [Language reference](reference/language.md) · [FAQ](faq.md)

---

## 1. Error categories

| Phase | When | Typical causes |
|-------|------|----------------|
| **Lexical** | Scanning source | Invalid bytes, unclosed string |
| **Syntax (parse)** | Building AST | Missing `ENDIF`, wrong token where expression expected |
| **Runtime** | Executing AST | Undefined variable, bad index, division by zero, I/O failure |

The **`pseudocode check`** command catches **parse** failures only (plus trivial program checks). It does **not** simulate all runtime paths.

---

## 2. Message format

When the controlling terminal supports it, messages may use **ANSI colours** (red for “error”, dim for “note”). Disable with:

```bash
export PSEUDO_NO_COLOR=1
```

A typical **parse** error:

```text
error at path/to/file.pseudo:12:3: expected ENDIF, got token
```

Fields:

- **`path/to/file.pseudo`** — source file path passed to the lexer.  
- **`12`** — **1-based line number** at the start of the problematic token.  
- **`3`** — **1-based column** (best effort; UTF-8 wide characters may skew columns in some terminals).

A typical **runtime** error:

```text
runtime error at path/to/file.pseudo:45:1: division by zero
```

---

## 3. Parse errors — catalogue

These are representative; wording may vary slightly between versions.

| Situation | You might see |
|-----------|----------------|
| Missing closing keyword | `expected ENDIF` / `expected ENDWHILE` / … |
| Wrong punctuation | `expected ')'` / `expected ','` |
| Statement start invalid | `unexpected token at start of statement` |
| Expression incomplete | `expected expression` |
| File incomplete | `expected end of file` |

**Remediation:** Compare your block structure with the [Language reference](reference/language.md#4-statements). Use consistent indentation for humans; the interpreter does not require indentation but teachers do.

---

## 4. Runtime errors — catalogue

| Situation | Example message (informative) |
|-----------|-------------------------------|
| Use before declare | `undefined variable 'x'` |
| Redeclaration | `redeclaration of 'x'` |
| Array bounds | `array index out of range` |
| Integer division by zero | `division by zero` |
| Bad built-in usage | `LENGTH expects STRING or ARRAY` |
| Missing return | `function 'F' did not RETURN` |
| Undefined procedure | `undefined procedure 'P'` |
| Undefined function | `undefined function 'F'` |
| File open failure | `could not open file '…'` |
| BYREF misuse | `BYREF requires a simple variable` |

---

## 5. Trace mode

Enable **statement-level** tracing:

```bash
PSEUDO_TRACE=1 pseudocode program.pseudo
```

Each executed statement prints a short line on **stderr** (implementation detail: not every inner expression is traced). Use this in labs to show **order of execution** without a full debugger.

---

## 6. REPL behaviour

In **`pseudocode repl`**, after a run that failed, the next buffer can be entered normally; the interpreter clears the **“had error”** flag between REPL executions so you can fix and retry. **Global state** (variables) persists until you restart the REPL.

---

## 7. Teaching tips

1. **Read line and column** aloud with students—map to the guide’s line-number conventions.  
2. Run **`check`** before **`run`** in automated homework scripts.  
3. Distinguish **syntax** (compiler) vs **logic** (wrong answer but runs) vs **runtime** (crashes).  
4. For exams, practise **paper** tracing; the interpreter validates **mechanical** correctness, not **mark scheme** wording.

---

## 8. Reporting bugs upstream

Include:

- **Minimal** `.pseudo` file.  
- **Command line** used.  
- **Full stderr** output.  
- **Version** from `pseudocode version`.  
- **OS** and compiler (if custom build).

---

## 9. Future improvements

Possible enhancements (not in v1.0):

- Stable **error codes** (e.g. `PC_ERR_UNDEFINED_VAR`) for LMS integration.  
- **Warnings** (unused variables, unreachable code).  
- **Structured** JSON errors for IDEs.  
- **Source-level** stack traces with procedure names.
