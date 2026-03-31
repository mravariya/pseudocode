# Package managers (Homebrew and others)

This project can be installed from **source** (see [Installation](installation.md)) or via **third-party package managers** where maintainers or users publish recipes.

**See also:** [One-command install scripts](installation.md#22-one-command-install-scripts) · [CMake build](installation.md#61-cmake-preferred-when-available)

---

## Homebrew (macOS / Linux)

The repository includes a **tap-style** formula at **[`Formula/pseudocode.rb`](../Formula/pseudocode.rb)**. It builds with **CMake**, enables **`PC_EMBED_INSTALL_STDLIB`** so **`pkg available`** finds **`share/pseudocode/stdlib-packages`** inside the install prefix (required after the Cellar layout moves files out of the build tree).

### Install from GitHub (recommended)

```bash
brew tap mravariya/pseudocode https://github.com/mravariya/pseudocode
brew install --HEAD mravariya/pseudocode/pseudocode
```

The formula is **HEAD-only** until versioned release tarballs are published; **`--HEAD`** tracks **`main`**.

### Install from a local clone

```bash
cd /path/to/pseudocode
brew install --HEAD ./Formula/pseudocode.rb
```

### Dependencies

The formula declares **`cmake`** (build) and **`readline`** (recommended for a nicer REPL). Linux users may need **`ncurses`** via Homebrew’s dependency chain when using Linuxbrew.

### Submitting to **homebrew-core**

To propose **`brew install pseudocode`** without a custom tap, open a pull request against [**Homebrew/homebrew-core**](https://github.com/Homebrew/homebrew-core). You will typically need:

- A **stable** source archive (**tagged release** on GitHub) and a **`sha256`** checksum (not a moving **`main`** tarball).
- Evidence of **notability** (Homebrew’s audit rules: stars/forks, usage).
- A formula that passes **`brew audit --strict`** and **`brew test`**.

After the first stable release, you can adapt **`Formula/pseudocode.rb`** with a **`url`** / **`sha256`** block and remove or supplement the **`head`** spec.

---

## Other ecosystems (unofficial)

There is **no** first-party **Scoop**, **winget**, **Chocolatey**, **Snap**, or **APT** package in this repository yet. Community recipes are welcome; they should:

- Install **`pseudocode`** and **`pkg`** together (same build).
- Preserve **`stdlib-packages`** next to the prefix (or set **`PSEUDOCODE_STDLIB`** to that directory) so **`pkg available`** works. CMake **`cmake --install`** with **`-DPC_EMBED_INSTALL_STDLIB=ON`** matches what the Homebrew formula does.

---

## Maintainer note: `PC_EMBED_INSTALL_STDLIB`

When packaging with **`cmake --install`**, configure with **`-DPC_EMBED_INSTALL_STDLIB=ON`** so the binaries embed **`$prefix/share/pseudocode/stdlib-packages`**. Default **OFF** keeps **`./build/pseudocode`** working from a build tree without installing. See **`CMakeLists.txt`**.
