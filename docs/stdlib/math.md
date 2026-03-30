# Math-style builtins (`math`)

Pseudocode exposes a subset of C’s `<math.h>` under **case-insensitive** names, similar to Python’s [`math`](https://docs.python.org/3/library/math.html) module.

**Note:** There is no `import math`; call functions directly, e.g. `OUTPUT SQRT(2.0)`.

---

## Implemented (Python `math` name → Pseudocode)

| Python | Pseudocode | Args | Returns |
|--------|------------|------|---------|
| `math.sqrt` | `SQRT` | `REAL` | `REAL` |
| `math.sin` | `SIN` | `REAL` | `REAL` |
| `math.cos` | `COS` | `REAL` | `REAL` |
| `math.tan` | `TAN` | `REAL` | `REAL` |
| `math.asin` | `ASIN` | `REAL` | `REAL` |
| `math.acos` | `ACOS` | `REAL` | `REAL` |
| `math.atan` | `ATAN` | `REAL` | `REAL` |
| `math.atan2` | `ATAN2` | `REAL`, `REAL` | `REAL` |
| `math.hypot` | `HYPOT` | `REAL`, `REAL` | `REAL` |
| `math.log` (natural) | `LOG` | `REAL` | `REAL` |
| `math.log10` | `LOG10` | `REAL` | `REAL` |
| `math.exp` | `EXP` | `REAL` | `REAL` |
| `math.pow` | `POW` | `REAL`, `REAL` | `REAL` |
| `math.floor` | `FLOOR` | `REAL` | `REAL` |
| `math.ceil` | `CEIL` | `REAL` | `REAL` |
| `math.trunc` | `TRUNC` | `REAL` | `REAL` |
| `math.fmod` | `FMOD` or `MODREAL` | `REAL`, `REAL` | `REAL` |
| `math.degrees` | `DEGREES` | `REAL` (radians) | `REAL` |
| `math.radians` | `RADIANS` | `REAL` (degrees) | `REAL` |
| `abs` (built-in) | `ABS` | numeric | `INTEGER` or `REAL` |
| `math.pi` | `PI` | none | `REAL` |
| `math.e` | `EULER` or `E_CONST` | none | `REAL` |

---

## Not implemented (common Python `math` names)

These exist in Python but are **not** builtins here (use your own `FUNCTION` or extend `pc_stdlib.c`):

`math.factorial`, `math.gcd`, `math.lcm`, `math.isfinite`, `math.isnan`, `math.isinf`, `math.copysign`, `math.frexp`, `math.ldexp`, `math.modf`, `math.remainder`, `math.nextafter`, `math.ulp`, hyperbolic functions (`sinh`, `cosh`, …), `math.erf`, `math.gamma`, `math.lgamma`, `math.perm`, `math.comb`, `math.prod`, `math.dist`, `math.fsumm`, bitwise float helpers, etc.

---

## Rounding note

Exam-style **`ROUND(x)`** is a separate builtin (nearest integer via `floor(x+0.5)` in the implementation); Python’s `round()` has different tie-breaking and optional `ndigits`.

---

## Example

```text
DECLARE x : REAL
x <- SQRT(2.0)
OUTPUT x
OUTPUT PI
OUTPUT DEGREES(3.14159265)
```
