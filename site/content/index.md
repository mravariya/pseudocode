## Why use this interpreter?

If you are studying **Cambridge International AS & A Level Computer Science (9618)**, you need to read and write **pseudocode** for algorithms, trace tables, and papers. This project gives you a **free, open-source** program that runs that style of pseudocode on your own computer: **no account**, **no browser lock-in**, and a full **tutorial** and **language reference** so you can practise the same keywords and structures you see in the syllabus.

It is ideal for **revision**, **homework**, and **experimenting** with `IF`, `FOR`, `PROCEDURE`, arrays, and files before you sit exams.

---

## Language at a glance

The dialect follows **Cambridge 9618** pseudocode where practical. A few **optional extensions** (for example extra ways to join strings) are documented separately; for exams, always follow your **official guide** and teacher.

| Types | `INTEGER` · `REAL` · `STRING` · `CHAR` · `BOOLEAN` · `DATE` |
|-------|--------------------------------------------------------------|
| Control flow | `IF` / `ENDIF`, `CASE` / `ENDCASE`, `FOR` / `NEXT`, `WHILE` / `ENDWHILE`, `REPEAT` / `UNTIL` |
| Subprograms | `PROCEDURE` / `CALL`, `FUNCTION` / `RETURN`, `BYVAL` / `BYREF` |
| Data structures | **1D** and **2D** arrays (inclusive bounds); no classes or `TYPE` records in v1.0 |
| Files | `OPENFILE`, `READFILE`, `WRITEFILE`, `CLOSEFILE`, `EOF` |
| Assignment | `<-` or `←` |

Full rules: **[Documentation home](docs/index.html)** — language reference, strings, arrays, and built-ins.

---

## Example pseudocode (copy and run)

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

## Learn more

| Resource | Description |
|----------|-------------|
| **[Tutorial](docs/tutorial/index.html)** | Hands-on introduction (~30 minutes) |
| **[Installation](docs/installation.html)** | macOS, Linux, Windows, PATH |
| **[Getting started](docs/getting-started.html)** | Minimal commands |
| **[FAQ](docs/faq.html)** | Exams, comparison with Python, common questions |

---

## Syllabus and disclaimer

This site is an **independent learning tool**. **Cambridge International** owns the **9618** qualification. Use the [official qualification page](https://www.cambridgeinternational.org/programmes-and-qualifications/cambridge-international-as-and-a-level-computer-science-9618/) for syllabus and assessment rules.

---

## Contributing and license

- **[Contributing](contributing.html)** — issues, pull requests, building from source.  
- **[License (MIT)](license.html)** — open source.  
- **Authors:** [`AUTHORS.md` on GitHub](https://github.com/mravariya/pseudocode/blob/main/AUTHORS.md).
