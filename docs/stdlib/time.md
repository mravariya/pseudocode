# Time-style builtins (`time`)

Rough analogues to pieces of Python’s [`time`](https://docs.python.org/3/library/time.html). There is no `struct_time`, `strftime` with arbitrary format strings, or monotonic clock in this subset.

---

## Implemented

| Python (rough analogue) | Pseudocode | Returns | Notes |
|-------------------------|------------|---------|-------|
| `time.time()` | `TIME()` | `REAL` | Seconds since Unix epoch (`time_t` cast to `double`). |
| — | `CLOCKMS()` | `INTEGER` | CPU time used by the process, milliseconds (`clock()`), or error if unavailable. |
| `time.strftime(...)` (fixed format) | `NOWSTRING()` | `STRING` | Local wall-clock time as `"%Y-%m-%d %H:%M:%S"`. |

---

## Not implemented

`sleep`, `gmtime` / `localtime` structs, `perf_counter`, `monotonic`, `time_ns`, timezone names, `strptime`, high-resolution timers, etc.

---

## Example

```text
OUTPUT TIME()
OUTPUT NOWSTRING()
```

**Repository demo:** [`examples/stdlib-demos/time/time-and-clock.pseudo`](../../examples/stdlib-demos/time/time-and-clock.pseudo).
