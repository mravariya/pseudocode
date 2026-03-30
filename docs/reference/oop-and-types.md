# OOP, classes, and user-defined types

What the **9618 Pseudocode** syllabus describes for **records**, **pointers**, and **object-oriented** ideas — and what this **interpreter implements today**. Use this page when searching for **class**, **object**, **TYPE**, **ENDTYPE**, **RECORD**, **OOP**, or **pointer**.

**See also:** [Language reference — §7](language.md#7-user-defined-types-record-pointers-oop) · [Implementation notes](../implementation.md)

---

## Quick answers

| Question | In this interpreter (v1.0.x) |
|----------|------------------------------|
| **`CLASS` / objects like Python or Java? | **No** — not implemented. |
| **`TYPE` … `ENDTYPE` records? | **No** — not implemented. |
| **Pointers / `^` / `NEW`? | **No** — not implemented. |
| **What should I use instead? | Scalars, **arrays** (1D/2D), **procedures** and **functions** with explicit parameters. |

---

## Table of contents

1. [Syllabus vs this project](#1-syllabus-vs-this-project)
2. [Why teachers ask about OOP](#2-why-teachers-ask-about-oop)
3. [Modelling data without RECORD](#3-modelling-data-without-record)
4. [Roadmap (informal)](#4-roadmap-informal)
5. [Comparison to Python (mental model)](#5-comparison-to-python-mental-model)

---

## 1. Syllabus vs this project

The Cambridge **Pseudocode Guide** (for **9618**) describes several features that go beyond simple scalars and arrays, including:

- **User-defined composite types** (often written with `TYPE` / `ENDTYPE` and field names).  
- **Pointers** and dynamic structures (lists, trees) in some presentations.  
- **Object-oriented** pseudocode in some editions or extension material (classes, methods, inheritance — wording varies by year and handout).

The **reference interpreter** in this repository targets a **portable subset** used in most classroom papers: control flow, subprograms, files, **1D/2D arrays**, and built-ins. **§7** of the [language reference](language.md#7-user-defined-types-record-pointers-oop) states explicitly that **RECORD**, **pointers**, and **OOP** are **not implemented** in v1.0.

**Search terms:** not implemented, future extension, v1.0 scope.

---

## 2. Why teachers ask about OOP

Exam papers sometimes use **abstract** data descriptions (“a student record has name, id, …”) even when the answer is expected as parallel arrays or a fixed layout. The **interpreter** does not provide a `class` keyword or method dispatch; you express structure with:

- Several **variables** or **array columns** with comments, or  
- **2D** tables where rows are entities and columns are fields (by convention).

---

## 3. Modelling data without RECORD

**Parallel arrays** (same index = same entity):

```text
// Concept: student names and scores — by position
DECLARE Names  : ARRAY[1 : 30] OF STRING
DECLARE Scores : ARRAY[1 : 30] OF INTEGER
```

**2D grid** for tabular data:

```text
DECLARE Table : ARRAY[1 : 10, 1 : 5] OF STRING
```

**Behaviour** = **PROCEDURE** / **FUNCTION** with explicit arguments (no `self`).

**Search terms:** modelling, struct without struct, parallel arrays.

---

## 4. Roadmap (informal)

A future major version might add:

- `TYPE` … `ENDTYPE` with fixed fields.  
- Optional pointer / dynamic features (high complexity for a teaching interpreter).  
- OOP-style syntax only if it stays aligned with published 9618 pseudocode.

There is **no** committed release date in this documentation set; watch the repository and `PC_VERSION_STRING` in the source.

---

## 5. Comparison to Python (mental model)

| Python idea | This Pseudocode interpreter |
|-------------|----------------------------|
| `class` with attributes | Not available — use variables / arrays. |
| `obj.method()` | Use `CALL Proc(objFields…)` or free functions. |
| `list` of dicts | Use parallel arrays or 2D array conventions. |
| `__init__` | Use a **procedure** that initialises globals or passed arrays. |

This table is for **intuition** only; exam answers must follow **9618** style, not Python.

---

*Index keywords:* `class`, `object`, `OOP`, `TYPE`, `ENDTYPE`, `RECORD`, `pointer`, `NEW`, `not implemented`, `9618`.
