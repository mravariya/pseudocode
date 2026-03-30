# Website (GitHub Pages)

This folder contains a **small static site generator** (about one Python file plus templates). It is **not Jekyll**: there is no Ruby stack, no `Gemfile`, and nothing heavy to install beyond a single PyPI package.

## What it does

1. Converts **`site/content/index.md`** into the **landing page** (`index.html`).
2. Converts every **`docs/**/*.md`** file into **HTML** under `site/public/docs/`, fixing `.md` links to `.html`.
3. Publishes **`CONTRIBUTING.md`**, **`AUTHORS.md`**, and **`LICENSE`** as pages.
4. Copies **`site/assets/`** (CSS, tiny JS for the mobile menu).
5. Applies **syntax highlighting** to fenced **` ```pseudocode `** and **` ```bash `** blocks (keyword, string, comment colours) for readable snippets.

## Publish in seconds

```bash
pip install -r site/requirements-site.txt
python3 site/build.py
```

Output: **`site/public/`** — upload that folder to any static host, or use the **Deploy GitHub Pages** workflow (`.github/workflows/pages.yml`).

Preview locally:

```bash
python3 site/build.py --serve
# open http://127.0.0.1:8765
```

## Configuration

Edit **`site/config.json`**:

- `github_user` / `github_repo` — footer links, JSON-LD `codeRepository`, and GitHub URL rewrites.
- **`canonical_base`** — full site URL with trailing slash (e.g. `https://user.github.io/repo/`). Drives **canonical links**, **Open Graph / Twitter** URLs, **`sitemap.xml`**, and **`robots.txt`**. If empty, those files are skipped.
- `keywords` — comma-separated **meta keywords** (plus JSON-LD) for search snippets.
- `author` — `<meta name="author">` and structured data.
- `og_image` — optional absolute URL for **og:image** / **Twitter** cards (social previews).
- `twitter_site` — optional `@handle` for **twitter:site**.
- `cambridge_spec_url` — hero link to the official qualification page.
- `site_name`, `site_tagline`, `description` — branding and default meta description.

After changing `canonical_base`, submit **`sitemap.xml`** in [Google Search Console](https://search.google.com/search-console) (and similar tools) so crawlers discover every doc page.

## Customising the landing page

Edit **`site/content/index.md`** only (Markdown). Rebuild. No template edit required for most text and tables.

To change layout, colours, or navigation labels, edit:

- `site/templates/landing.html` — home page shell and hero.
- `site/templates/layout.html` — documentation and auxiliary pages.
- `site/assets/style.css`
- `site/build.py` (`nav_items`, `rewrite_markdown_links`, …)

## GitHub Pages setup

1. In the repository **Settings → Pages**, set **Source** to **GitHub Actions** (recommended).
2. Push to `main`; the **Deploy GitHub Pages** workflow builds and publishes `site/public`.
3. If your site URL is `https://<user>.github.io/<repo>/`, relative links in the built HTML already work. For a **custom domain**, add a `CNAME` in Pages settings and adjust nothing else if the site is served from the repo root of that domain.

## Ignoring build output

`site/public/` is listed in **`.gitignore`**. The live site is produced in CI (or by you locally); you do not need to commit generated HTML.
