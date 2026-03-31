<section class="feature-band" aria-labelledby="why-heading">
  <h2 id="why-heading" class="section-title">Why students choose this interpreter</h2>
  <p class="section-lead">A <strong>local</strong>, <strong>predictable</strong> runtime for the same keywords and structures you practise for papers — with a manual that reads like a real language reference.</p>
  <div class="feature-grid">
    <article class="feature-card">
      <h3 class="feature-card-title"><span class="feature-icon" aria-hidden="true">▸</span> Exam-aligned</h3>
      <p>Built around <strong>Cambridge</strong> pseudocode (e.g. <strong>CS0478</strong>, <strong>CS9618</strong>, <strong>IT9626</strong>): <code>DECLARE</code>, <code>FOR</code>/<code>NEXT</code>, <code>PROCEDURE</code>/<code>CALL</code>, arrays, files, and documented extensions where they differ from strict exam style.</p>
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

If you are studying **IGCSE Computer Science (0478 / CS0478)**, **AS & A Level Computer Science (9618 / CS9618)**, or **AS & A Level Information Technology (9626 / IT9626)**, you need to read and write **pseudocode** for algorithms, trace tables, and papers. This project gives you a **free, open-source** program that runs that style of pseudocode on your own computer: **no account**, **no browser lock-in**, and a full **tutorial** and **language reference** so you can practise the same keywords and structures you see in the syllabus.

It is ideal for **revision**, **homework**, and **experimenting** with `IF`, `FOR`, `PROCEDURE`, arrays, and files before you sit exams.

---

## Try it in thirty seconds

```bash
# Clone, build, run the hello example (from repo root)
./scripts/build.sh
./build/pseudocode run examples/hello.pseudo
```

```pseudocode
// Minimal Cambridge-style program
DECLARE Name : STRING
OUTPUT "What's your name? "
INPUT Name
OUTPUT "Hello ", Name
```

Full setup paths (CMake, PATH, troubleshooting): **[Installation guide](docs/installation.html)**.

**No local install?** **[Try online](docs/try-online.html)** — open the project in **GitHub Codespaces** or **Gitpod** and run **`./build/pseudocode repl`** in the cloud terminal (GitHub Pages itself cannot run the C interpreter in the browser).

---

## Language at a glance

The dialect follows **Cambridge** pseudocode (CS0478, CS9618, IT9626) where practical. A few **optional extensions** (for example extra ways to join strings) are documented separately; for exams, always follow your **official guide** and teacher.

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

This site is an **independent learning tool**. **Cambridge International** owns each qualification. Use the official pages for syllabus and assessment rules: [IGCSE Computer Science (0478)](https://www.cambridgeinternational.org/programmes-and-qualifications/cambridge-igcse-computer-science-0478/), [AS & A Level Computer Science (9618)](https://www.cambridgeinternational.org/programmes-and-qualifications/cambridge-international-as-and-a-level-computer-science-9618/), [AS & A Level Information Technology (9626)](https://www.cambridgeinternational.org/programmes-and-qualifications/cambridge-international-as-and-a-level-information-technology-9626/).

---

## Contributing and license

- **[Contributing](contributing.html)** — issues, pull requests, building from source.  
- **[License (MIT)](license.html)** — open source.  
- **Authors:** [`AUTHORS.md` on GitHub](https://github.com/mravariya/pseudocode/blob/main/AUTHORS.md).
