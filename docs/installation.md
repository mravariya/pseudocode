# Installing Pseudocode

**Prefer not to install on your own PC?** See **[Try online](try-online.md)** (GitHub Codespaces or Gitpod) for a browser-based Linux environment with the same build steps.

This guide explains how to obtain a working `**pseudocode**` and `**pkg**` binary on your system. It is structured like installation chapters in **Python** and **Julia** documentation: supported platforms first, then build options, verification, and troubleshooting.

**Contents**

- [1. Supported platforms](#1-supported-platforms)
- [2. What you are installing](#2-what-you-are-installing) (includes [2.1 `pkg`](#21-installing-pkg-and-putting-it-on-path) and [2.2 One-command install scripts](#22-one-command-install-scripts))
- [3. macOS](#3-macos)
- [4. Linux](#4-linux)
- [5. Windows](#5-windows)
- [6. Build methods](#6-build-methods) (includes [6.4 dev rebuild](#64-one-command-rebuild-while-developing))
- [7. Verifying the installation](#7-verifying-the-installation)
- [8. PATH configuration](#8-path-configuration)
- [9. Optional dependencies](#9-optional-dependencies)
- [10. Troubleshooting](#10-troubleshooting)
- [11. Portable deployment and installers](#11-portable-deployment-and-installers)
- [12. Homebrew and other package managers](#12-homebrew-and-other-package-managers)
- **[Step-by-step: macOS](#step-by-step-macos)**
- **[Step-by-step: Windows](#step-by-step-windows)**

---

## Step-by-step: macOS

Follow these in order. Replace the folder name if yours differs (quotes matter if the path contains spaces).

### Step 1 — Open Terminal

- Press **⌘ + Space**, type **Terminal**, press **Enter**.

### Step 2 — Go to the project folder

If the project is in your Downloads folder (default name from this repo):

```bash
cd ~/Downloads/9618\ Computer\ Science\ Pseudocode\ Interpreter
```

Or use quotes:

```bash
cd "~/Downloads/9618 Computer Science Pseudocode Interpreter"
```

Check you are in the right place:

```bash
ls scripts/build.sh
```

You should see `scripts/build.sh` listed (not “No such file”).

### Step 3 — Install Apple’s compiler tools (once per Mac)

```bash
xcode-select --install
```

A dialog appears → click **Install** → wait until it finishes. If it says the tools are already installed, continue.

### Step 4 — Make the build scripts executable (once)

```bash
chmod +x scripts/build.sh scripts/install.sh scripts/install-macos.sh
```

### Step 5 — Build the programs

```bash
./scripts/build.sh
```

Wait until it finishes without errors. You should get:

- `build/pseudocode`
- `build/pkg`

(Optional) If you use **Homebrew** and want CMake to drive the build:

```bash
brew install cmake
./scripts/build.sh
```

### Step 6 — Run a test (no PATH setup yet)

Still inside the project folder:

```bash
./build/pseudocode examples/hello.pseudo
```

You should see something like:

```text
Hello from Pseudocode
Length =  21
```

If that works, the build is good.

### Step 7 — (Recommended) Install so you can type `pseudocode` anywhere

```bash
./scripts/install.sh
```

(`**./scripts/install-macos.sh**` does the same thing; the name is historical.)

This runs **`build.sh`** again, then copies **`pseudocode`** and **`pkg`** into **`$PREFIX/bin`**. The default is **`PREFIX=$HOME/.local`**, i.e. **`~/.local/bin`**. To install elsewhere, set **`PREFIX`** (ensure **`$PREFIX/bin`** is writable — use **`sudo`** for system paths such as **`/usr/local`**):

```bash
sudo env PREFIX=/usr/local ./scripts/install.sh
```

### Step 8 — Add the install `bin` directory to your PATH

If you used the default **`PREFIX`**, add **`~/.local/bin`**. If you set **`PREFIX`** to something else, add **`$PREFIX/bin`** instead.

**If you use zsh (default on recent macOS):**

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

**If you use bash:**

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bash_profile
source ~/.bash_profile
```

### Step 9 — Verify from any directory

```bash
pseudocode version
pseudocode help
```

Run a sample using a full path to the file:

```bash
pseudocode "$HOME/Downloads/9618 Computer Science Pseudocode Interpreter/examples/hello.pseudo"
```

---

## Step-by-step: Windows

Pick **one** path: **A (Visual Studio)** is common on school PCs; **B (MSYS2)** behaves closest to Mac/Linux and builds `**pkg`** most reliably.

---

### Path A — Visual Studio 2022 + CMake (native Windows)

#### A1 — Install software (once)

1. Download and install **Visual Studio 2022** (Community is fine).
2. In the installer, select workload **“Desktop development with C++”** → **Install**.
3. Download **CMake** for Windows from [cmake.org](https://cmake.org/download/) and install. During setup, choose **“Add CMake to the system PATH”** if offered.

#### A2 — Open the right terminal

- Press **Start**, type **“x64 Native Tools Command Prompt for VS 2022”** → open it.  
(Or **“Developer PowerShell for VS 2022”**.)

#### A3 — Go to the project folder

Example if the project is on your Desktop (adjust the path):

```bat
cd C:\Users\YourName\Desktop\9618 Computer Science Pseudocode Interpreter
```

Or Downloads:

```bat
cd C:\Users\YourName\Downloads\9618 Computer Science Pseudocode Interpreter
```

Check:

```bat
dir scripts\build.sh
```

#### A3b — Optional: build and install with PowerShell (recommended)

In **Developer PowerShell for VS 2022** (repository root), you can build with CMake and copy **`pseudocode.exe`** and **`pkg.exe`** to **`%LOCALAPPDATA%\Pseudocode\bin`**:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
.\scripts\install-windows.ps1
```

Then add **`%LOCALAPPDATA%\Pseudocode\bin`** to your user **Path** (same steps as **A7** below, but paste the **`Pseudocode\bin`** folder instead of **`build\Release`**), **close and reopen** the terminal, and run **`pseudocode version`**. Skip **A4–A6** if you use this path.

#### A4 — Configure and build with CMake (manual)

```bat
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

#### A5 — Find the `.exe` files

Depending on the generator, look for:

- `build\Release\pseudocode.exe` and `build\Release\pkg.exe`, **or**
- `build\pseudocode.exe` and `build\pkg.exe`

List the folder:

```bat
dir build\Release
dir build
```

#### A6 — Run a test using the full path to the exe

Example (adjust if your `.exe` is only under `build\`):

```bat
build\Release\pseudocode.exe examples\hello.pseudo
```

#### A7 — Add to PATH (so you can run `pseudocode` from anywhere)

1. Copy the **full folder path** that contains `pseudocode.exe` (e.g. `C:\...\build\Release`).
2. Press **Win + R** → type `sysdm.cpl` → **Enter**.
3. Tab **Advanced** → **Environment Variables**.
4. Under **User variables**, select **Path** → **Edit** → **New** → paste that folder path → **OK** on all dialogs.
5. **Close and reopen** Command Prompt or PowerShell.

#### A8 — Verify

```bat
pseudocode version
```

---

### Path B — MSYS2 + MinGW (recommended for `pkg` and Unix-like commands)

#### B1 — Install MSYS2

1. Download from [msys2.org](https://www.msys2.org/) and run the installer.
2. Finish the first-time update when the installer tells you to (update packages in the MSYS2 terminal).

#### B2 — Install compiler and CMake

Open **“MSYS2 MinGW x64”** from the Start menu, then run:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make
```

#### B3 — Go to your project (Windows drives under `/c/`)

Example:

```bash
cd "/c/Users/YourName/Downloads/9618 Computer Science Pseudocode Interpreter"
```

#### B4 — Build

```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

#### B5 — Run a test

```bash
./build/pseudocode examples/hello.pseudo
```

#### B6 — PATH (optional)

Add `**C:\msys64\mingw64\bin**` (or your MSYS2 install path) and the folder containing `pseudocode.exe` to the user **Path** in Windows (same as A7), **or** always run from this terminal using `./build/pseudocode`.

---

### Windows notes

- Use `**pseudocode.exe`** and `**pkg.exe**` in Command Prompt; in **PowerShell** the same names work once PATH is set.
- If `**pkg`** misbehaves under a pure MSVC build, use **MSYS2 (Path B)** for package commands.

---

## 1. Supported platforms


| OS                | Status                                              | Typical toolchain            |
| ----------------- | --------------------------------------------------- | ---------------------------- |
| **macOS** 11+     | Fully supported                                     | Apple Clang, optional CMake  |
| **Linux** (glibc) | Fully supported                                     | GCC or Clang, optional CMake |
| **Windows**       | Supported via **CMake + MSVC** or **MSYS2 / MinGW** | See §5                       |


The C code targets **C11**. No third-party runtime libraries are required beyond the C standard library and (on Unix) **libm** for math functions.

---

## 2. What you are installing


| Binary           | Purpose                                                                                              |
| ---------------- | ---------------------------------------------------------------------------------------------------- |
| `**pseudocode`** | Runs `.pseudo` / `.pseudocode` files, `repl`, `check`, etc.                                          |
| `**pkg**`        | Manages local package folders under `~/.pseudocode` (POSIX) or `%USERPROFILE%\.pseudocode` (Windows) |


Both are built from the same core sources; `pkg` omits the REPL object file in the CMake layout.

### 2.1 Installing `pkg` and putting it on `PATH`

- **`pkg`** is produced in the **same build step** as **`pseudocode`**. There is no separate download or package for it.
- After **`./scripts/build.sh`**, use **`./build/pkg`** (or install both binaries to `~/.local/bin`, `/usr/local/bin`, etc., as described in this guide).
- To **use** `pkg` after installation: see **[Package manager — how to use](package-manager.md#how-to-use-pkg-quick-start)** and **[short and full commands](package-manager.md#commands-short-and-full-forms)**.

### 2.2 One-command install scripts

These scripts **build** (if needed) and **copy** both programs to a directory you can put on **`PATH`**:

| Platform | Script | Default destination |
| -------- | ------ | ------------------- |
| **macOS / Linux** | **`scripts/install.sh`** (same as **`scripts/install-macos.sh`**) | **`$PREFIX/bin`** with **`PREFIX=$HOME/.local`** → **`~/.local/bin`** |
| **Windows** | **`scripts/install-windows.ps1`** (run in **PowerShell** from the repo root) | **`%LOCALAPPDATA%\Pseudocode\bin`** |

**Unix `PREFIX`:** override the install root, e.g. **`PREFIX=/opt/pseudocode ./scripts/install.sh`**. If **`$PREFIX/bin`** is not writable, run with **`sudo`** (for example **`sudo env PREFIX=/usr/local ./scripts/install.sh`**).

**Windows:** the PowerShell script uses **CMake** (same as a manual Visual Studio CMake build). If **`cmake`** is not on **`PATH`**, install CMake or follow the manual steps under [§5 Windows](#5-windows). After running the script, add **`%LOCALAPPDATA%\Pseudocode\bin`** to your **user** `Path` and open a **new** terminal.

---

## 3. macOS

### 3.1 Prerequisites

Install **Xcode Command Line Tools** (provides `clang`, `make`, `git`):

```bash
xcode-select --install
```

**CMake** is optional but recommended for IDE integration:

```bash
brew install cmake
```

### 3.2 Build from source

Clone or unpack the repository, then from the repository root:

```bash
chmod +x scripts/build.sh scripts/install.sh scripts/install-macos.sh
./scripts/build.sh
```

The script prefers **CMake** when `cmake` is on your `PATH`; otherwise it invokes `**cc`** directly and writes binaries to `build/`.

### 3.3 Install onto PATH

From the repository root:

```bash
./scripts/install.sh
```

(`**./scripts/install-macos.sh**` is equivalent.)

Default **`PREFIX`** is **`$HOME/.local`**, so binaries go to **`~/.local/bin`**. Add that directory to **`PATH`** (see [§8 PATH configuration](#8-path-configuration)).

### 3.4 System-wide install (advanced)

Either copy manually:

```bash
sudo cp build/pseudocode build/pkg /usr/local/bin/
```

Or use the install script with a **`PREFIX`** (must be writable or use **`sudo`**):

```bash
sudo env PREFIX=/usr/local ./scripts/install.sh
```

---

## 4. Linux

Install a C compiler and **make** (package names vary):

```bash
# Debian / Ubuntu example
sudo apt update
sudo apt install build-essential cmake
```

Then:

```bash
chmod +x scripts/build.sh scripts/install.sh scripts/install-macos.sh
./scripts/build.sh
```

Install onto **`PATH`** the same way as macOS — the **`install.sh`** / **`install-macos.sh`** script is **not** macOS-specific:

```bash
./scripts/install.sh
```

Default destination is **`~/.local/bin`** (override with **`PREFIX`**). Add **`$HOME/.local/bin`** (or **`$PREFIX/bin`**) to your shell profile if needed.

---

## 5. Windows

### 5.1 Recommended: Visual Studio + CMake

1. Install **Visual Studio** with workload **Desktop development with C++**.
2. Install **CMake** for Windows.
3. Open **x64 Native Tools Command Prompt** (or use Visual Studio’s CMake integration).

```powershell
cd path\to\9618-Computer-Science-Pseudocode-Interpreter
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Artifacts may appear as `build\Release\pseudocode.exe` (multi-config generators) or `build\pseudocode.exe` (Ninja / single-config).

### 5.2 MSYS2 / MinGW

Install **MSYS2**, then in the MinGW64 shell:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make
chmod +x scripts/build.sh scripts/install.sh scripts/install-macos.sh
./scripts/build.sh
```

Optional: **`./scripts/install.sh`** copies **`pseudocode`** and **`pkg`** into **`~/.local/bin`** (or **`$PREFIX/bin`**), same as on Linux.

This path gives the most **POSIX-like** behaviour for `**pkg`** (directory iteration).

### 5.3 `pkg` on Windows

The reference `pkg` tool uses `**dirent**`. It builds cleanly in **MSYS2/MinGW**. Pure **MSVC** builds may require additional portability patches; track issues in the project repository.

---

## 6. Build methods

### 6.1 CMake (preferred when available)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Install (optional):

```bash
cmake --install build --prefix "$HOME/.local"
```

### 6.2 Plain compiler (`scripts/build.sh` fallback)

Invokes `cc` with an explicit file list and `-lm` on Unix. No generated headers.

### 6.3 Debug builds

Pass `-DCMAKE_BUILD_TYPE=Debug` or compile with `-g -O0` for debugging the **C** interpreter (e.g. in GDB or LLDB).

### 6.4 One-command rebuild while developing

If you edit **`src/`** regularly, from the repository root run:

```bash
chmod +x scripts/dev.sh scripts/build.sh   # once per clone
./scripts/dev.sh
```

This rebuilds **`pseudocode`** and **`pkg`**, then runs **`pseudocode version`**, **`pseudocode check examples/hello.pseudo`**, and checks **`pkg`**. With **GNU Make**: **`make dev`** is equivalent. To also rebuild the static site: **`./scripts/dev.sh --site`**.

If you use **`./scripts/install.sh`** (or **`install-macos.sh`** / **`install-windows.ps1`**) to copy binaries onto **`PATH`**, run that install step again after **`dev.sh`** so the installed copy matches **`build/`**.

Details: [Getting started — Development rebuild loop](getting-started.md#development-rebuild-loop) · [Contributing](../CONTRIBUTING.md).

---

## 7. Verifying the installation

```bash
pseudocode version
```

Expected output includes `**1.0.0**` (or the version in `src/pc_common.h`).

```bash
pseudocode help
pkg help
```

Check **`pkg`** specifically (version should match **`pseudocode version`**):

```bash
pkg version
# or: pkg -v
# or: pkg --version
```

If you built from a clone, from the **repository root** you can list optional catalog bundles:

```bash
pkg available
```

Run a sample:

```bash
pseudocode examples/hello.pseudo
```

(from the repository root, or use the full path to `hello.pseudo`).

---

## 8. PATH configuration

The interpreter must be found as `**pseudocode**` on the shell search path.

- **macOS / Linux:** `echo $PATH` should contain the directory holding the binary.
- **Windows:** *Settings → System → About → Advanced system settings → Environment variables → Path* → add the folder containing `pseudocode.exe`.

---

## 9. Optional dependencies


| Dependency       | Effect                                                                                                                  |
| ---------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **GNU readline** | If CMake finds **readline** on Unix, the REPL uses line editing and history. Without it, a simple `fgets` loop is used. |
| **CMake**        | Generates build trees and install rules; not required if you use the `cc` fallback.                                     |


---

## 10. Troubleshooting

### `command not found: pseudocode`

The binary is not on `PATH`, or the shell was not restarted after editing profile files. Use the full path to the executable to confirm it runs.

### `cannot read file` when running a program

Your current working directory does not contain the `.pseudo` file. Pass an **absolute path** or `cd` to the correct folder.

### CMake cannot find a compiler

Install **Xcode Command Line Tools** (macOS) or **build-essential** (Linux) or the MSVC C++ workload (Windows).

### `pkg` fails on Windows with MSVC

Use **MSYS2** for `pkg`, or wait for a Windows-specific port. The `**pseudocode`** runner itself is usually unaffected.

### Colours look wrong in the terminal

Set `**PSEUDO_NO_COLOR=1**` to disable ANSI escape sequences (see [CLI](cli.md)).

---

## 11. Portable deployment and installers

**This repository** ships **`scripts/install.sh`** (macOS/Linux), **`scripts/install-macos.sh`** (same behaviour), and **`scripts/install-windows.ps1`** (PowerShell) — see [§2.2](#22-one-command-install-scripts).

**Classroom USB stick:** copy `pseudocode` (and optionally `pkg`) plus sample `.pseudo` files. Students run:

```bash
/path/to/pseudocode /path/to/program.pseudo
```

**Installers:** wrap the same binaries:

- **macOS:** `pkgbuild` / `productbuild` installing into `/usr/local/bin`.
- **Windows:** **Inno Setup**, **WiX**, or **MSIX** adding the install directory to the user `PATH`.

**GitHub Releases:** attach zip/tar archives per platform; CI can build them (see `.github/workflows/build.yml`).

---

## 12. Homebrew and other package managers

**Homebrew (macOS / Linux):** **[`Formula/pseudocode.rb`](../Formula/pseudocode.rb)** supports a **stable** install from a **tagged** GitHub archive (default — like **`brew install python`**) and optional **`--HEAD`**.

```bash
brew tap mravariya/pseudocode https://github.com/mravariya/pseudocode
brew install mravariya/pseudocode/pseudocode
```

**Helper script (install / uninstall / reinstall / `run` / `pkg`):** **`./scripts/brew-pseudocode.sh`**.

**Links, uninstall, rebuild, and homebrew-core PR steps:** **[Package managers](package-managers.md)**.

---

**Next step:** [Tutorial: Pseudocode in 30 minutes](tutorial/index.md) or [Getting started (quick)](getting-started.md).