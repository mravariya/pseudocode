# Arrays (1D and 2D)

Declaring **ARRAY** types, **bounds**, **indexing**, and **whole-array** behaviour in the reference interpreter. Structured for quick lookup (like a focused section of the [Python sequence docs](https://docs.python.org/3/library/stdtypes.html#sequence-types-list-tuple-range), but for Pseudocode).

**See also:** [Language reference — §2.2](language.md#22-arrays) · [§3.6 Indexing](language.md#36-indexing) · [Tutorial](../tutorial/index.md)

---

## Quick answers

| Question | Answer |
|----------|--------|
| One-dimensional arrays? | **Yes** — `ARRAY[lo : hi] OF type`. |
| Two-dimensional arrays? | **Yes** — `ARRAY[lo1 : hi1, lo2 : hi2] OF type`. |
| Higher rank (3D+)? | **Not** in this interpreter. |
| Bounds inclusive? | **Yes** — both ends. |
| Dynamic size / `append`? | **No** — size is fixed at declaration. |

---

## Table of contents

1. [Declaration](#1-declaration)
2. [1D indexing](#2-1d-indexing)
3. [2D indexing](#3-2d-indexing)
4. [Bounds and types](#4-bounds-and-types)
5. [Whole-array assignment](#5-whole-array-assignment)
6. [Common patterns](#6-common-patterns)
7. [Limitations](#7-limitations)

---

## 1. Declaration

### 1.1 One dimension

```text
DECLARE A : ARRAY[lower : upper] OF elementType
```

- **`lower`**, **`upper`** — expressions evaluated **once** when the declaration runs; must yield **INTEGER** values.  
- **`elementType`** — `INTEGER`, `REAL`, `STRING`, `CHAR`, `BOOLEAN`, or `DATE` (same as scalar `DECLARE`).

**Search terms:** `DECLARE ARRAY`, 1D array, one-dimensional.

### 1.2 Two dimensions

```text
DECLARE M : ARRAY[rowLo : rowHi, colLo : colHi] OF elementType
```

The comma inside `[` `]` separates the **first** and **second** dimension bounds — **not** string concatenation.

**Search terms:** 2D array, matrix, grid, two-dimensional.

---

## 2. 1D indexing

```text
A[indexExpr]
```

- **`indexExpr`** — any expression that evaluates to an integer.  
- **Out of range** (outside `lower`…`upper`) → **runtime error**.

**Search terms:** subscript, index, element access.

---

## 3. 2D indexing

```text
M[rowExpr, colExpr]
```

Syntax is a **comma inside brackets** after the array name: first index is the **row** (first declared range), second is the **column** (second range).

```text
DECLARE T : ARRAY[1 : 3, 1 : 4] OF INTEGER
T[2, 3] <- 99
```

**Search terms:** `[,]`, row column, nested index.

---

## 4. Bounds and types

| Topic | Rule |
|-------|------|
| Inclusive bounds | Both `lower` and `upper` are valid indices (when `lower <= upper`). |
| Evaluation time | Bounds are computed at **declaration** in the current environment. |
| Element type | All elements share the same type; no mixed-type arrays. |
| Arrays of arrays | Not used — use the built-in **2D** form above. |

---

## 5. Whole-array assignment

When the language reference allows it, you may assign **one array to another** of **compatible** shape and element type; the interpreter **copies** storage (details in [language.md §4.1](language.md#41-assignment)). Prefer explicit loops in exam answers unless your guide shows whole-array copy.

**Search terms:** array copy, assign array to array.

---

## 6. Common patterns

**Fill a 1D table**

```text
DECLARE I : INTEGER
DECLARE V : ARRAY[0 : 9] OF INTEGER
FOR I <- 0 TO 9
  V[I] <- 0
NEXT I
```

**Nested loops over 2D**

```text
DECLARE R : INTEGER
DECLARE C : INTEGER
DECLARE G : ARRAY[1 : 5, 1 : 5] OF STRING
FOR R <- 1 TO 5
  FOR C <- 1 TO 5
    G[R, C] <- "."
  NEXT C
NEXT R
```

---

## 7. Limitations

- **No** 3D+ arrays.  
- **No** resizing after declaration.  
- **No** sparse matrix syntax — every slot exists in the declared range.  
- **RECORD** / array of user-defined types — not in v1.0; see [OOP and user-defined types](oop-and-types.md).

---

*Index keywords:* `ARRAY`, `OF`, `1D`, `2D`, `index`, `bounds`, `whole-array`, `DECLARE`.
