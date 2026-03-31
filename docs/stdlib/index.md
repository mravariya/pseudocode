# Standard library areas (Python-style)

The interpreter ships **built-in functions** in the C runtime (`src/pc_stdlib.c`). They are available **without** installing anything. **`IMPORT numpy AS np`** (and **`pandas`**, **`matplotlib`**) exposes **Python-style** qualified calls such as **`np.sum(A)`**; see **[Built-in functions — library modules](../reference/builtins.md#python-style-library-modules-numpy-pandas-matplotlib)**.

The **`pkg install`** catalog (`math`, `random`, `time`, `os`, `system`, `numpy`, `pandas`, `matplotlib`) copies **optional stub** `.pseudo` files into `~/.pseudocode/packages/` for classroom handouts and does **not** change which builtins exist. The **`numpy`**, **`pandas`**, and **`matplotlib`** stubs mainly **point at docs and examples** — the real **`np.*`-style** behaviour lives in the C runtime.

**Catalog layout:** repository root [`stdlib-packages/`](../../stdlib-packages/) (also installed under `share/pseudocode/stdlib-packages` when you `cmake --install`). **`pkg`** finds it via `PSEUDOCODE_STDLIB`, compile-time `PC_BUILTIN_STDLIB_PACKAGES`, or `stdlib-packages` next to the current working directory.

| Topic | Pseudocode builtins | Python analogue |
|-------|---------------------|-----------------|
| [Math](math.md) | `SQRT`, `SIN`, `COS`, … | [`math`](https://docs.python.org/3/library/math.html) |
| [Random](random.md) | `RANDOM`, `RANDINT`, `SEED`, … | [`random`](https://docs.python.org/3/library/random.html) |
| [Time](time.md) | `TIME`, `CLOCKMS`, `NOWSTRING` | [`time`](https://docs.python.org/3/library/time.html) |
| [OS](os.md) | `GETENV` | [`os.environ`](https://docs.python.org/3/library/os.html#process-parameters) |
| [System](system.md) | `SYSTEM` | [`os.system`](https://docs.python.org/3/library/os.html#os.system) |

**Runnable demos** (no `pkg` required): [`examples/stdlib-demos/`](../../examples/stdlib-demos/) — see [`examples/README.md`](../../examples/README.md).

**See also:** [Built-in functions (full list)](../reference/builtins.md) · [Examples catalogue](../examples.md) · [Package manager](../package-manager.md) · [CLI](../cli.md)
