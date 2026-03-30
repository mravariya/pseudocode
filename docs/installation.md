# Installing Pseudocode

This guide explains how to obtain a working `**pseudocode**` and `**pkg**` binary on your system. It is structured like installation chapters in **Python** and **Julia** documentation: supported platforms first, then build options, verification, and troubleshooting.

**Contents**

- [1. Supported platforms](#1-supported-platforms)
- [2. What you are installing](#2-what-you-are-installing)
- [3. macOS](#3-macos)
- [4. Linux](#4-linux)
- [5. Windows](#5-windows)
- [6. Build methods](#6-build-methods)
- [7. Verifying the installation](#7-verifying-the-installation)
- [8. PATH configuration](#8-path-configuration)
- [9. Optional dependencies](#9-optional-dependencies)
- [10. Troubleshooting](#10-troubleshooting)
- [11. Portable deployment and installers](#11-portable-deployment-and-installers)
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
chmod +x scripts/build.sh scripts/install-macos.sh
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
./scripts/install-macos.sh
```

This copies binaries to `**~/.local/bin**`.

### Step 8 — Add `~/.local/bin` to your PATH

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

#### A4 — Configure and build with CMake

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
chmod +x scripts/build.sh scripts/install-macos.sh
./scripts/build.sh
```

The script prefers **CMake** when `cmake` is on your `PATH`; otherwise it invokes `**cc`** directly and writes binaries to `build/`.

### 3.3 Install onto PATH

Default prefix is `**~/.local**`:

```bash
./scripts/install-macos.sh
```

Add to `**~/.zshrc**` (or `~/.bash_profile`):

```bash
export PATH="$HOME/.local/bin:$PATH"
```

Reload the shell or open a new terminal.

### 3.4 System-wide install (advanced)

Copy `build/pseudocode` and `build/pkg` to `/usr/local/bin` (requires administrator credentials):

```bash
sudo cp build/pseudocode build/pkg /usr/local/bin/
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
chmod +x scripts/build.sh
./scripts/build.sh
```

Install to `~/.local/bin` or another directory on your `PATH` (same pattern as macOS §3.3).

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
./scripts/build.sh
```

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

**Classroom USB stick:** copy `pseudocode` (and optionally `pkg`) plus sample `.pseudo` files. Students run:

```bash
/path/to/pseudocode /path/to/program.pseudo
```

**Installers:** wrap the same binaries:

- **macOS:** `pkgbuild` / `productbuild` installing into `/usr/local/bin`.
- **Windows:** **Inno Setup**, **WiX**, or **MSIX** adding the install directory to the user `PATH`.

**GitHub Releases:** attach zip/tar archives per platform; CI can build them (see `.github/workflows/build.yml`).

---

**Next step:** [Tutorial: Pseudocode in 30 minutes](tutorial/index.md) or [Getting started (quick)](getting-started.md).