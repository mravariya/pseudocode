# Examples catalogue

This page lists **curated examples** that ship with the repository and **patterns** you can copy into your own `.pseudo` files. Each entry points to the **full listing** or contains a **drop-in** fragment.

**Run any file** with:

```bash
pseudocode path/to/file.pseudo
```

**See also:** [Tutorial](tutorial/index.md) · [Built-in functions](reference/builtins.md) · [Language reference](reference/language.md)

---

## 1. Bundled programs (`examples/`)

See **[`examples/README.md`](../examples/README.md)** for the full layout. Shortcuts at the repo root (easy to type in docs):

| File | Topics demonstrated |
|------|---------------------|
| **`hello.pseudo`** | `DECLARE`, assignment, `LENGTH`, `OUTPUT`, string literal |
| **`loop.pseudo`** | `FOR` … `NEXT`, integer variable |
| **`function.pseudo`** | `FUNCTION` / `RETURNS` / `RETURN`, call inside expression |

### 1.1 `examples/vanilla-pseudocode/` (language tour)

| File | Topics |
|------|--------|
| **`variables-and-types.pseudo`** | `DECLARE` for `INTEGER`, `REAL`, `STRING`, `CHAR`, `BOOLEAN`, `DATE` |
| **`constants.pseudo`** | `CONSTANT`, mixed `OUTPUT` |
| **`arithmetic-div-mod.pseudo`** | `DIV`, `MOD`, `/` |
| **`boolean-logic.pseudo`** | `AND`, `OR`, `NOT` |
| **`for-loop-basic.pseudo`** / **`for-loop-step.pseudo`** | `FOR` … `NEXT`, `STEP` |
| **`while-loop.pseudo`** | `WHILE` … `ENDWHILE` |
| **`repeat-until.pseudo`** | `REPEAT` … `UNTIL` |
| **`if-else-endif.pseudo`** | nested `IF` |
| **`case-of.pseudo`** | `CASE OF` … `ENDCASE` with ranges and `OTHERWISE` |
| **`procedure-byval.pseudo`** / **`procedure-byref.pseudo`** | `PROCEDURE`, `CALL`, `BYVAL` / `BYREF` |
| **`function-return.pseudo`** | `FUNCTION`, `RETURN`, nested calls |
| **`array-1d.pseudo`** / **`array-2d.pseudo`** | `ARRAY`, `LENGTH` |
| **`string-basics.pseudo`** | `&`, `MID`, `RIGHT`, `CONCAT`, `UPPERCASE` |
| **`nested-loops.pseudo`** | nested `FOR` |
| **`file-read-text.pseudo`** | `OPENFILE`, `READFILE`, `EOF`, `CLOSEFILE` (needs `data/sample-in.txt`; run from repo root) |

### 1.2 `examples/stdlib-demos/` (built-ins / pkg topics)

These exercise **built-in** functions documented under **`docs/stdlib/`** (the **`pkg install math`** style catalog only ships stub files; you do not need `pkg` to run these demos).

| Path | Topics |
|------|--------|
| **`stdlib-demos/math/math-all-builtins.pseudo`** | `SQRT`, trig, `HYPOT`, `POW`, `FMOD`, `LOG`, `ROUND`, `PI`, `EULER`, … |
| **`stdlib-demos/random/random-seed-and-bounds.pseudo`** | `SEED`, `RANDOM`, `RANDINT` |
| **`stdlib-demos/time/time-and-clock.pseudo`** | `TIME`, `CLOCKMS`, `NOWSTRING` |
| **`stdlib-demos/os/getenv-demo.pseudo`** | `GETENV` |
| **`stdlib-demos/system/system-true.pseudo`** | `SYSTEM` (Unix `true`; adjust on Windows) |

### 1.3 `examples/numpy-style/` (NumPy-like patterns)

These programs mirror common **NumPy** workflows using **loops** and **fixed** arrays (the language does not pass `ARRAY` into procedures). See **[NumPy-style patterns](numpy-style.md)** for the mapping table and teaching notes.

| Path | Topics |
|------|--------|
| **`numpy-style/01-sum-and-mean.pseudo`** | Sum and mean with `LENGTH`, `TOREAL` |
| **`numpy-style/02-fill-zeros-ones-linspace.pseudo`** | Fill with zeros/ones; linspace-style spacing |
| **`numpy-style/03-dot-product.pseudo`** | Dot product |
| **`numpy-style/04-max-and-argmax.pseudo`** | Max and 1-based argmax |
| **`numpy-style/05-matmul-vector.pseudo`** | Small matrix × vector |
| **`numpy-style/06-add-scalar-to-array.pseudo`** | Add scalar element-wise |
| **`numpy-style/07-np-workspace-functions.pseudo`** | Prefixed globals + scalar-return functions |

### 1.4 `examples/pandas-style/` (Pandas-like patterns)

These programs model **tables** as **parallel 1D arrays** (one array per column). There is no `DataFrame` type. See **[Pandas-style patterns](pandas-style.md)** for the mapping table.

| Path | Topics |
|------|--------|
| **`pandas-style/01-parallel-columns.pseudo`** | Row index + multiple columns |
| **`pandas-style/02-head-and-tail.pseudo`** | First / last *k* rows |
| **`pandas-style/03-filter-rows.pseudo`** | Filter by condition; count rows |
| **`pandas-style/04-column-min-max-mean.pseudo`** | One-pass column stats |
| **`pandas-style/05-idxmin-row.pseudo`** | Row index of minimum (1-based) |
| **`pandas-style/06-groupby-sum.pseudo`** | Sum by category key |
| **`pandas-style/07-merge-inner-on-key.pseudo`** | Inner join with nested loops |
| **`pandas-style/08-pd-workspace-globals.pseudo`** | `Pd` prefix workspace + helpers |

### 1.5 `examples/matplotlib-style/` (Matplotlib-like patterns)

There is **no** plotting API. Programs **compute series**, **print** tables or **ASCII charts**, or **write** a simple text file for use with Python (or other tools). See **[Matplotlib-style patterns](matplotlib-style.md)**.

| Path | Topics |
|------|--------|
| **`matplotlib-style/01-line-y-from-x.pseudo`** | Sample **`y = f(x)`** (sine) into arrays |
| **`matplotlib-style/02-print-xy-table.pseudo`** | Console **(x, y)** table |
| **`matplotlib-style/03-write-xy-pairs-file.pseudo`** | **`WRITEFILE`** interleaved x,y lines under **`out/`** |
| **`matplotlib-style/04-ascii-column-chart.pseudo`** | Text column chart with `*` |
| **`matplotlib-style/05-ascii-histogram-bins.pseudo`** | Bin counts + `#` bars |
| **`matplotlib-style/06-two-series-console.pseudo`** | Two labelled blocks (subplot idea) |
| **`matplotlib-style/07-mpl-workspace-globals.pseudo`** | **`Mpl`** globals + **`CALL`** print procedure |

**Suggested exercise:** modify `hello.pseudo` to print your name and the **length** of a sentence you choose, then step through **`vanilla-pseudocode/`** in the table order.

---

## 2. Beginner patterns

### 2.1 Arithmetic and output

```text
DECLARE a : INTEGER
DECLARE b : INTEGER
a <- 15
b <- 4
OUTPUT "sum ", a + b
OUTPUT "div ", a DIV b
OUTPUT "mod ", a MOD b
OUTPUT "real ", a / b
```

### 2.2 Boolean logic

```text
DECLARE ok : BOOLEAN
ok <- TRUE AND NOT FALSE
OUTPUT ok
```

---

## 3. Control flow

### 3.1 Nested `IF`

```text
DECLARE x : INTEGER
x <- 10
IF x > 0 THEN
  IF x < 100 THEN
    OUTPUT "between"
  ELSE
    OUTPUT "large"
  ENDIF
ELSE
  OUTPUT "non-positive"
ENDIF
```

### 3.2 `CASE` with ranges (integer)

```text
DECLARE n : INTEGER
n <- 7
CASE OF n
  1 TO 5 : OUTPUT "low"
  6 TO 10 : OUTPUT "mid"
  OTHERWISE : OUTPUT "other"
ENDCASE
```

---

## 4. Subprograms

### 4.1 Procedure with `BYREF`

```text
PROCEDURE Inc(BYREF k : INTEGER)
  k <- k + 1
ENDPROCEDURE

DECLARE v : INTEGER
v <- 1
CALL Inc(v)
OUTPUT v
```

### 4.2 Function with multiple parameters

```text
FUNCTION Max(a : INTEGER, b : INTEGER) RETURNS INTEGER
IF a > b THEN
  RETURN a
ELSE
  RETURN b
ENDIF
ENDFUNCTION

OUTPUT Max(3, 9)
```

---

## 5. Arrays

### 5.1 1D — running total

```text
DECLARE T : ARRAY[1:5] OF INTEGER
DECLARE i : INTEGER
DECLARE sum : INTEGER
sum <- 0
FOR i <- 1 TO 5
  T[i] <- i * i
  sum <- sum + T[i]
NEXT i
OUTPUT sum
```

### 5.2 2D — trace

```text
DECLARE G : ARRAY[1:2,1:3] OF INTEGER
DECLARE r : INTEGER
DECLARE c : INTEGER
FOR r <- 1 TO 2
  FOR c <- 1 TO 3
    G[r,c] <- r * 10 + c
  NEXT c
NEXT r
OUTPUT G[2,3]
```

---

## 6. Strings and built-ins

```text
DECLARE s : STRING
DECLARE t : STRING
s <- "alpha"
t <- UPPERCASE(s & "BETA")
OUTPUT t
OUTPUT MID(t, 1, 5)
```

---

## 7. Files (text)

Requires an existing **`in.txt`** in the working directory:

```text
DECLARE line : STRING
OPENFILE "in.txt" FOR read
WHILE NOT EOF("in.txt")
  READFILE "in.txt", line
  OUTPUT line
ENDWHILE
CLOSEFILE "in.txt"
```

---

## 8. Advanced projects (assignment ideas)

| Project | Concepts |
|---------|----------|
| **Quiz program** | `REPEAT`, `INPUT`, `CASE`, score counter |
| **Linear search** | `ARRAY`, `FOR`, `IF` |
| **Simple menu** | `WHILE`, `CASE`, procedures |
| **Copy file** | `OPENFILE`, `READFILE`, `WRITEFILE`, `EOF` |

---

## 9. Contributing examples

Add new `.pseudo` files under **`examples/vanilla-pseudocode/`** (language) or **`examples/stdlib-demos/<topic>/`** (built-in demos), keep them **self-contained** (or document required input files and working directory), update **`examples/README.md`** if you add a new subfolder, and extend **this catalogue** in your pull request. Short **smoke-test** samples may still live next to **`hello.pseudo`** at **`examples/`** root.
