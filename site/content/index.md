<section class="feature-band" aria-labelledby="why-heading">
  <h2 id="why-heading" class="section-title">Why students choose this interpreter</h2>
  <p class="section-lead">A <strong>local</strong>, <strong>predictable</strong> runtime for the same keywords and structures you practise for papers — with a manual that reads like a real language reference.</p>
  <div class="feature-grid">
    <article class="feature-card">
      <h3 class="feature-card-title"><span class="feature-icon" aria-hidden="true">▸</span> Exam-aligned</h3>
      <p>Built around <strong>Cambridge 9618</strong> pseudocode: <code>DECLARE</code>, <code>FOR</code>/<code>NEXT</code>, <code>PROCEDURE</code>/<code>CALL</code>, arrays, files, and documented extensions where they differ from strict exam style.</p>
    </article>
    <article class="feature-card">
      <h3 class="feature-card-title"><span class="feature-icon" aria-hidden="true">▸</span> Run anywhere</h3>
      <p>Single C11 binary — <strong>macOS</strong>, <strong>Linux</strong>, and <strong>Windows</strong> (MSYS2 / MinGW). No sign-up, no cloud dependency for practice sessions.</p>
    </article>
    <article class="feature-card">
      <h3 class="feature-card-title"><span class="feature-icon" aria-hidden="true">▸</span> Professional workflow</h3>
      <p><strong>Syntax check</strong> for CI, <strong>REPL</strong> for quick experiments, optional <strong>VS Code</strong> integration, and a <strong>package manager</strong> (<code>pkg</code>) for optional stub libraries.</p>
    </article>
  </div>
</section>

## Why use this interpreter?

If you are studying **Cambridge International AS & A Level Computer Science (9618)**, you need to read and write **pseudocode** for algorithms, trace tables, and papers. This project gives you a **free, open-source** program that runs that style of pseudocode on your own computer: **no account**, **no browser lock-in**, and a full **tutorial** and **language reference** so you can practise the same keywords and structures you see in the syllabus.

It is ideal for **revision**, **homework**, and **experimenting** with `IF`, `FOR`, `PROCEDURE`, arrays, and files before you sit exams.

---

## Try it in thirty seconds

```bash
# Clone, build, run the hello example (from repo root)
./scripts/build.sh
./build/pseudocode run examples/hello.pseudo
```

```pseudocode
// Minimal 9618-style program
DECLARE Name : STRING
OUTPUT "What's your name? "
INPUT Name
OUTPUT "Hello ", Name
```

Full setup paths (CMake, PATH, troubleshooting): **[Installation guide](docs/installation.html)**.

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
| Maths and tools | Built-ins like `SQRT`, `RANDINT`, `TIME`, `GETENV` — see **[Standard library topics](docs/stdlib/index.html)** |

Complete rules: **[Documentation home](docs/index.html)** — language reference, strings, arrays, built-ins, CLI, and `pkg`.

---

## Example: loop and array

```pseudocode
DECLARE I : INTEGER
DECLARE A : ARRAY[1 : 5] OF INTEGER
FOR I <- 1 TO 5
  A[I] <- I * 2
NEXT I
OUTPUT A[3]   // expect 6
```

---

## Learning path

| Step | Resource |
|------|----------|
| 1 | **[Tutorial — Pseudocode in 30 minutes](docs/tutorial/index.html)** |
| 2 | **[Getting started](docs/getting-started.html)** — commands cheat sheet |
| 3 | **[Language reference](docs/reference/language.html)** — full syntax |
| 4 | **[Built-in functions](docs/reference/builtins.html)** and **[Stdlib (Python-style)](docs/stdlib/index.html)** |
| 5 | **[FAQ](docs/faq.html)** — exams, Python comparison, common errors |

---

## Syllabus and disclaimer

This site is an **independent learning tool**. **Cambridge International** owns the **9618** qualification. Use the [official qualification page](https://www.cambridgeinternational.org/programmes-and-qualifications/cambridge-international-as-and-a-level-computer-science-9618/) for syllabus and assessment rules.

---

## Contributing and license

- **[Contributing](contributing.html)** — issues, pull requests, building from source.  
- **[License (MIT)](license.html)** — open source.  
- **Authors:** [`AUTHORS.md` on GitHub](https://github.com/mravariya/pseudocode/blob/main/AUTHORS.md).
