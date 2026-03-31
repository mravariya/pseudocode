# Recent collaboration log (high level)

This page is a **curated summary** of major topics from recent Cursor sessions around this repository. It is **not** a verbatim chat transcript and omits casual wording, tool noise, and anything that belongs in a **private** log.

**Private session notes (never committed):** see **[Agent session log (format)](agent-session-log.md)** and the gitignored file **`AGENT.local.md`** at the repo root.

---

## 2026-03-30 (themes)

| Area | Summary |
|------|---------|
| **Teaching “library” layers** | Added **NumPy-style**, **Pandas-style**, and **Matplotlib-style** docs (`docs/numpy-style.md`, `pandas-style.md`, `matplotlib-style.md`), runnable examples under `examples/numpy-style/`, `pandas-style/`, `matplotlib-style/`, and optional **`pkg`** stubs under `stdlib-packages/{numpy,pandas,matplotlib}/`. |
| **Interpreter / tooling** | Work on **package manager** behaviour, **stdlib** builtins documentation, **examples** layout (`vanilla-pseudocode/`, `stdlib-demos/`), and related **docs** cross-links. |
| **Pseudocode gotchas** | Example fixes (e.g. **`step`** is a reserved **`FOR`** keyword; use another name for increments). |

---

## 2026-03-31 (themes)

| Area | Summary |
|------|---------|
| **Math / examples** | Clarified **`PI()`** and **`EULER()`** as **zero-argument functions** (not variables); updated **`examples/main.pseudo`** and **`docs/stdlib/math.md`**. |
| **Install UX** | **`scripts/install.sh`** wrapper, **`PREFIX`** documented, **Windows** install script headers, **`docs/installation.md`** §2.2 / §12, **`uninstall-local.sh`**, **`dev.sh`** messaging. |
| **Homebrew** | **`Formula/pseudocode.rb`** (stable **`url`/`sha256`** + optional **`head`**), **`PC_EMBED_INSTALL_STDLIB`** in **CMake** for prefix installs, **`.gitattributes`** `Formula/` **export-ignore** for consistent tarball checksums, **`scripts/brew-pseudocode.sh`**, **`scripts/homebrew-sync-tag-sha.sh`**, **`packaging/homebrew-core-pseudocode.rb`**, Git tag **`v1.0.0`**, CI job **`cmake-install-prefix`**. |
| **Docs / site** | **`docs/package-managers.md`** (tap install, **formulae.brew.sh** note, core PR checklist), README and **site** nav updates; **`python3 site/build.py`** for previews (**`site/public/`** gitignored, Pages via Actions). |
| **GUI** | **IDLE-style** editor documented in **`docs/getting-started.md`** (`tools/pseudocode_idle.py`). |
| **Local uninstall** | Removed **`~/.local/bin`** copies, **`build/`** binaries, **`~/.pseudocode/`** when requested (machine-specific; not repeated here). |
| **Agent workflow** | **`docs/agent-session-log.md`**, **`.cursor/rules/agent-session-log.mdc`**, **`.gitignore`** **`AGENT.local.md`**, README pointer. |
| **homebrew-core** | Explained that **https://formulae.brew.sh/formula/pseudocode** stays **404** until a formula is **merged** into **homebrew-core**; tap install remains the supported path until then. |

---

## How to use this file

- **Students / readers:** ignore this page unless you care how the project evolved.  
- **Maintainers:** add a **new dated subsection** when a batch of work is worth recording; keep tables short.  
- **Full chat:** keep out of git; use **`AGENT.local.md`** locally if you want a running agent checklist.

---

## See also

- [Documentation home](index.md)  
- [Agent session log (format)](agent-session-log.md)  
- [Package managers](package-managers.md)  
- [Installation](installation.md)
