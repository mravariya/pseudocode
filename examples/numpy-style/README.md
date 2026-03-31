# NumPy-style examples (Pseudocode)

Patterns inspired by **NumPy** for students who already think in **arrays + vector ops**, expressed in **fixed-bound** Cambridge-style pseudocode.

**Read first:** [`docs/numpy-style.md`](../../docs/numpy-style.md) (limitations vs real NumPy).

Run from the **repository root**:

```bash
./build/pseudocode run examples/numpy-style/01-sum-and-mean.pseudo
```

| File | NumPy-ish idea |
|------|----------------|
| **`01-sum-and-mean.pseudo`** | `np.sum`, `np.mean` on a 1D `REAL` array |
| **`02-fill-zeros-ones-linspace.pseudo`** | `zeros` / `ones` / `linspace`-style fill |
| **`03-dot-product.pseudo`** | `np.dot` on two equal-length 1D arrays |
| **`04-max-and-argmax.pseudo`** | `np.max`, `np.argmax` |
| **`05-matmul-vector.pseudo`** | Small **matrix × vector** (`M @ v`) |
| **`06-add-scalar-to-array.pseudo`** | Broadcast `a + k` with a loop |
| **`07-np-workspace-functions.pseudo`** | Globals `NpN`, `NpX` + `NpSumX`, `NpMeanX` |

There is **no** `import`: copy patterns into your own programs or keep names consistent with your worksheet.
