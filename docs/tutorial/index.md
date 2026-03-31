# Tutorial: Pseudocode in 30 minutes

This tutorial assumes you have already [built and installed](../installation.md) the `pseudocode` program. You will write short programs, run them from the terminal, and learn enough of the language to read and write **Cambridge International** pseudocode as used in **IGCSE Computer Science (0478 / CS0478)**, **AS & A Level Computer Science (9618 / CS9618)**, and **AS & A Level Information Technology (9626 / IT9626)**.

For authoritative exam rules, always follow your **Pseudocode Guide for Teachers**; this runtime adds a few **documented extensions** (aliases such as `SUBSTRING`, `TOINTEGER`) for convenience.

---

## 1. Running your first program

Create a file `hello.pseudo`:

```text
// My first program
OUTPUT "Hello, world"
```

Run it:

```bash
pseudocode hello.pseudo
```

You should see:

```text
Hello, world
```

**What happened:** the interpreter read the file, parsed the `OUTPUT` statement, and printed the string to standard output, followed by a newline.

**Try this:**

```bash
pseudocode check hello.pseudo
```

That parses the file without running it. It is useful in scripts and before exams to catch syntax errors early.

---

## 2. Variables and assignment

Variables must be **declared** before use. The assignment operator is **`<-`** (two characters) or the Unicode arrow **←**.

```text
DECLARE Score : INTEGER
DECLARE Name : STRING

Score <- 42
Name <- "Ada"
OUTPUT "Name: ", Name, " Score: ", Score
```

**Notes:**

- Identifiers are **case-insensitive**: `score` and `Score` are the same name.
- `INTEGER` holds whole numbers; `STRING` holds text in double quotes.

---

## 3. Constants

Use `CONSTANT` when a name should never change. Only **literals** are allowed on the right (not expressions).

```text
CONSTANT MaxMark = 100
CONSTANT PassMark = 50
DECLARE Mark : INTEGER
Mark <- 72
IF Mark >= PassMark THEN
  OUTPUT "Pass"
ENDIF
```

---

## 4. Arithmetic and types

Basic types you will use most often:

| Type | Purpose |
|------|---------|
| `INTEGER` | Whole numbers |
| `REAL` | Decimal numbers |
| `STRING` | Text |
| `CHAR` | Single character (single quotes) |
| `BOOLEAN` | `TRUE` or `FALSE` |
| `DATE` | Dates (often as `dd/mm/yyyy`) |

Arithmetic uses `+`, `-`, `*`, `/`, `DIV` (integer division), and `MOD` (remainder). The operator `/` produces a **real** result even if operands are integers.

```text
DECLARE a : INTEGER
DECLARE b : REAL
a <- 7 DIV 2        // 3
b <- 7 / 2          // real division
OUTPUT a, b
```

---

## 5. Making decisions: `IF`

```text
DECLARE n : INTEGER
n <- -3
IF n < 0 THEN
  OUTPUT "negative"
ELSE
  OUTPUT "non-negative"
ENDIF
```

Every `IF` that opens with `IF ... THEN` is closed with `ENDIF`. You may omit `ELSE`.

---

## 6. Choosing with `CASE`

`CASE` compares one variable against several **branches**. A branch can be a single value, a range, or `OTHERWISE`.

```text
DECLARE Grade : CHAR
Grade <- 'B'
CASE OF Grade
  'A' : OUTPUT "Excellent"
  'B' : OUTPUT "Good"
  'C' : OUTPUT "Satisfactory"
  OTHERWISE : OUTPUT "See your teacher"
ENDCASE
```

---

## 7. Loops

### Counted loop: `FOR` … `NEXT`

```text
DECLARE i : INTEGER
FOR i <- 1 TO 5
  OUTPUT i
NEXT i
```

With a step:

```text
FOR i <- 10 TO 1 STEP -1
  OUTPUT i
NEXT i
```

If the start is greater than the end and the step is positive, the body does not run (Cambridge convention).

### Pre-test loop: `WHILE`

```text
DECLARE n : INTEGER
n <- 3
WHILE n > 0
  OUTPUT n
  n <- n - 1
ENDWHILE
```

### Post-test loop: `REPEAT` … `UNTIL`

The body runs at least once. The loop **stops** when the `UNTIL` condition is **TRUE**.

```text
DECLARE Password : STRING
REPEAT
  OUTPUT "Enter password:"
  INPUT Password
UNTIL Password = "secret"
OUTPUT "Access granted"
```

---

## 8. Procedures and functions

A **procedure** performs an action; call it with `CALL`. A **function** returns a value; use it inside an expression **without** `CALL`.

```text
PROCEDURE Greet(Name : STRING)
  OUTPUT "Hello, ", Name
ENDPROCEDURE

FUNCTION Double(x : INTEGER) RETURNS INTEGER
  RETURN x * 2
ENDFUNCTION

CALL Greet("Sam")
OUTPUT Double(21)
```

Parameters are **by value** unless you write `BYREF`. By-reference parameters let a procedure change the caller’s variable (see [Language reference](../reference/language.md#subprograms)).

---

## 9. Arrays

Declare a fixed-size array with explicit bounds (lower and upper are inclusive):

```text
DECLARE Items : ARRAY[1:5] OF INTEGER
DECLARE k : INTEGER
FOR k <- 1 TO 5
  Items[k] <- k * 10
NEXT k
OUTPUT Items[3]
```

Two dimensions:

```text
DECLARE Board : ARRAY[1:3,1:3] OF CHAR
Board[2,3] <- 'X'
```

Whole-array assignment is supported when both sides are arrays of compatible shape (implementation detail: see [Language reference](../reference/language.md#arrays)).

---

## 10. Strings and built-in functions

Concatenate with `&`:

```text
DECLARE s : STRING
s <- "Hello" & " " & "world"
OUTPUT s
OUTPUT LENGTH(s)
```

See [Built-in functions](../reference/builtins.md) for `MID`, `RIGHT`, `INT`, `RAND`, and others.

---

## 11. Text files

Typical pattern: open, loop until end-of-file, close.

```text
DECLARE Line : STRING
OPENFILE "input.txt" FOR read
WHILE NOT EOF("input.txt")
  READFILE "input.txt", Line
  OUTPUT Line
ENDWHILE
CLOSEFILE "input.txt"
```

File names can be string variables. Modes after `FOR` are: `read`, `write`, `append`, `random` (see [Language reference](../reference/language.md#file-handling)).

---

## 12. Using the REPL

```bash
pseudocode repl
```

Type one or more lines, then press **Enter on a blank line** to execute the block. Use `:quit` to exit. The REPL keeps one **global environment**; redeclaring the same name in the same session is an error—use new names or restart.

---

## 13. Where to go next

- **Exact grammar and edge cases:** [Language reference](../reference/language.md)
- **Every built-in:** [Built-in functions](../reference/builtins.md)
- **CLI flags and scripting:** [CLI](../cli.md)
- **Teaching and errors:** [Error handling](../error-handling.md)
- **More programs:** [Examples](../examples.md)

You now have the same “read the tutorial, then the library reference” workflow recommended for [Python](https://docs.python.org/3/tutorial/index.html) and [Julia](https://docs.julialang.org/en/v1/manual/getting-started/) users.
