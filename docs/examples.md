# Examples catalogue

This page lists **curated examples** that ship with the repository and **patterns** you can copy into your own `.pseudo` files. Each entry points to the **full listing** or contains a **drop-in** fragment.

**Run any file** with:

```bash
pseudocode path/to/file.pseudo
```

**See also:** [Tutorial](tutorial/index.md) · [Built-in functions](reference/builtins.md) · [Language reference](reference/language.md)

---

## 1. Bundled programs (`examples/`)

| File | Topics demonstrated |
|------|---------------------|
| **`hello.pseudo`** | `DECLARE`, assignment, `LENGTH`, `OUTPUT`, string literal |
| **`loop.pseudo`** | `FOR` … `NEXT`, integer variable |
| **`function.pseudo`** | `FUNCTION` / `RETURNS` / `RETURN`, call inside expression |

**Suggested exercise:** modify `hello.pseudo` to print your name and the **length** of a sentence you choose.

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

Add a new `.pseudo` file under **`examples/`**, keep it **self-contained** (or document required input files), and mention it in this catalogue in your pull request.
