# Syntax quick reference

**One-page cheat sheet.** For full semantics, precedence rules, and edge cases, read the [**Language reference**](reference/language.md). For built-ins, see [**Built-in functions**](reference/builtins.md).

---

## Literals and comments

| Item | Form |
|------|------|
| Integer | `123`, `-7` |
| Real | `3.14`, `0.5` |
| String | `"text"`, escapes `\\` `\"` `\n` `\t` |
| Char | `'x'` |
| Boolean | `TRUE`, `FALSE` |
| Date (token) | `02/01/2005` (see language ref) |
| Comment | `//` to end of line |

---

## Declaration

```text
DECLARE x : INTEGER
DECLARE a : ARRAY[1:n] OF REAL
DECLARE m : ARRAY[r1:r2,c1:c2] OF CHAR
CONSTANT Pi = 3.14159
```

---

## Assignment

```text
x <- expr
x ← expr
```

---

## Operators (precedence summary)

`*` `/` `DIV` `MOD` → `+` `-` (two **STRING**s → concat) → `&` → comparisons → `AND` → `OR`  
Unary: `-` `NOT`  

**Extension:** on `←` / `<-` RHS (and `CONSTANT` / `RETURN`), `expr1 , expr2 , …` joins **STRING** segments. Full guide: [Strings and concatenation](reference/strings-and-concatenation.md).

---

## Control flow

```text
IF cond THEN ... [ELSE ...] ENDIF

CASE OF v
  val : ...
  lo TO hi : ...
  OTHERWISE : ...
ENDCASE

FOR i <- a TO b [STEP s] ... NEXT i

WHILE cond ... ENDWHILE

REPEAT ... UNTIL cond
```

---

## Subprograms

```text
PROCEDURE P([BYREF] a : T, ...) ... ENDPROCEDURE
CALL P(args)

FUNCTION F(...) RETURNS T ... RETURN expr ENDFUNCTION
// use F(args) in expressions — no CALL
```

---

## I/O and files

```text
INPUT x
OUTPUT a, b, c

OPENFILE path FOR read|write|append|random
READFILE path, var
WRITEFILE path, expr
CLOSEFILE path
EOF(path)
```

---

## Built-ins (names only)

`LENGTH` · `MID` / `SUBSTRING` · `RIGHT` · `CONCAT` · `INT` / `TOINTEGER` · `TOREAL` · `ROUND` · `RAND` / `RANDOM` · `UCASE` · `LCASE` · `UPPERCASE` · `LOWERCASE` · `EOF`

---

## Documentation map

| Need | Document |
|------|----------|
| Learn step by step | [Tutorial](tutorial/index.md) |
| Every rule | [Language reference](reference/language.md) |
| Each built-in | [Built-in functions](reference/builtins.md) |
| Run / check / REPL | [CLI](cli.md) |
