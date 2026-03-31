# Matplotlib-style patterns in Pseudocode

This interpreter has **no** `import matplotlib`, **no** pixels, and **no** plotting API. This page is a **conceptual bridge**: how **chart-minded** workflows map to **loops**, **`OUTPUT`**, **text diagrams**, and optional **file export** for real tools (Python, spreadsheet, gnuplot, …).

**Runnable code:** [`examples/matplotlib-style/`](../examples/matplotlib-style/) · **Examples catalogue:** [examples.md](examples.md)

**Related:** sample points and grids follow [NumPy-style patterns](numpy-style.md); tabular series follow [Pandas-style patterns](pandas-style.md).

---

## What is missing compared to Matplotlib

| Matplotlib idea | In this Pseudocode |
|-----------------|-------------------|
| `plot`, `scatter`, `imshow`, … | **No** — compute **`x[i]`**, **`y[i]`** (or a 2D grid) with **`FOR`**, then **`OUTPUT`** or **`WRITEFILE`**. |
| Axes, labels, legends, `subplots` | **No** — print **titles as strings** and separate **blocks** of numbers or ASCII art. |
| Interactive figures, saving PNG/PDF | **No** — export **text data** and plot elsewhere; or draw a **rough ASCII** preview in the terminal. |
| `plt.style`, colours | **No** — ASCII uses one character (e.g. `*`, `#`). |

What you **do** have: **math builtins** (e.g. `SIN`, `COS`, `PI`) for **y = f(x)**, **fixed-size arrays**, **`OUTPUT`**, **`OPENFILE` … FOR write** + **`WRITEFILE`** (see [§6 File handling](reference/language.md#6-file-handling)), and the host **`SYSTEM`** builtin (use only with trusted commands — see [stdlib: system](stdlib/system.md)).

### Reserved names

**`STEP`** is reserved in **`FOR`** loops — do not use **`step`** as a variable name (see [NumPy-style — reserved names](numpy-style.md#reserved-names-that-clash-with-numpy-habits)).

---

## Pattern: export (x, y) for a real plot

**`WRITEFILE`** writes **one value per line** (and appends a newline). A simple portable format is **interleaved pairs**: for each point `i`, write **`X[i]`** then **`Y[i]`** (two lines per point). In Python you can read:

```python
lines = [float(line) for line in open("xy_pairs.txt")]
xs, ys = lines[0::2], lines[1::2]
plt.plot(xs, ys)
```

---

## Quick mapping table

| Matplotlib (rough) | Pseudocode pattern |
|--------------------|-------------------|
| `np.linspace` + `y = f(x)` | Fill **`X[i]`**, **`Y[i]`** in a **`FOR`** (see [linspace-style](numpy-style.md)) |
| `plt.plot(x, y)` | **`OUTPUT`** columns; or **`WRITEFILE`** pairs as above |
| `plt.scatter(x, y)` | Same data as plot; ASCII “density” is optional (examples) |
| Quick visual sanity check | **ASCII bar / column** plot: build **`STRING`**s with **`&`** in a loop |
| `plt.hist` | **Bin counts** with **`IF`** / **`CASE`**; print counts or ASCII bars |
| `plt.savefig` | **Not** in-language — write data file, plot with another program |

---

## See also

- [Built-in functions](reference/builtins.md) — `SIN`, `COS`, `PI`, …  
- [Arrays (1D and 2D)](reference/arrays.md)  
- [NumPy-style patterns](numpy-style.md) · [Pandas-style patterns](pandas-style.md) · [stdlib: system](stdlib/system.md) (`SYSTEM`, optional external tools)  
- [Language reference](reference/language.md)
