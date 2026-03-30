# Try pseudocode online

**GitHub Pages** serves **static files only** (HTML, CSS, images). The real **`pseudocode`** program is a **C** binary, so it cannot run inside the browser on this site the way some JavaScript “playgrounds” do.

You can still **try the full interpreter online** using a free **cloud development environment** that clones this repository and runs the same commands as on your laptop.

---

## Option 1 — GitHub Codespaces (recommended)

**GitHub Codespaces** opens a VS Code–style editor in your browser with a Linux terminal, **gcc**, and this repo already checked out.

1. **Sign in** to [GitHub](https://github.com).
2. Open a new Codespace for this project (replace `OWNER/REPO` if you use a fork):

   **[Create Codespace for `mravariya/pseudocode`](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mravariya%2Fpseudocode)**

3. Wait for the container to finish **post-create** (it runs **`./scripts/build.sh`** automatically when using the dev container in this repo).
4. In the **terminal**:

   ```bash
   ./build/pseudocode run examples/hello.pseudo
   ./build/pseudocode repl
   ```

5. Edit **`examples/hello.pseudo`** or create **`my.pseudo`** in the file tree, save, and run **`./build/pseudocode run my.pseudo`**.

**Notes**

- Free tier has monthly minutes; see [GitHub Codespaces billing](https://docs.github.com/en/billing/concepts/product-billing/github-codespaces).
- If the build did not run automatically, execute: **`chmod +x scripts/build.sh && ./scripts/build.sh`**.

---

## Option 2 — Gitpod

**Gitpod** can open the same repository in a browser workspace (account may be required).

**[Open in Gitpod](https://gitpod.io/#https://github.com/mravariya/pseudocode)**

Then in the terminal:

```bash
chmod +x scripts/build.sh
./scripts/build.sh
./build/pseudocode repl
```

---

## Option 3 — Install on your computer

For unlimited offline use, build locally: **[Installation](installation.md)** and **[Getting started](getting-started.md)**.

---

## Why there is no “Run” button on this website

Running pseudocode would require either:

- **WebAssembly** — compiling the whole C interpreter to WASM (possible, but not shipped yet), or  
- **A server** — executing user code on a backend (not available on GitHub Pages).

The links above give you the **real** interpreter in a full Linux environment instead.

---

## See also

- [Tutorial](tutorial/index.md) — language walkthrough after you can run programs  
- [CLI](cli.md) — `run`, `check`, `repl`, flags  
- [Development rebuild loop](getting-started.md#development-rebuild-loop) — **`./scripts/dev.sh`** after changing `src/`
