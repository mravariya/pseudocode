# OS-style builtins (`os`)

Minimal environment access, analogous to reading Python’s [`os.environ`](https://docs.python.org/3/library/os.html#os.environ).

---

## Implemented

| Python | Pseudocode | Args | Returns |
|--------|------------|------|---------|
| `os.environ.get(name, "")` | `GETENV` | `STRING` name | `STRING` value, or empty string if unset |

---

## Not implemented

`os.getenv` default other than empty string, `os.putenv`, `os.unsetenv`, `os.getcwd`, `os.chdir`, `os.listdir`, `os.path.*`, process IDs, `os.system` (see [system](system.md)), etc.

---

## Example

```text
DECLARE home : STRING
home <- GETENV("HOME")
OUTPUT home
```
