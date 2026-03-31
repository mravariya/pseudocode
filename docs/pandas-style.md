# Pandas-style patterns in Pseudocode

You can write **`IMPORT pandas AS pd`** and call built-in reducers on a **single column array** (e.g. **`pd.sum(Price)`**) — see [Built-ins — library modules](reference/builtins.md#python-style-library-modules-numpy-pandas-matplotlib). There is still **no** real `DataFrame`. This page is a **conceptual bridge** for **tabular** workflows with **fixed-size** parallel arrays and **loops**.

**Runnable code:** [`examples/pandas-style/`](../examples/pandas-style/) · **Examples catalogue:** [examples.md](examples.md)

**Related:** fixed vectors and reductions are the same machinery as [NumPy-style patterns](numpy-style.md). For chart-oriented output see [Matplotlib-style patterns](matplotlib-style.md).

---

## What is missing compared to Pandas

| Pandas idea | In this Pseudocode |
|-------------|-------------------|
| `DataFrame`, `Series` objects | **No** — model a table as **one array per column**, shared **row index** `1 … n`. |
| Dynamic rows, `append`, `drop` | **No** — row count is fixed at `DECLARE`. |
| `df.loc`, `df.iloc`, slicing | **No** — use **`FOR`** over row index ranges; access **`Col[i]`**. |
| `groupby`, `merge`, `join` (general) | **Only** small, explicit loops (fixed keys / small tables). |
| `apply`, vectorised string ops | **No** — **row-by-row** `IF` and assignments. |
| Passing `ARRAY` into `PROCEDURE` / `FUNCTION` | **No** — **scalar** parameters only; use **globals** with a **`Pd` prefix** for a “workspace” (same idea as [NumPy-style patterns](numpy-style.md)). |
| CSV I/O, indexes, MultiIndex | **Not** in scope here — use **[files](reference/language.md)** if you need persistence. |

What you **do** have: parallel **1D** arrays as columns, **2D** arrays as a single matrix, **`FOR`** scans, **filters** (`IF` inside a loop), **aggregates** (running sums / counts), and **nested-loop joins** on keys.

### Reserved names

**`STEP`** is reserved in **`FOR`** loops — do not use **`step`** as a variable name (see [NumPy-style — reserved names](numpy-style.md#reserved-names-that-clash-with-numpy-habits)).

---

## Pattern: parallel columns (“one table”)

Think **`df['A']`** and **`df['B']`** as **`A[i]`** and **`B[i]`** for **`i <- 1 TO nRows`**.

```text
CONSTANT nRows = 4
DECLARE Dept : ARRAY[1:nRows] OF INTEGER
DECLARE Sales : ARRAY[1:nRows] OF REAL

Dept[1] <- 10
Dept[2] <- 10
Dept[3] <- 20
Sales[1] <- 100.0
Sales[2] <- 50.0
Sales[3] <- 200.0
// ... fill remaining rows
```

---

## Quick mapping table

| Pandas (rough) | Pseudocode pattern |
|----------------|-------------------|
| `df.head(k)` / `df.tail(k)` | `FOR i <- 1 TO k` … `OUTPUT … Col[i]`; tail: `FOR i <- n-k+1 TO n` |
| `df[df['x'] > t]` | Scan: `IF Col[i] > t THEN` (copy to second table, or print, or count) |
| `df['x'].sum()` / `.mean()` | Same as [NumPy-style reductions](numpy-style.md) on that column |
| `df['x'].idxmin()` (1-based here) | One pass: track **min value** and **row index** |
| `df.groupby('key')['v'].sum()` | Buckets: `FOR i` then `IF Key[i]=k THEN acc[k] <- acc[k]+V[i]` (fixed set of keys) |
| `pd.merge(left, right, on='key')` | Nested `FOR`: `IF LKey[i]=RKey[j] THEN` emit or store match |

---

## See also

- [Arrays (1D and 2D)](reference/arrays.md)  
- [NumPy-style patterns](numpy-style.md)  
- [Built-in functions](reference/builtins.md) — `LENGTH`, `TOREAL`, …  
- [Language reference](reference/language.md)
