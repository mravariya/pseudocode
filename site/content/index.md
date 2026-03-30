## Language at a glance

The dialect follows **Cambridge International AS & A Level Computer Science (9618)** pseudocode where practical. The interpreter adds a few **teaching-friendly extensions** (for example extra string concatenation forms); for final exams, follow your official guide if it differs.

| Types | `INTEGER` · `REAL` · `STRING` · `CHAR` · `BOOLEAN` · `DATE` |
|-------|--------------------------------------------------------------|
| Control flow | `IF` / `ENDIF`, `CASE` / `ENDCASE`, `FOR` / `NEXT`, `WHILE` / `ENDWHILE`, `REPEAT` / `UNTIL` |
| Subprograms | `PROCEDURE` / `CALL`, `FUNCTION` / `RETURN`, `BYVAL` / `BYREF` |
| Data structures | **1D** and **2D** arrays (inclusive bounds); no classes or `TYPE` records in v1.0 |
| Files | `OPENFILE`, `READFILE`, `WRITEFILE`, `CLOSEFILE`, `EOF` |
| Assignment | `<-` or `←` |

Full rules: **[Documentation home](docs/index.html)** → Language reference, strings, arrays, and built-ins.

---

## Code snippets

### Hello and I/O

```text
DECLARE Name : STRING
OUTPUT "Name? "
INPUT Name
OUTPUT "Hello ", Name
```

### Loop and array

```text
DECLARE I : INTEGER
DECLARE A : ARRAY[1 : 5] OF INTEGER
FOR I <- 1 TO 5
  A[I] <- I * 2
NEXT I
```

---

## Documentation and tutorial

| Resource | Description |
|----------|-------------|
| **[Tutorial](docs/tutorial/index.html)** | Step-by-step introduction (~30 minutes) |
| **[Installation](docs/installation.html)** | macOS, Linux, Windows, PATH, troubleshooting |
| **[Getting started](docs/getting-started.html)** | Minimal commands |
| **[FAQ](docs/faq.html)** | Exams, Python comparison, common questions |

---

## Subject specification (9618)

This project is a **tool for learning and practice**, not an official Cambridge product. Assessment is set by **Cambridge International**. Use the qualification page linked in the site header for syllabus and assessment details.

---

## Contributing and license

- **[Contributing](contributing.html)** — how to report issues, submit PRs, and build the project.  
- **[License (MIT)](license.html)** — open source terms.  
- **Authors:** see **`AUTHORS.md`** on [GitHub](https://github.com/mravariya/9618-Computer-Science-Pseudocode-Interpreter/blob/main/AUTHORS.md).
