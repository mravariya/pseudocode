# Package managers (Homebrew and others)

This project can be installed from **source** (see [Installation](installation.md)) or via **Homebrew** (macOS / Linux).

**See also:** [One-command install scripts](installation.md#22-one-command-install-scripts) · [CMake build](installation.md#61-cmake-preferred-when-available) · **`scripts/brew-pseudocode.sh`**

---

## Homebrew (macOS / Linux)

### Find it on the Homebrew site

- **Formula browser (after it lands in core):** [https://formulae.brew.sh/formula/pseudocode](https://formulae.brew.sh/formula/pseudocode) — this URL works **only after** the formula is merged into **homebrew-core** (until then you may see “Not Found”).
- **Homebrew organisation:** [https://github.com/Homebrew](https://github.com/Homebrew)
- **Core formulae tree:** [https://github.com/Homebrew/homebrew-core/tree/master/Formula](https://github.com/Homebrew/homebrew-core/tree/master/Formula)

### Default install (stable tarball — like `brew install python`)

Uses release tag **`v1.0.0`** and checksum in **[`Formula/pseudocode.rb`](../Formula/pseudocode.rb)**. **No `--HEAD`** required.

```bash
brew tap mravariya/pseudocode https://github.com/mravariya/pseudocode
brew install mravariya/pseudocode/pseudocode
```

**One script (same thing):**

```bash
./scripts/brew-pseudocode.sh install
```

### Uninstall

```bash
./scripts/brew-pseudocode.sh uninstall
# or:
brew uninstall mravariya/pseudocode/pseudocode
brew untap mravariya/pseudocode
```

**After** a future **homebrew-core** merge:

```bash
brew uninstall pseudocode
```

### Rebuild / upgrade from Homebrew

```bash
./scripts/brew-pseudocode.sh reinstall
# or:
./scripts/brew-pseudocode.sh upgrade
```

**Build from source** (e.g. after toolchain change):

```bash
brew reinstall mravariya/pseudocode/pseudocode --build-from-source
```

### Run on your machine (after install)

Same as a normal CLI (ensure Homebrew **`bin`** is on your **`PATH`**):

```bash
pseudocode version
pseudocode run path/to/file.pseudo
pseudocode repl
pkg available
pkg install math
```

**Via helper:**

```bash
./scripts/brew-pseudocode.sh run version
./scripts/brew-pseudocode.sh pkg available
```

### Install latest `main` (optional)

```bash
brew install --HEAD mravariya/pseudocode/pseudocode
# or:
./scripts/brew-pseudocode.sh install-head
```

### Technical details

The formula builds with **CMake** and **`-DPC_EMBED_INSTALL_STDLIB=ON`** so **`pkg available`** resolves **`share/pseudocode/stdlib-packages`** under the Cellar/prefix. See **`CMakeLists.txt`**.

**Dependencies:** **`cmake`** (build), **`readline`**.

### Maintainer: new release tag + checksum

1. Bump **`PC_VERSION_*`** / **`PC_VERSION_STRING`** in **`src/pc_common.h`** (and **`CMakeLists.txt`** `project(... VERSION)` if you keep them aligned).
2. Commit and **`git push origin main`**.
3. **`git tag -a vX.Y.Z -m "Release vX.Y.Z"`** and **`git push origin vX.Y.Z`**.
4. Update **`url`** / **`sha256`** in **`Formula/pseudocode.rb`**:

   ```bash
   ./scripts/homebrew-sync-tag-sha.sh vX.Y.Z
   ```

5. If the checksum **changes** the formula file, the tarball changes → run **`homebrew-sync-tag-sha.sh`** again, **`git commit`**, then **`git tag -f vX.Y.Z`** and **`git push -f origin vX.Y.Z`** until one run leaves **`git diff`** empty.

6. Copy the same **`url`/`sha256`** into **`packaging/homebrew-core-pseudocode.rb`** when opening a **homebrew-core** PR.

---

## Submitting to **homebrew-core** (`brew install pseudocode`)

Homebrew maintainers require (among other things):

1. **Stable URL** + **`sha256`** (not a moving **`main`** archive) — satisfied by GitHub **tag** tarballs above.  
2. **Notability** (stars, forks, usage) — see [**Acceptable formulae**](https://docs.brew.sh/Acceptable-Formulae).  
3. **`brew audit --strict --new formula pseudocode`** and **`brew test pseudocode`** clean on macOS (and often Linux).

**Typical flow:**

1. **Fork** [homebrew-core](https://github.com/Homebrew/homebrew-core).  
2. Create **`Formula/pseudocode.rb`** from **`packaging/homebrew-core-pseudocode.rb`**, with the **same** **`url`/`sha256`** as **`Formula/pseudocode.rb`** in this repo.  
3. On your Mac (with Homebrew):

   ```bash
   brew install --build-from-source ./Formula/pseudocode.rb
   brew test pseudocode
   brew audit --strict --new formula pseudocode
   ```

4. Open a **pull request** to **Homebrew/homebrew-core** with the formula and the audit/test notes.

After merge, discovery and install become:

- **Search / info:** [formulae.brew.sh/formula/pseudocode](https://formulae.brew.sh/formula/pseudocode)  
- **Install:** **`brew install pseudocode`**  
- **Uninstall:** **`brew uninstall pseudocode`**

---

## Other ecosystems (unofficial)

There is **no** first-party **Scoop**, **winget**, **Chocolatey**, **Snap**, or **APT** package in this repository yet. Community recipes should install **`pseudocode`** and **`pkg`** together and preserve **`stdlib-packages`** (or **`PSEUDOCODE_STDLIB`**) like the Homebrew formula.

---

## Maintainer note: `PC_EMBED_INSTALL_STDLIB`

When packaging with **`cmake --install`**, configure with **`-DPC_EMBED_INSTALL_STDLIB=ON`** so binaries embed **`$prefix/share/pseudocode/stdlib-packages`**. Default **OFF** keeps **`./build/pseudocode`** working from a build tree without installing.
