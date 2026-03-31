# Pandas-style examples (Pseudocode)

Patterns inspired by **Pandas** for students who think in **tables and Series**, expressed as **parallel columns** (fixed row count) and **loops**.

**Read first:** [`docs/pandas-style.md`](../../docs/pandas-style.md) (limitations vs real Pandas).

Run from the **repository root**:

```bash
./build/pseudocode run examples/pandas-style/01-parallel-columns.pseudo
```

| File | Pandas-ish idea |
|------|-----------------|
| **`01-parallel-columns.pseudo`** | One “table”: row index + several columns |
| **`02-head-and-tail.pseudo`** | First / last *k* rows (`head`, `tail`) |
| **`03-filter-rows.pseudo`** | Boolean filter on one column; count matches |
| **`04-column-min-max-mean.pseudo`** | Single-pass stats on one numeric column |
| **`05-idxmin-row.pseudo`** | Row index of minimum (`idxmin`, 1-based) |
| **`06-groupby-sum.pseudo`** | Sum of a value column by category key |
| **`07-merge-inner-on-key.pseudo`** | Inner join on integer key (nested loops) |
| **`08-pd-workspace-globals.pseudo`** | `Pd` prefix globals + scalar helper functions |

There is **no** `import`: copy patterns into your own programs or align names with your worksheet.
