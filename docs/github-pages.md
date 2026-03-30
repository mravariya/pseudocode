# GitHub Pages: custom site vs Jekyll README

The polished documentation site is produced by **`python3 site/build.py`** into **`site/public/`** and deployed by the workflow **`.github/workflows/pages.yml`** (“**Deploy GitHub Pages**”).

If **`https://<user>.github.io/<repo>/`** still looks like a **plain README** (Cayman-style title, “Pseudocode interpreter”, grey theme, **no** purple hero or stats strip), GitHub is **not** serving that build. It is almost always serving a **Jekyll** build from the repository instead.

---

## Quick check (5 seconds)

1. Open your Pages URL, e.g. `https://mravariya.github.io/pseudocode/`.
2. **View page source** (right‑click → View Page Source).
3. **Wrong (Jekyll):** you see something like  
   `<meta name="generator" content="Jekyll v3.10.0" />`  
   and `<html lang="en-US">`.
4. **Right (our site):** you see  
   `<html lang="en-GB">`,  
   a long `<title>` such as **“Pseudocode | Cambridge 9618 A Level…”**,  
   and **no** Jekyll generator meta. You should also see classes like **`page-landing`**, **`landing-stats`**, **`hero-title`**.

---

## Fix: only one publisher

### 1. Pages source must be GitHub Actions only

1. Repo **Settings → Pages**.
2. Under **Build and deployment → Source**, choose **GitHub Actions** (not **Deploy from a branch**).
3. If the UI still offers **branch** / **folder** (`/ (root)` or `/docs`), **clear** that so only the Actions workflow drives the site.

You cannot reliably mix **branch + Jekyll** with the **Actions artifact** from `site/public`; the wrong one may win.

### 2. Remove extra “GitHub Pages” workflows

When you first enabled Pages, GitHub may have suggested **“GitHub Pages Jekyll”** or similar. That creates **another** workflow that builds Jekyll from **`README.md`** and also deploys to **`github-pages`**.

1. Open the **Actions** tab.
2. For each workflow **other than** **Deploy GitHub Pages** (from **`pages.yml`**), open it and inspect the YAML:
   - If it contains **`jekyll`**, **`actions/jekyll`**, or **`actions/configure-pages`** together with a Jekyll build, it is **not** our site.
3. **Disable** that workflow (**⋯ → Disable workflow**) or **delete** its file under **`.github/workflows/`** on **`main`** and push.

Keep **only** the workflow that runs:

- `pip install -r site/requirements-site.txt`
- `python3 site/build.py`
- `actions/upload-pages-artifact` with **`path: site/public`**
- `actions/deploy-pages`

### 3. Redeploy

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
