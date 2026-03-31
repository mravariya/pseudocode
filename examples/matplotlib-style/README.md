# Matplotlib-style examples (Pseudocode)

Patterns for students who think in **plots and charts**, expressed as **numeric series**, **console output**, and optional **file export**. There is **no** drawing API in the interpreter.

**Read first:** [`docs/matplotlib-style.md`](../../docs/matplotlib-style.md).

Run from the **repository root**:

```bash
./build/pseudocode run examples/matplotlib-style/01-line-y-from-x.pseudo
```

The folder **`out/`** holds **generated** text files (git-ignored); it must exist — the repo ships **`out/.gitignore`** so the directory is tracked.

| File | Matplotlib-ish idea |
|------|---------------------|
| **`01-line-y-from-x.pseudo`** | Sample **`y = f(x)`** (sine) into arrays |
| **`02-print-xy-table.pseudo`** | Tabular **`x`**, **`y`** like a preview of `plot` data |
| **`03-write-xy-pairs-file.pseudo`** | Export interleaved x,y lines for Python/plotting tools |
| **`04-ascii-column-chart.pseudo`** | Vertical bars (`*`) as a rough “chart” |
| **`05-ascii-histogram-bins.pseudo`** | Bin counts + horizontal bar counts |
| **`06-two-series-console.pseudo`** | Two labelled blocks (subplot idea in text) |
| **`07-mpl-workspace-globals.pseudo`** | **`Mpl`** prefix globals + print helper |

Optional **`pkg install matplotlib`** installs a stub that points at **`docs/matplotlib-style.md`**.
