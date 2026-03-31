# Language reference

This document specifies the **Pseudocode** dialect accepted by the reference interpreter **pseudocode** v1.0.x. It follows the **Cambridge International** Pseudocode Guide (as used for **IGCSE Computer Science (0478 / CS0478)**, **AS & A Level Computer Science (9618 / CS9618)**, and **AS & A Level Information Technology (9626 / IT9626)**) where possible. Sections marked **Extension** describe behaviour added for teaching convenience; exam answers should follow your official guide for *your* syllabus if it differs.

**See also:** [Built-in functions](builtins.md) · [Tutorial](../tutorial/index.md) · [Syntax quick reference](../syntax-reference.md)

---

## 1. Lexical structure

### 1.1 Source encoding

Source files are interpreted as **UTF-8**. The assignment operator may be written as:

- U+2190 **LEFTWARDS ARROW** (`←`), or  
- ASCII **`<-`** (less-than followed by hyphen).

### 1.2 Comments

A comment starts with `//` and runs to the end of the line. There are no block comments.

### 1.3 Whitespace and newlines

Spaces, tabs, and newlines separate tokens. In most places newlines are optional between statements; the parser skips blank lines. Some constructs (for example `CASE` arms) rely on statement boundaries; see §6.3.

### 1.4 Identifiers

An identifier consists of letters `A–Z` and `a–z`, digits `0–9`, and underscore `_`. It **must not** begin with a digit.

**Case insensitivity:** `Total`, `total`, and `TOTAL` denote the same identifier.

Identifiers must not spell a **reserved word** (keyword) in exam papers; the interpreter reserves the same words listed in §1.5.

### 1.5 Keywords

The following are reserved (matched case-insensitively), including types and statement openers:

`DECLARE`, `CONSTANT`, `IMPORT`, `AS`, `INTEGER`, `REAL`, `STRING`, `CHAR`, `BOOLEAN`, `DATE`, `ARRAY`, `OF`, `TYPE`, `ENDTYPE`, `IF`, `THEN`, `ELSE`, `ENDIF`, `CASE`, `ENDCASE`, `OTHERWISE`, `FOR`, `TO`, `STEP`, `NEXT`, `REPEAT`, `UNTIL`, `WHILE`, `ENDWHILE`, `PROCEDURE`, `ENDPROCEDURE`, `FUNCTION`, `ENDFUNCTION`, `RETURNS`, `CALL`, `RETURN`, `BYVAL`, `BYREF`, `INPUT`, `OUTPUT`, `OPENFILE`, `CLOSEFILE`, `READFILE`, `WRITEFILE`, `SEEK`, `GETRECORD`, `PUTRECORD`, `AND`, `OR`, `NOT`, `MOD`, `DIV`, `TRUE`, `FALSE`.

### 1.6 Literals

| Form | Type |
|------|------|
| Decimal digits, optional leading `-` | `INTEGER` (token may merge into date; see below) |
| Digits with `.` and fractional part | `REAL` |
| `"..."` with escapes `\\`, `\"`, `\n`, `\t`, `\r` | `STRING` |
| `'x'` (single character) | `CHAR` |
| `TRUE`, `FALSE` | `BOOLEAN` |
| `d/m/yyyy` where final group is **four digits** | `DATE` (lexer token; see §2.6) |

### 1.7 Operators and punctuation

`+` `-` `*` `/` `=` `<>` `<` `>` `<=` `>=` `&` `(` `)` `[` `]` `,` `:` `.`

---

## 2. Types and values

### 2.1 Primitive types

- **INTEGER** — signed whole numbers (implementation uses 64-bit internally).
- **REAL** — floating-point.
- **STRING** — immutable character sequence in memory.
- **CHAR** — one character.
- **BOOLEAN** — `TRUE` or `FALSE`.
- **DATE** — stored as a string in `dd/mm/yyyy` form for this implementation.

### 2.2 Arrays

Arrays are **fixed-size** at declaration. Bounds are **inclusive** at both ends.

```text
DECLARE A : ARRAY[lower : upper] OF elementType
DECLARE M : ARRAY[r1 : r2, c1 : c2] OF elementType
```

Bound expressions are evaluated once at declaration time; they must yield integers.

### 2.3 Constants

```text
CONSTANT name = expression
```

Exam papers often show a **single literal**. The interpreter accepts a full **expression** on the right (including the Extension comma-chain for strings documented in §3.1).

### 2.4 Variable declaration

```text
DECLARE name : typeName
```

`typeName` is one of `INTEGER`, `REAL`, `STRING`, `CHAR`, `BOOLEAN`, `DATE`, or an array form as in §2.2.

### 2.5 Scope and redeclaration

At the top level of a program, each name may be declared once in the current environment. The interpreter reports a **runtime** error on redeclaration.

Procedure and function bodies use a **new local environment** whose parent is the global environment. Names in inner scopes **shadow** globals.

### 2.6 Date literals **(lexer)**

A numeric sequence matching `⟨int⟩/⟨int⟩/⟨yyyy⟩` (final part exactly four digits) is tokenised as a single **DATE** literal. Other uses of `/` between integers remain division. Ambiguous cases are rare in classroom code; when in doubt, use a string or declare the format in comments (exam style).

---

## 3. Expressions

### 3.1 Precedence (high to low)

1. Primary: literals, variables, `( expr )`, function calls, indexing  
2. Unary: `-` `NOT`  
3. `*` `/` `DIV` `MOD`  
4. `+` `-` (for numbers; `+` also joins two **STRING** values—see §3.3)  
5. `&` (string concatenation)  
6. `=` `<>` `<` `>` `<=` `>=`  
7. `AND`  
8. `OR`  

**Extension:** On the right-hand side of **assignment**, **CONSTANT**, and **RETURN**, a **comma-separated list** of full expressions is parsed as left-associative string concatenation (each segment must evaluate to **STRING**). Commas inside `IF` / `WHILE` conditions, `FOR` bounds, procedure arguments, **`WRITEFILE`** (single data expression), and similar positions do **not** gain this meaning. Details: [Strings and concatenation](strings-and-concatenation.md).

Use parentheses when unsure.

### 3.2 Arithmetic

- `DIV` and `MOD` use **integer** semantics; `DIV` by zero is a runtime error.
- `/` promotes to **real** division.

### 3.3 Strings

- **`&`** — concatenates two **STRING** operands (Cambridge-style).  
- **`+`** — if **both** operands are **STRING**, concatenates them; if both are numeric, adds; mixing string and number is a runtime error.  
- **Comma (`,`)** — only in contexts listed in §3.1 (assignment-style RHS): joins adjacent **STRING** expressions with **lower** precedence than `OR` (you are chaining whole expressions, not breaking `+` across a comma accidentally in the middle of a subexpression in the way `OUTPUT` lists work—see [Strings and concatenation](strings-and-concatenation.md)).

### 3.4 Boolean expressions

`AND` and `OR` **short-circuit**: `AND` skips the right operand if the left is false; `OR` skips the right if the left is true.

### 3.5 Function calls in expressions

```text
identifier ( argumentList )
```

User-defined **functions** and **built-in** functions use the same call syntax. Do **not** prefix with `CALL`.

**Qualified names** (extension, Python-style): `prefix.name ( argumentList )` where `prefix` is one or more identifiers separated by dots, **only** as a call target (not as a variable). Typical use: **`np.sum(A)`** after **`IMPORT numpy AS np`**. The first segment must be either a **library alias** from `IMPORT` or a supported module name (`numpy`, `pandas`, `matplotlib`). Only **one** dot is allowed between prefix and method (e.g. `np.sum`, not `np.foo.bar`). See [Built-in functions — Python-style libraries](builtins.md#python-style-library-modules-numpy-pandas-matplotlib).

### 3.6 Indexing

One dimension: `name[index]`. Two dimensions: `name[row, col]` parsed as nested indices (see implementation). Indices are expressions evaluated to integers; out-of-range access is a runtime error.

---

## 4. Statements

### 4.0 `IMPORT` (Python-style library alias)

```text
IMPORT moduleName
IMPORT moduleName AS alias
```

**Supported `moduleName` values** (case-insensitive): **`numpy`**, **`pandas`**, **`matplotlib`**.

- **`IMPORT numpy AS np`** — subsequent calls like **`np.sum(A)`** resolve to built-in **`numpy.sum`**.
- **`IMPORT numpy`** — same as **`IMPORT numpy AS numpy`**; you may call **`numpy.sum(A)`** directly without a short alias.

This statement registers aliases only; it does **not** load files from disk. Re-importing the same alias replaces the previous binding.

### 4.1 Assignment

```text
lvalue <- expression
```

`lvalue` is a variable name or an indexed array element. The right-hand side may be a **comma-separated chain** of expressions that concatenate strings (Extension); see §3.1 and [Strings and concatenation](strings-and-concatenation.md).

Whole-array assignment is permitted when the right-hand side evaluates to an array value compatible with the left (implementation copies storage).

### 4.2 `INPUT`

```text
INPUT variableOrIndexedVariable
```

Reads one line from **stdin** (implementation). Trailing newline is stripped. Parsing into `INTEGER` / `REAL` / `BOOLEAN` / `CHAR` / `STRING` follows C library parsing (`strtoll`, `strtod`, etc.).

### 4.3 `OUTPUT`

```text
OUTPUT expr1 , expr2 , ...
```

Prints each value, separated by a single space, then a newline.

### 4.4 `IF`

```text
IF condition THEN
  statements
[ ELSE
  statements ]
ENDIF
```

### 4.5 `CASE`

```text
CASE OF discriminant
  valueOrRange : statements
  ...
  [ OTHERWISE : statements ]
ENDCASE
```

- **Integer** discriminant: a branch `low TO high` matches if `low ≤ d ≤ high` (after evaluating bounds).
- **CHAR** discriminant: range compares character codes.
- **STRING** / **DATE**: single-value equality only (compared to the first expression of the branch).

Execution runs the **first** matching branch only, then exits the `CASE`.

### 4.6 `FOR`

```text
FOR iterator <- start TO end [ STEP step ]
  statements
NEXT iterator
```

The iterator variable must already be declared as `INTEGER`. Step defaults to `1`. If `step > 0`, the loop runs while `iterator ≤ end`; if `step < 0`, while `iterator ≥ end`. `step = 0` is a runtime error.

### 4.7 `WHILE`

```text
WHILE condition
  statements
ENDWHILE
```

### 4.8 `REPEAT`

```text
REPEAT
  statements
UNTIL condition
```

Loop terminates when `condition` is **TRUE**.

---

## 5. Subprograms

### 5.1 Procedure definition

```text
PROCEDURE name ( [BYVAL|BYREF] param : type , ... )
  statements
ENDPROCEDURE
```

### 5.2 Procedure call

```text
CALL name ( arguments )
```

### 5.3 Function definition

```text
FUNCTION name ( [BYVAL|BYREF] param : type , ... ) RETURNS type
  statements
ENDFUNCTION
```

`BYREF` on function parameters is not allowed by Cambridge style; the interpreter may not enforce this in all paths—avoid it in exam code.

### 5.4 `RETURN`

In a function, `RETURN expr` assigns the result and exits the function immediately. If execution falls off the end without `RETURN`, a runtime error is reported.

### 5.5 Parameter passing

- **BYVAL** (default): argument value is copied.
- **BYREF**: argument must be a simple variable (implementation); callee updates share storage with the caller’s variable.

---

## 6. File handling

### 6.1 Opening and closing

```text
OPENFILE fileExpr FOR mode
CLOSEFILE fileExpr
```

`fileExpr` is a string expression (literal or variable). `mode` is an identifier: `read`, `write`, `append`, or `random` (case-insensitive).

### 6.2 Text lines

```text
READFILE fileExpr , stringVariable
WRITEFILE fileExpr , dataExpr
```

`READFILE` reads the next line without the newline. `WRITEFILE` writes the value and appends a newline.

### 6.3 End-of-file

```text
EOF ( fileExpr )
```

Returns `TRUE` if there is no more data to read (implementation: peek ahead in the open file handle). See [Built-in functions](builtins.md#eof).

### 6.4 Random / binary records

`SEEK`, `GETRECORD`, and `PUTRECORD` are **not** fully implemented in v1.0 for arbitrary record types. Prefer text files for portable classroom use.

---

## 7. User-defined types (RECORD, pointers, OOP)

**Not implemented** in v1.0. The Cambridge guide describes `TYPE` … `ENDTYPE` records, pointers, sets, and object-oriented features; these are **planned extensions**. Programs using only material in §§1–6 are portable on this interpreter.

---

## 8. Formal grammar (informative)

The implementation is a hand-written recursive-descent parser. An approximate EBNF for statements:

```text
program        ::= statement*
statement      ::= declare | constant | importStmt | assign | ifStmt | caseStmt | forStmt
                 | whileStmt | repeatStmt | procDef | funcDef | callStmt
                 | returnStmt | inputStmt | outputStmt | openFile | closeFile
                 | readFile | writeFile
importStmt     ::= IMPORT IDENT [ AS IDENT ]
declare        ::= DECLARE IDENT : type
constant       ::= CONSTANT IDENT = literal
assign         ::= lvalue ASSIGN expr
...
```

Full detail is in the source files `src/pc_lexer.c`, `src/pc_parser.c`, and `src/pc_interp.c`.

---

## 9. Versioning

Behaviour documented here matches **interpreter version 1.0.x** (`PC_VERSION_STRING` in `src/pc_common.h`). Minor releases may fix bugs without changing syntax; incompatible changes would bump the major version and this document.
