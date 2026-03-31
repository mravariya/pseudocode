# Strings and concatenation

How to build **STRING** values from pieces: operators **`&`**, **`+`**, and **comma (`,`)**, plus how **`OUTPUT`** differs. This page is written like a small chapter of a language manual (similar in spirit to the [Python tutorial on strings](https://docs.python.org/3/tutorial/introduction.html#strings)) so you can **search by keyword** (`concat`, `ampersand`, `comma`, `plus`, `OUTPUT`).

**See also:** [Language reference — §3 Expressions](language.md#3-expressions) · [Built-in string functions](builtins.md) · [Syntax quick reference](../syntax-reference.md)

---

## Quick answers

| Question | Answer |
|----------|--------|
| Cambridge exam `&` | **Yes** — supported. |
| Also use `+` for two strings? | **Yes** (interpreter extension): `"a" + "b"` → `"ab"`. |
| Use comma like `Message <- "Hi ", Name`? | **Yes** on **assignment-style** RHS only (see below). |
| Is `OUTPUT "a", "b"` the same as concatenation? | **No** — it prints **two** values with a space between them. |
| Mix `"x" + 1`? | **No** — runtime error; both sides of `+` must be STRING for string concat. |

---

## Table of contents

1. [Three ways to concatenate](#1-three-ways-to-concatenate)
2. [Where comma means “join strings”](#2-where-comma-means-join-strings)
3. [Precedence and parsing](#3-precedence-and-parsing)
4. [OUTPUT vs assignment](#4-output-vs-assignment)
5. [Examples](#5-examples)
6. [Errors and pitfalls](#6-errors-and-pitfalls)
7. [Exam alignment (Cambridge syllabi)](#7-exam-alignment-cambridge-syllabi)

---

## 1. Three ways to concatenate

### 1.1 `&` (ampersand) — Cambridge style

Binary operator between two expressions. **Both** must evaluate to **STRING** at runtime.

```text
DECLARE s : STRING
s <- "Hello " & "world"
```

**Search terms:** `&`, ampersand, string concat, concatenation.

### 1.2 `+` (plus) — overloaded

- Two **INTEGER** / **REAL** operands → normal arithmetic.
- Two **STRING** operands → **concatenation** (same result as `&` for two plain strings).
- One STRING and one number → **error** (unlike Python’s f-strings or some BASICs).

```text
s <- "a" + "b"        // OK → "ab"
n <- 1 + 2            // OK → 3
// s <- "n=" + n      // runtime error — do not mix types on +
```

**Search terms:** `+`, plus, string plus, overload.

### 1.3 Comma (`,`) — assignment-style chains only

A **comma-separated list** on the right-hand side of certain statements is parsed as **left-associative** string concatenation. Each **segment** is a full expression (from `OR` down through primaries). Every segment must be **STRING**.

```text
Message <- "Hello ", Name
```

This is an **interpreter extension** for teaching and readability; official Cambridge pseudocode emphasises `&`.

**Search terms:** comma concat, comma-separated string, RHS comma.

---

## 2. Where comma means “join strings”

Comma **joins strings** only in these positions:

| Construct | Comma meaning |
|-----------|----------------|
| `lvalue <- expr1 , expr2 , ...` | Concatenate strings (left to right). |
| `CONSTANT Id = expr1 , expr2 , ...` | Same rule for the initializer. |
| `RETURN expr1 , expr2 , ...` | Same (each part must be STRING). |
| `WRITEFILE fileExpr , dataExpr` | **One** data expression only (so integers and reals can be written without forcing STRING concat). |

Comma **does not** mean concatenation in:

- **`IF` / `WHILE` / `UNTIL`** conditions  
- **`FOR` … `TO` / `STEP`** bounds  
- **Procedure/function argument lists** — `Proc(a, b)` still means two arguments  
- **`OUTPUT expr1 , expr2`** — separate values to print (see §4)  
- **Array declaration** bounds — `ARRAY[1:10, 1:5]` is 2D syntax, not concat  

---

## 3. Precedence and parsing

Rough order (high to low) for expression **operators** (full table in [language.md §3.1](language.md#31-precedence-high-to-low)):

1. `* / DIV MOD`  
2. `+ -` (numeric, or STRING+STRING)  
3. `&`  
4. Comparisons  
5. `AND` / `OR`  

The **comma chain** for assignment-style RHS is **outside** a single `parse_expr`: each side of a comma is a **complete** expression up through `OR`. So:

```text
// Left segment is the whole OR/AND/... expression before the comma
Message <- "Hi " & Prefix, Name
// means:  ("Hi " & Prefix)  joined with  Name
```

Use parentheses inside a segment if you need a different grouping.

**Search terms:** precedence, binding, order of operations.

---

## 4. OUTPUT vs assignment

| Code | Behaviour |
|------|-----------|
| `OUTPUT "A", "B"` | Prints `A`, space, `B`, newline — **two** values. |
| `s <- "A", "B"` | **One** STRING variable `s` equal to `"AB"` (comma concat on RHS). |
| `OUTPUT "A" & "B"` | Prints one string `AB`. |

**Search terms:** OUTPUT comma, print multiple, space separator.

---

## 5. Examples

```text
DECLARE Greeting : STRING
DECLARE Name : STRING
DECLARE Line  : STRING

Name <- "Ada"
Greeting <- "Hello ", Name                    // comma chain
Line <- "Hello " & Name                        // Cambridge style
Line <- "Hello " + Name                        // same if both STRING

OUTPUT "Debug: ", Line                         // two OUTPUT arguments, not concat
```

---

## 6. Errors and pitfalls

| Situation | Typical outcome |
|-----------|------------------|
| `x <- "a", 42` | Comma concat requires **STRING** on both sides of each join — **error** if a segment is INTEGER. |
| `"a" + 1` | **Error** — build strings only from **STRING** operands, or use builtins such as `CONCAT` after converting values to strings by your own logic. |
| Forgetting `&` in exam paper | Use `&` in submitted answers; the interpreter still accepts `+` and comma where documented. |

---

## 7. Exam alignment (Cambridge syllabi)

The same string rules matter for **IGCSE Computer Science (0478 / CS0478)**, **AS & A Level Computer Science (9618 / CS9618)**, and **AS & A Level Information Technology (9626 / IT9626)** wherever pseudocode is examined — always follow **your** published guide.

- **Safe for exams:** `&` between string expressions.  
- **Extensions in this interpreter:** STRING `+` STRING; comma chains on assignment-style RHS.  
- If your mark scheme insists on pure syllabus syntax, **prefer `&`** and avoid comma-chained RHS on submitted scripts.

---

*Index keywords (for search in repo or editor):* `string concatenation`, `TOK_AMP`, `comma RHS`, `assignment comma`, `OUTPUT`, `WRITEFILE string`, `STRING + STRING`.
