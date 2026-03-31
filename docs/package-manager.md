# Package manager (`pkg`)

The **`pkg`** program manages **optional collections** of Pseudocode source files so teachers can reuse modules across classrooms. It is intentionally smaller than **pip** or **npm**: v1.0 focuses on **local directory install**, an **optional catalog** of stub packages shipped with the repository, a predictable **on-disk layout**, and a documented path toward **HTTP registries**.

**Version:** **`pkg version`**, **`pkg -v`**, and **`pkg --version`** print the same release string as **`pseudocode version`** (shared `PC_VERSION_STRING` / `PC_PKG_VERSION_STRING`).

**See also:** [Installation — both binaries](installation.md#2-what-you-are-installing) · [Package managers — Homebrew](package-managers.md) · [Standard library topics (Python-style)](stdlib/index.md) · [Registry schema](../registry/registry-schema.md) · [CLI](cli.md) · [FAQ](faq.md)

---

## Installing `pkg`

You do **not** install `pkg` separately. It is built **together** with **`pseudocode`** from the same repository:

- After **`./scripts/build.sh`** or **CMake**, you get **`build/pkg`** (or **`build/pkg.exe`**) next to **`build/pseudocode`**.
- Put **both** on your **`PATH`** (see [Installing Pseudocode](installation.md)) so you can run `pkg` from any directory.

Quick check:

```bash
pkg version
pkg help
```

---

## How to use `pkg` (quick start)

1. **See what the catalog offers** (from the repo root, or with `PSEUDOCODE_STDLIB` set):  
   `pkg available`
2. **Install an optional bundle** by name (e.g. stub files for **math**, **random**, …):  
   `pkg install math` or the short form **`pkg -i math`**
3. **Install from a folder** you trust (copies all `*.pseudo` / `*.pseudocode` into a named package):  
   `pkg install /path/to/folder my-lib`
4. **List what is installed** under your user directory:  
   `pkg list`
5. **Remove a package**:  
   `pkg remove my-lib`

Installed files live under **`~/.pseudocode/packages/`** (Unix) or **`%USERPROFILE%\.pseudocode\packages\`** (Windows). The interpreter does not auto-load them yet — copy or merge snippets into your program, or see [Including installed code](#including-installed-code).

Full install and PATH steps: **[Installation](installation.md)** (see **[§2.2 One-command install scripts](installation.md#22-one-command-install-scripts)**).

---

## Synopsis (all commands)

```text
pkg install <directory> [package-name]    # local folder
pkg install <catalog-name>                # e.g. math, random (from stdlib catalog)
pkg -i <catalog-name>                     # same as: pkg install <catalog-name>
pkg --install <catalog-name>              # same as above
pkg available                             # list catalog package names
pkg remove <package-name>
pkg list
pkg search <term>
pkg update
pkg version | -v | --version
pkg help | -h | --help
```

---

## Commands: short and full forms

Every subcommand is listed here with **equivalent** spellings where aliases exist.

| Purpose | Full command | Short / alias |
|---------|----------------|---------------|
| Show usage | `pkg help` | `pkg -h`, `pkg --help` |
| Print toolkit version | `pkg version` | `pkg -v`, `pkg --version` |
| Install from catalog name | `pkg install NAME` | `pkg -i NAME`, `pkg --install NAME` |
| Install from local directory | `pkg install DIR [NAME]` | *(no short form — use full `install`)* |
| List catalog package names | `pkg available` | *(no alias)* |
| List installed packages | `pkg list` | *(no alias)* |
| Remove installed package | `pkg remove NAME` | *(no alias)* |
| Search hint (placeholder) | `pkg search TERM` | *(no alias)* |
| Registry refresh (future) | `pkg update` | *(no alias)* |

**Mnemonic:** **`-i`** = **i**nstall (catalog or name only when not a directory path).

---

## Command reference

### `pkg install` (local directory)

If the first argument is an **existing directory** on disk, **`pkg`** copies every file whose name ends in **`.pseudo`** or **`.pseudocode`** from that folder into the package store.

- If **`package-name`** is omitted, the **last path component** (directory or folder name) is used—for example `pkg install ./libs/foo` installs into package **`foo`**.
- Existing files in the destination package folder may be **overwritten** when names collide.

**Example:**

```bash
pkg install ./course-lib cs9618-lib
```

### `pkg install` (catalog / optional bundles)

If the first argument is **not** a directory, **`pkg`** looks for a **catalog root** directory containing subfolders (one per package name). Search order:

1. Environment variable **`PSEUDOCODE_STDLIB`** (path to the catalog root, e.g. `.../stdlib-packages`).
2. Compile-time **`PC_BUILTIN_STDLIB_PACKAGES`** (set by CMake / `scripts/build.sh` to the source-tree `stdlib-packages/`).
3. **`./stdlib-packages`** or **`../stdlib-packages`** relative to the **current working directory**.

It then runs the same copy step as a local install from **`<catalog>/<name>/`**.

Bundled catalog names in this repository: **`math`**, **`random`**, **`time`**, **`os`**, **`system`**, **`numpy`**, **`pandas`**, **`matplotlib`**. The first five install **stub** `.pseudo` files that point at builtins; **`numpy`**, **`pandas`**, and **`matplotlib`** are **teaching** stubs only (no those runtimes) — see **[NumPy-style patterns](numpy-style.md)** / **[`examples/numpy-style/`](../../examples/numpy-style/)**, **[Pandas-style patterns](pandas-style.md)** / **[`examples/pandas-style/`](../../examples/pandas-style/)**, and **[Matplotlib-style patterns](matplotlib-style.md)** / **[`examples/matplotlib-style/`](../../examples/matplotlib-style/)**. **Builtins** (e.g. `SQRT`, `GETENV`) live in the interpreter whether or not you run **`pkg install`**. Runnable programs that exercise those builtins are under **[`examples/stdlib-demos/`](../../examples/stdlib-demos/)** (see **[`examples/README.md`](../../examples/README.md)**).

**Examples:**

```bash
pkg available
pkg install math
pkg -i random
```

### `pkg available`

Prints the catalog path and one line per **subdirectory** of the catalog (each is an installable name). If no catalog is found, prints a short hint to set **`PSEUDOCODE_STDLIB`** or run from the repository root.

### `pkg version` (aliases: `-v`, `--version`)

Prints the package manager and toolkit version (same as **`pseudocode`**).

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

### `pkg help` (aliases: `-h`, `--help`)

Usage summary and pointer to **`~/.pseudocode`** layout.

---

## Abbreviations (summary)

| Long form | Short form |
|-----------|------------|
| `pkg install NAME` (catalog) | `pkg -i NAME`, `pkg --install NAME` |
| `pkg version` | `pkg -v`, `pkg --version` |
| `pkg help` | `pkg -h`, `pkg --help` |

There are **no** one-letter aliases for `available`, `list`, `remove`, or `update`.

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

After **`cmake --install`**, catalog files are typically under **`share/pseudocode/stdlib-packages`** inside your install prefix. Point **`PSEUDOCODE_STDLIB`** at that directory if **`pkg available`** is empty from an arbitrary working directory.

---

## Including installed code

The interpreter implements **`IMPORT module AS alias`** for the built-in **library namespaces** **`numpy`**, **`pandas`**, and **`matplotlib`** (see [Language reference: IMPORT](reference/language.md#40-import-python-style-library-alias) and [Built-ins: library modules](reference/builtins.md#python-style-library-modules-numpy-pandas-matplotlib)). That is separate from **`pkg install`**, which only copies **stub** `.pseudo` files into your packages directory.

**`INCLUDE "path"`** is not implemented. Practical options for arbitrary shared code today:

1. **Copy** the needed `.pseudo` file into your project.  
2. **Concatenate** sources in a build script before running.  
3. **Teach** students to paste small library snippets (acceptable for exam-sized tasks).

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
