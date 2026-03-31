# NumPy-style examples (Pseudocode)

Patterns inspired by **NumPy** for students who already think in **arrays + vector ops**, expressed in **fixed-bound** Cambridge-style pseudocode.

**Read first:** [`docs/numpy-style.md`](../../docs/numpy-style.md) (limitations vs real NumPy).

Run from the **repository root**:

```bash
./build/pseudocode run examples/numpy-style/00-import-np-sum-mean.pseudo
```

| File | NumPy-ish idea |
|------|----------------|
| **`00-import-np-sum-mean.pseudo`** | **`IMPORT numpy AS np`** then **`np.sum`**, **`np.mean`** (built-ins) |
| **`01-sum-and-mean.pseudo`** | Same sums with explicit `FOR` loops (no `IMPORT`) |
| **`02-fill-zeros-ones-linspace.pseudo`** | `zeros` / `ones` / `linspace`-style fill |
| **`03-dot-product.pseudo`** | `np.dot` on two equal-length 1D arrays |
| **`04-max-and-argmax.pseudo`** | `np.max`, `np.argmax` |
| **`05-matmul-vector.pseudo`** | Small **matrix × vector** (`M @ v`) |
| **`06-add-scalar-to-array.pseudo`** | Broadcast `a + k` with a loop |
| **`07-np-workspace-functions.pseudo`** | Globals `NpN`, `NpX` + `NpSumX`, `NpMeanX` |

You may use **`IMPORT numpy AS np`** and built-in **`np.sum`** / **`np.mean`** / … (see [`docs/reference/builtins.md`](../../docs/reference/builtins.md)) or copy **loop** patterns into your own programs.
