# GitHub Pages: custom site vs Jekyll README

The polished documentation site is produced by **`python3 site/build.py`** into **`site/public/`** and deployed by the workflow **`.github/workflows/pages.yml`** (“**Deploy GitHub Pages**”).

If **`https://<user>.github.io/<repo>/`** still looks like a **plain README** (Cayman-style title, “Pseudocode interpreter”, grey theme, **no** purple hero or stats strip), GitHub is **not** serving that build. It is almost always serving a **Jekyll** build from the repository instead.

---

## What we checked on GitHub (no login needed)

Using GitHub’s **public API**, the **`pseudocode`** repo currently has **only** these workflow files under **`.github/workflows/`**:

- **`build.yml`** — compiles the C interpreter on Ubuntu + macOS. It does **not** publish Pages. **Keep it enabled** for CI.
- **`pages.yml`** — builds **`site/public`** and deploys to Pages. **This is the one you want for the website.**

There is **no** extra Jekyll workflow file in the repo to delete. The plain README look comes from **repository Settings** still using **“Deploy from a branch”** (GitHub’s built‑in Jekyll on the server), which runs **even though** `pages.yml` also runs.

**You cannot fix this by disabling Actions workflows** — you must switch **Pages → Source** to **GitHub Actions** only (see below).

From your machine, run:

```bash
chmod +x scripts/check-pages-remote.sh
./scripts/check-pages-remote.sh
```

It re-checks the API and fetches the live homepage to say whether Jekyll or the custom site is showing.

---

## Fix in one place (repo admin, browser)

1. **Open (while logged into GitHub):**  
   **`https://github.com/mravariya/pseudocode/settings/pages`**  
   (replace `mravariya/pseudocode` if you use a fork.)

2. Find **“Build and deployment”**.

3. **Source** must be **GitHub Actions** — not **Deploy from a branch**.

4. If you see **Branch: `main`** and **Folder: `/ (root)`** (or `/docs`), change the dropdown to **GitHub Actions**. That turns off the server-side Jekyll build from your README.

5. Under **“GitHub Actions”**, ensure the suggested workflow is **Deploy GitHub Pages** / **`pages.yml`** (or leave default; your repo already has the file).

6. Push to **`main`** or re-run **Deploy GitHub Pages** from the Actions tab. Wait a minute, then hard-refresh the site.

---

## Quick check (5 seconds)

1. Open your Pages URL, e.g. `https://mravariya.github.io/pseudocode/`.
2. **View page source** (right‑click → View Page Source).
3. **Wrong (Jekyll):** you see something like  
   `<meta name="generator" content="Jekyll v3.10.0" />`  
   and `<html lang="en-US">`.
4. **Right (our site):** you see  
   `<html lang="en-GB">`,  
   a long `<title>` such as **“Pseudocode | CS0478, CS9618, IT9626 …”**,  
   and **no** Jekyll generator meta. You should also see classes like **`page-landing`**, **`landing-stats`**, **`hero-title`**.

---

## Fix: only one publisher (summary)

If you already followed **[Fix in one place (repo admin, browser)](#fix-in-one-place-repo-admin-browser)** above, you are done with Settings. Do **not** disable **`build.yml`** — it does not publish Pages.

### 1. Workflows: what to leave on

| Workflow | Purpose | Disable? |
|----------|---------|----------|
| **Deploy GitHub Pages** (`pages.yml`) | Builds **`site/public`** and publishes the real docs site | **No** — keep on |
| **build** (`build.yml`) | Compiles **`pseudocode`** / **`pkg`** on Ubuntu + macOS | **No** — keep on (not related to Pages) |

If you **ever** add another workflow that runs **Jekyll** or deploys **`github-pages`** from **`README.md`**, disable **that** one — but in this repository there is **no** such file on GitHub right now.

### 2. Redeploy

After the above, run **Deploy GitHub Pages** manually (**Actions → Deploy GitHub Pages → Run workflow**) or push any commit to **`main`**.

Wait a minute, hard‑refresh the site (or use a private window), then run the **View page source** check again.

---

## What you should see when it is correct

- Dark themed **landing** with gradient hero, **stats** row (MIT / C11 / REPL / pkg), **feature cards**, and **colourful** fenced code blocks on the home page.
- **Docs** at **`/pseudocode/docs/`** as HTML generated from **`docs/*.md`**.

Local preview (same output as CI):

```bash
pip install -r site/requirements-site.txt
python3 site/build.py --serve
# http://127.0.0.1:8765
```

---

## Duplicate rows under “Deployments”

Seeing both **pages-build-deployment** and **Deploy GitHub Pages** is common; the important part is **which files** actually get published. Use the **generator / `lang` / title** test above—not the deployment label alone.

---

## See also

- [site/README.md](../site/README.md) — build and `config.json`
- [Contributing](../CONTRIBUTING.md) — `./scripts/dev.sh --site` to rebuild the site locally
