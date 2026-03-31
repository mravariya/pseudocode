# Example programs

Runnable **`.pseudo`** sources grouped by topic. Run from the **repository root** (or adjust paths):

```bash
./build/pseudocode run examples/vanilla-pseudocode/for-loop-basic.pseudo
```

## Layout

| Folder | Purpose |
|--------|---------|
| **`vanilla-pseudocode/`** | Core language: types, constants, control flow, procedures, functions, arrays, strings, files. |
| **`stdlib-demos/`** | Built-in **math**, **random**, **time**, **environment**, and **system** APIs (same calls the **`pkg install math`** etc. stubs document — builtins work without `pkg`). |
| **`numpy-style/`** | **`IMPORT numpy AS np`** built-ins (**`np.sum`**, …) plus loop-based patterns — **`docs/numpy-style.md`**. **`pkg install numpy`** copies a stub that points at docs. |
| **`pandas-style/`** | Tabular loop patterns; **`IMPORT pandas AS pd`** gives **`pd.sum`** / **`pd.mean`** on one array — **`docs/pandas-style.md`**. |
| **`matplotlib-style/`** | Plot *data* with **`OUTPUT`** / files / ASCII; **`IMPORT matplotlib AS plt`** only provides **stub** calls — **`docs/matplotlib-style.md`**. |
| **Root** (`hello.pseudo`, …) | Short samples kept for quick smoke tests and docs that reference a single path. |

## Optional `pkg` catalog

The repo ships **`stdlib-packages/{math,random,time,os,system,numpy,pandas,matplotlib}/stub.pseudo`** for `pkg install`. The first five do not define extra behaviour beyond pointing at docs; see **`docs/stdlib/`** and **`examples/stdlib-demos/`** for runnable demos. **`numpy`**, **`pandas`**, and **`matplotlib`** stubs point at docs; the **`IMPORT` / `np.*`** behaviour is **built into the interpreter** — run **`numpy-style/00-import-np-sum-mean.pseudo`** and the other topic folders for full examples.
