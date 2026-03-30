# Package manager (`pkg`)

The **`pkg`** program manages **optional collections** of Pseudocode source files so teachers can reuse modules across classrooms. It is intentionally smaller than **pip** or **npm**: v1.0 focuses on **local directory install**, a predictable **on-disk layout**, and a documented path toward **HTTP registries**.

**See also:** [Registry schema](../registry/registry-schema.md) · [CLI](cli.md) · [FAQ](faq.md)

---

## Synopsis

```text
pkg install <directory> [package-name]
pkg remove <package-name>
pkg list
pkg search <term>
pkg update
pkg help
```

---

## Command reference

### `pkg install <directory> [package-name]`

Copies every file whose name ends in **`.pseudo`** or **`.pseudocode`** from **`directory`** into the package store.

- If **`package-name`** is omitted, the **last path component** (directory or folder name) is used—for example `pkg install ./libs/foo` installs into package **`foo`**.
- Existing files in the destination package folder may be **overwritten** when names collide.

**Example:**

```bash
pkg install ./course-lib cs9618-lib
```

### `pkg remove <package-name>`

Deletes the installed package directory. On Unix this runs `rm -rf` on the package path; on Windows the PowerShell helper uses `rmdir /S /Q`. **This is destructive**—there is no recycle-bin integration.

### `pkg list`

Lists subdirectory names under the package root (one line per installed package). If nothing is installed, prints a short notice.

### `pkg search <term>`

v1.0 prints a **placeholder** message. Future versions may query a local index or remote registry. Until then, use shell tools:

```bash
ls ~/.pseudocode/packages | grep term
```

### `pkg update`

Reserved for **registry refresh**. Currently prints instructions to configure **`registry.txt`** and explains that HTTP download is not yet wired.

### `pkg help`

Usage summary and pointer to **`~/.pseudocode`** layout.

---

## Directory layout

After running **`pkg`**, the following tree is used (Unix paths; Windows uses `%USERPROFILE%`):

```text
~/.pseudocode/
  packages/
    <package-name>/
      *.pseudo
      *.pseudocode
  registry.txt          # optional: one base URL per line (future)
  cache/                # reserved for future tarballs
```

Print the resolved root with:

```bash
pkg help
```

---

## Including installed code

The interpreter **does not** yet implement **`INCLUDE "path"`** or **`IMPORT`**. Practical options today:

1. **Copy** the needed `.pseudo` file into your project.  
2. **Concatenate** sources in a build script before running.  
3. **Teach** students to paste small library snippets (acceptable for exam-sized tasks).

An `INCLUDE` directive is the natural follow-up feature once package paths are stable.

---

## Security and trust

Treat **`pkg install`** like copying arbitrary code: only install packages from **teachers you trust**. There is no signature verification in v1.0.

---

## Online registry (design)

The file [registry/registry-schema.md](../registry/registry-schema.md) describes JSON index format, tarball layout, and caching. When implemented, **`pkg update`** will fetch indices and **`pkg install name@version`** will download into **`cache/`** then extract into **`packages/`**.

---

## Building `pkg`

Built together with **`pseudocode`** via CMake or **`scripts/build.sh`**. The binary is **`pkg`** (no extension on Unix, **`pkg.exe`** on Windows).

---

## Platform notes

**POSIX** (`dirent`): macOS, Linux, MSYS2 **MinGW** — fully supported.

**MSVC-only** Windows builds may need extra work for directory enumeration; prefer **MSYS2** for `pkg` until unified.
