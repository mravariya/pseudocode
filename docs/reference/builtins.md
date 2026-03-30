# Built-in functions

This page lists every **pre-defined** callable that is resolved by the interpreter **without** a user `FUNCTION` definition. Names are **case-insensitive**.

**Syntax:** like ordinary function calls: `NAME(arg1, arg2, …)` inside expressions.

**Errors:** wrong arity or invalid types produce a **runtime error** with a short message and (when available) source location.

**See also:** [Language reference — expressions](language.md#3-expressions) · [I/O statements](language.md#4-statements) · [Standard library topics (Python comparison)](../stdlib/index.md)

---

## Summary table

| Name | Arguments | Returns |
|------|-----------|---------|
| `LENGTH` | string **or** array | `INTEGER` |
| `MID` / `SUBSTRING` | string, start, length | `STRING` |
| `RIGHT` | string, count | `STRING` |
| `CONCAT` | string, string | `STRING` |
| `INT` / `TOINTEGER` | numeric or string | `INTEGER` |
| `TOREAL` | numeric or string | `REAL` |
| `RAND` / `RANDOM` | optional upper bound | `REAL` |
| `RANDINT` / `RANDOMINT` | low, high (inclusive) | `INTEGER` |
| `RANDOMSEED` / `SEEDRANDOM` / `SEED` | seed value | `INTEGER` (`0`) |
| `ROUND` | numeric | `INTEGER` |
| `SQRT`, `SIN`, `COS`, `TAN`, `ASIN`, `ACOS`, `ATAN` | numeric | `REAL` |
| `ATAN2`, `HYPOT`, `POW`, `FMOD` / `MODREAL` | two numerics | `REAL` |
| `LOG`, `LOG10`, `EXP` | numeric | `REAL` |
| `FLOOR`, `CEIL`, `TRUNC` | numeric | `REAL` |
| `ABS` | numeric | `INTEGER` or `REAL` |
| `PI` | none | `REAL` |
| `EULER` / `E_CONST` | none | `REAL` |
| `RADIANS`, `DEGREES` | numeric | `REAL` |
| `TIME` | none | `REAL` |
| `CLOCKMS` | none | `INTEGER` |
| `NOWSTRING` | none | `STRING` |
| `GETENV` | string name | `STRING` |
| `SYSTEM` | string command | `INTEGER` |
| `UPPERCASE` | string | `STRING` |
| `LOWERCASE` | string | `STRING` |
| `UCASE` | char | `CHAR` |
| `LCASE` | char | `CHAR` |
| `EOF` | file path string | `BOOLEAN` |

`INPUT` and `OUTPUT` are **statements**, not functions.

---

## String functions

### `LENGTH(x)`

- **Overload 1 — string:** Returns the number of characters in `x` (UTF-8 byte length for multi-byte sequences in this C implementation; ASCII classroom strings behave as expected).
- **Overload 2 — array:** Returns the **total** number of elements (for 2D arrays: rows × columns).

**Errors:** if `x` is neither string nor array.

---

### `MID(s, x, y)` and `SUBSTRING(s, x, y)`

- **s** — `STRING`
- **x** — starting position, **1-based** (first character is position 1)
- **y** — length of substring (number of characters)

If `x < 1`, it is treated as `1`. If the range extends past the string end, the result is truncated.

**Returns:** new `STRING`.

**Errors:** first argument must be a string; wrong arity.

---

### `RIGHT(s, n)`

Returns the last `n` characters of `s`. If `n` exceeds the string length, returns the whole string. If `n < 0`, treated as `0` (empty string).

**Errors:** `s` must be a string.

---

### `CONCAT(a, b)`

Concatenates two strings. Missing or null string pointers are treated as empty in the implementation.

**Returns:** `STRING`.

**Note:** Exam style often uses `&`; `CONCAT` is equivalent for two operands.

---

## Numeric functions

### `INT(x)` and `TOINTEGER(x)`

- If `x` is `STRING`, parses as integer with `strtoll` (leading whitespace allowed; stops at first invalid character).
- If `x` is `INTEGER` or `REAL`, converts to integer (truncation toward zero for reals).

**Errors:** invalid value / missing type.

---

### `TOREAL(x)`

- If `x` is `STRING`, parses with `strtod`.
- Otherwise converts numeric to `REAL`.

---

### `ROUND(x)`

Rounds to nearest integer, **half away from zero** using `floor(x + 0.5)` for non-negative values; behaviour for negative halves follows that formula in the implementation.

**Returns:** `INTEGER`.

---

### `RAND(x)` / `RANDOM(x)` / `RANDOM()`

- **`RANDOM()`** (no arguments) — returns a `REAL` in `[0, 1)`.
- **`RANDOM(n)`** or **`RAND(n)`** with integer `n > 0` — returns a `REAL` in `[0, n)` (uniform; not inclusive of `n`).

Uses the C `rand()` generator; seeded once per process start from the clock (**not** cryptographically secure).

**Errors:** `n <= 0` for the one-argument form.

---

### `RANDINT(a, b)` / `RANDOMINT(a, b)`

Returns a uniform **integer** in the **closed** interval `[a, b]` (both endpoints included). Uses the same `rand()` generator as **`RANDOM`**.

**Errors:** if `b < a`.

---

### `RANDOMSEED(x)` / `SEEDRANDOM(x)` / `SEED(x)`

Seeds the global `rand()` generator with the integer value of **`x`** (low bits of the value are used). Returns **`0`** so the call can appear in an expression; the return value is not meaningful.

---

## Math-style functions

Single-argument functions expect a numeric value and return **`REAL`** (C `double` via `<math.h>`). Names are **case-insensitive**. For a Python ↔ API table and gaps, see [stdlib: math](../stdlib/math.md).

| Name | Arity | Notes |
|------|-------|--------|
| `SQRT`, `SIN`, `COS`, `TAN`, `ASIN`, `ACOS`, `ATAN` | 1 | Domain errors follow C (e.g. `SQRT` of negative may be NaN or host-defined). |
| `LOG`, `LOG10`, `EXP` | 1 | Natural log / base-10 log / `e^x`. |
| `FLOOR`, `CEIL`, `TRUNC` | 1 | `TRUNC` is toward zero (C `trunc`). |
| `ATAN2(y, x)` | 2 | Two-argument arctangent. |
| `HYPOT(x, y)` | 2 | `sqrt(x*x + y*y)` (C `hypot`). |
| `POW(b, e)` | 2 | `b` to the power `e`. |
| `FMOD` / `MODREAL` | 2 | Floating remainder (C `fmod`). |
| `RADIANS(deg)` | 1 | Degrees → radians. |
| `DEGREES(rad)` | 1 | Radians → degrees. |
| `PI` | 0 | Constant π. |
| `EULER` / `E_CONST` | 0 | Constant *e*. |

### `ABS(x)`

- If **`x`** is **`INTEGER`**, returns **`INTEGER`** absolute value (`llabs`).
- Otherwise returns **`REAL`** absolute value (`fabs`).

---

## Time and environment

### `TIME()`

Seconds since the Unix epoch as **`REAL`** (`time(NULL)` cast to `double`). No arguments.

---

### `CLOCKMS()`

Approximate **CPU time** used by the process in **milliseconds** from `clock()`. No arguments.

**Errors:** if `clock()` is unavailable on the host.

---

### `NOWSTRING()`

Current **local** wall-clock time formatted as `YYYY-MM-DD HH:MM:SS`. No arguments.

**Errors:** if formatting fails.

---

### `GETENV(name)`

**`name`** must be **`STRING`**. Returns the environment value as **`STRING`**, or **empty string** if unset.

---

### `SYSTEM(cmd)`

**`cmd`** must be **`STRING`**. Passed to the C library **`system()`** (host shell). Returns the host status code as **`INTEGER`**.

**Security:** equivalent to running a shell command; never use with untrusted input.

---

## Character case

### `UCASE(c)` / `LCASE(c)`

Cambridge-style **single-character** conversion.

- **Argument:** treated as `CHAR` (implementation reads the `CHAR` slot of the value).
- **Returns:** `CHAR`.

Non-letters are returned unchanged (for letters, case is toggled per ASCII rules).

---

### `UPPERCASE(s)` / `LOWERCASE(s)`

**Extension** — full **string** case mapping (ASCII letters only in this implementation).

**Returns:** new `STRING`.

If `s` is empty or null, returns an empty string.

---

## File predicate

### `EOF(path)`

- **path** — `STRING` expression identifying the file (same path string used with `OPENFILE` / `READFILE`).

**Returns:** `TRUE` if there is no open stream or no further data to read (implementation-defined at EOF); `FALSE` if another line could be read.

**Note:** Must be used with a file already opened for **read**; behaviour is undefined if the path was never opened or was closed.

---

## Examples

```text
DECLARE s : STRING
DECLARE n : INTEGER
s <- "ABCDEFGH"
OUTPUT MID(s, 2, 3)     // "BCD"
OUTPUT RIGHT(s, 3)      // "FGH"
OUTPUT LENGTH(s)        // 8
OUTPUT RANDOM(100)      // e.g. 47.32...
OUTPUT INT(" -12")      // -12
OUTPUT UPPERCASE("ab")  // "AB"
```

---

## Adding new builtins

Contributors implement new branches in `src/pc_stdlib.c` (pure functions) or in `src/pc_interp.c` when the builtin needs interpreter state (like `EOF`). Update this document and the [Language reference](language.md) summary when you add a public builtin.
