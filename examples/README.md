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
| **`numpy-style/`** | NumPy-*like* patterns (sums, dot product, linspace-style fills, small matmul) using loops — see **`docs/numpy-style.md`**. Optional **`pkg install numpy`** installs a stub that points to that doc. |
| **`pandas-style/`** | Pandas-*like* tabular patterns (parallel columns, head/tail, filter, groupby, merge) — see **`docs/pandas-style.md`**. Optional **`pkg install pandas`** installs a teaching stub. |
| **`matplotlib-style/`** | Matplotlib-*like* plotting *data* (series, ASCII bars, file export) — see **`docs/matplotlib-style.md`**. Optional **`pkg install matplotlib`** installs a teaching stub. |
| **Root** (`hello.pseudo`, …) | Short samples kept for quick smoke tests and docs that reference a single path. |

## Optional `pkg` catalog

The repo ships **`stdlib-packages/{math,random,time,os,system,numpy,pandas,matplotlib}/stub.pseudo`** for `pkg install`. The first five do not define extra behaviour beyond pointing at docs; see **`docs/stdlib/`** and **`examples/stdlib-demos/`** for runnable demos of the underlying builtins. **`numpy`**, **`pandas`**, and **`matplotlib`** are documentation-only; run the programs under **`numpy-style/`**, **`pandas-style/`**, and **`matplotlib-style/`** instead.
