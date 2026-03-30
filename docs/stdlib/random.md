# Random-style builtins (`random`)

Similar to Python’s [`random`](https://docs.python.org/3/library/random.html), but implemented with C `rand()` / `srand()` (not Mersenne Twister, **not** cryptographically secure).

---

## Implemented

| Python (typical) | Pseudocode | Behaviour |
|------------------|------------|-----------|
| `random.random()` | `RANDOM()` | `REAL` in `[0, 1)`. |
| — | `RANDOM(n)` / `RAND(n)` with integer `n > 0` | `REAL` in `[0, n)`. |
| `random.randint(a, b)` | `RANDINT(a, b)` / `RANDOMINT(a, b)` | Inclusive integer in `[a, b]` (both ends included). |
| `random.seed(s)` | `RANDOMSEED(s)` / `SEEDRANDOM(s)` / **`SEED(s)`** | Seeds the global generator; returns `0` (discard). |

**Also available:** `RAND` as an alias for `RANDOM` (see [builtins](../reference/builtins.md)).

---

## Not implemented

`random.choice`, `random.shuffle`, `random.sample`, `random.gauss`, `random.expovariate`, `secrets` module, per-instance generators, etc.

---

## Example

```text
CALL SEED(42)
DECLARE n : INTEGER
n <- RANDINT(1, 6)
OUTPUT n
```
