# Visual Studio Code integration

This guide explains how to use **Visual Studio Code** (or **VSCodium**) effectively with Pseudocode: syntax colouring, snippets, tasks to **Run** and **Check**, and how this compares to first-party editor support for languages like **Python** (extension marketplace) and **Julia** (Julia VS Code extension).

**See also:** [CLI](cli.md) · [Tutorial](tutorial/index.md)

---

## 1. Install the Pseudocode extension (folder)

The extension lives in the repository at **`vscode/pseudocode/`**. It is a **grammar + snippets** extension (no Node runtime required in the editor beyond VS Code itself).

### Option A — Development install

1. Open VS Code.  
2. **File → Open Folder…** and select **`vscode/pseudocode`**.  
3. Press **F5** only if you are developing the extension (optional).

To install into your user extensions directory, copy the folder:

```text
~/.vscode/extensions/pseudocode-1.0.0/
```

(on Windows: `%USERPROFILE%\.vscode\extensions\pseudocode-1.0.0\`), then **Reload Window**.

### Option B — Command Palette

Run **Developer: Install Extension from Location…** and choose **`vscode/pseudocode`**.

### Option C — VSIX (if you package one)

```bash
# from vscode/pseudocode, if you add vsce packaging later
vsce package
```

Then **Extensions → … → Install from VSIX…**.

---

## 2. What the extension provides

| Feature | Location in repo |
|---------|------------------|
| **Language ID** `pseudocode` | `package.json` |
| **File extensions** `.pseudo`, `.pseudocode` | `package.json` |
| **TextMate grammar** | `syntaxes/pseudocode.tmLanguage.json` |
| **Comment toggling** `//` | `language-configuration.json` |
| **Snippets** (`if`, `for`, …) | `snippets/pseudocode.json` |

The grammar highlights **keywords**, **strings**, **numbers**, and **comments**. It is not a **semantic** highlighter (it does not know if a name is a variable vs procedure—that would require a language server).

---

## 3. Run and Check — tasks (recommended)

This repository includes **`.vscode/tasks.json`** at the project root. After building **`build/pseudocode`**:

1. Open any **`.pseudo`** file.  
2. **Terminal → Run Task…**  
3. Choose **pseudocode: Run current file** (default **build** task, **⇧⌘B** / **Ctrl+Shift+B** on many setups).  
4. Or **pseudocode: Check current file** for parse-only validation.

Tasks use:

```json
"${workspaceFolder}/build/pseudocode"
```

If you installed `pseudocode` onto your **PATH**, change **`command`** to **`pseudocode`** and keep **`args`** as `["${file}"]` or `["check", "${file}"]`.

---

## 4. Snippets

With a `.pseudo` file focused, type a **prefix** and accept the completion:

| Prefix | Inserts |
|--------|---------|
| `if` | `IF` … `ENDIF` skeleton |
| `for` | `FOR` … `NEXT` skeleton |
| `while` | `WHILE` … `ENDWHILE` |
| `proc` | `PROCEDURE` … `ENDPROCEDURE` |
| `func` | `FUNCTION` … `ENDFUNCTION` |
| `dec` | `DECLARE` with type choice |

Edit **`snippets/pseudocode.json`** to match your teaching style.

---

## 5. Debugging

### 5.1 Pseudocode-level “debug”

Set in the integrated terminal:

```bash
export PSEUDO_TRACE=1   # macOS/Linux
pseudocode yourfile.pseudo
```

Trace lines appear on **stderr**.

### 5.2 C-level debugging

To step through the **interpreter implementation** (useful for university projects), compile with debug symbols and attach **CodeLLDB** (macOS/Linux) or **Visual Studio** debugger (Windows) to the **`pseudocode`** binary with arguments **`${file}`**.

Line-by-line stepping **inside** the `.pseudo` source would require a **debugger adapter** for Pseudocode (future work).

---

## 6. Settings

The extension contributes **`pseudocode.executablePath`** (default **`pseudocode`**) as documentation for tasks—you can mirror that value in your own **`tasks.json`** or wrapper scripts.

---

## 7. Comparison to Python / Julia workflows

| Aspect | Python | Julia | Pseudocode (this project) |
|--------|--------|-------|---------------------------|
| Marketplace extension | **Pylance** / Python | **Julia** | Install from **folder** / VSIX |
| Run file | Play button / integrated terminal | Julia: **Execute** | **Tasks** calling **`pseudocode`** |
| Debugger | **debugpy** | **JuliaDebug** | **Trace** env var or **C** debugger |
| Formatter / linter | **Black**, **Ruff** | **JuliaFormatter** | Not yet |

---

## 8. Troubleshooting

| Problem | Remedy |
|---------|--------|
| No syntax colour | Confirm language mode (bottom-right) is **Pseudocode**; reload window after install. |
| Task fails “not found” | Build first: `./scripts/build.sh`; check **`build/pseudocode`** exists. |
| Wrong file run | **`${file}`** must be the active editor tab when you run the task. |
