# NumPy-style patterns in Pseudocode

This interpreter is **not** Python and has **no** `import numpy`. This page is a **conceptual bridge**: how common **NumPy** ideas map to **Cambridge-style** pseudocode with **fixed-size** arrays and **loops**.

**Runnable code:** [`examples/numpy-style/`](../examples/numpy-style/) · **Examples catalogue:** [examples.md](examples.md)

---

## What is missing compared to NumPy

| NumPy idea | In this Pseudocode |
|------------|-------------------|
| Dynamic shape, `reshape`, `ravel` | **No** — bounds are fixed at `DECLARE`. |
| Slicing `a[2:5]`, fancy indexing | **No** — use loops over index ranges. |
| `import`, `np.array([...])` | **No** — fill arrays with `FOR` or assignment. |
| Vectorised `a + b`, `a * 3` | **No** — write **element-wise** `FOR` loops (see examples). |
| Passing arrays into `PROCEDURE` / `FUNCTION` | **No** — parameters are **scalar types** only (`INTEGER`, `REAL`, …). |
| `linalg`, FFT, masked arrays | **Not** in scope here. |

What you **do** have: **1D / 2D** arrays, **`LENGTH`**, arithmetic, **`FOR`**, **`FUNCTION` … RETURNS`** for scalars, and **well-chosen global names** as a poor-person’s “module” (see below).

### Reserved names that clash with NumPy habits

**`STEP`** is a keyword in **`FOR`** loops (`FOR i <- 1 TO 10 STEP 2`). You **cannot** use **`step`** as a variable name — use e.g. **`delta`** or **`h`** for a linspace increment.

---

## Pattern: a tiny “`np` workspace”

Because functions cannot take `ARRAY` parameters, teaching examples often use **one shared length** and **prefixed global arrays** (here **`Np`**):

```text
CONSTANT NpN = 8
DECLARE NpX : ARRAY[1:NpN] OF REAL

FUNCTION NpSumX() RETURNS REAL
  DECLARE i : INTEGER
  DECLARE s : REAL
  s <- 0.0
  FOR i <- 1 TO NpN
    s <- s + NpX[i]
  NEXT i
  RETURN s
ENDFUNCTION
```

Rename **`NpX` / `NpN` / `NpSumX`** to match your classroom, or **copy the loop** inline if you prefer no globals.

---

## Quick mapping table

| NumPy (rough) | Pseudocode pattern |
|---------------|-------------------|
| `np.sum(a)` | Loop: `s <- s + a[i]` |
| `np.mean(a)` | `sum / LENGTH(a)` or `sum / n` (**use `REAL`** for division) |
| `np.max(a)` / `np.argmax(a)` | Single pass: track `best` and optionally `bestIdx` |
| `np.zeros(n)` / `np.ones(n)` | `FOR i <- lo TO hi : a[i] <- 0` or `1` |
| `np.dot(a, b)` | `FOR i : s <- s + a[i] * b[i]` (same bounds) |
| `a + 3` (broadcast) | `FOR i : a[i] <- a[i] + 3` |
| `M @ v` (small matrix × vector) | Double loop: `y[r] <- y[r] + M[r,c] * v[c]` |
| `np.linspace(a,b,n)` | `FOR i <- 1 TO n : x[i] <- a0 + TOREAL(i-1)*delta` with `delta <- (b-a)/TOREAL(n-1)` (avoid naming the increment **`step`** — reserved) |

---

## See also

- [Matplotlib-style patterns](matplotlib-style.md) — series and text “plots”  
- [Pandas-style patterns](pandas-style.md) — tables as parallel columns  
- [Arrays (1D and 2D)](reference/arrays.md)  
- [Built-in functions](reference/builtins.md) — `LENGTH`, maths builtins in [stdlib: math](stdlib/math.md)  
- [Language reference](reference/language.md) — `DECLARE`, `FOR`, `FUNCTION`
