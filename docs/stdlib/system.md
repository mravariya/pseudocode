# System / shell (`system`)

Analogue to Python’s [`os.system`](https://docs.python.org/3/library/os.html#os.system): runs a string through the **host shell**. This is **dangerous** with untrusted input (command injection).

---

## Implemented

| Python | Pseudocode | Args | Returns |
|--------|------------|------|---------|
| `os.system(cmd)` | `SYSTEM` | `STRING` command | `INTEGER` host status (C `system()` return; interpretation is platform-specific). |

---

## Teaching note

Prefer **`OUTPUT`**, files, and pure pseudocode for assessments. Reserve **`SYSTEM`** for trusted local tooling only.

---

## Example

```text
DECLARE code : INTEGER
code <- SYSTEM("echo ok")
OUTPUT code
```
