#!/usr/bin/env python3
"""
Lightweight static site generator for GitHub Pages (no Jekyll).
Markdown in site/content/ + repo docs/ → HTML in site/public/

Usage:
  pip install -r site/requirements-site.txt
  python3 site/build.py

Optional:
  python3 site/build.py --serve   # http://127.0.0.1:8765 (stdlib only)
"""
from __future__ import annotations

import argparse
import html
import json
import os
import re
import shutil
import sys
from pathlib import Path

try:
    import markdown
except ImportError:
    print("Install the site dependency:  pip install -r site/requirements-site.txt", file=sys.stderr)
    sys.exit(1)

ROOT = Path(__file__).resolve().parent.parent
SITE = Path(__file__).resolve().parent
PUBLIC = SITE / "public"
CONTENT = SITE / "content"
DOCS_SRC = ROOT / "docs"
TEMPLATES = SITE / "templates"
ASSETS_SRC = SITE / "assets"

MD_EXTENSIONS = [
    "markdown.extensions.tables",
    "markdown.extensions.fenced_code",
    "markdown.extensions.sane_lists",
    "markdown.extensions.nl2br",
]

FONTS_HEAD = """
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&family=Source+Sans+3:ital,wght@0,400;0,600;0,700;1,400&display=swap" rel="stylesheet">
""".strip()


def load_config() -> dict:
    p = SITE / "config.json"
    with open(p, encoding="utf-8") as f:
        return json.load(f)


def github_blob(cfg: dict, path_in_repo: str) -> str:
    u, r = cfg["github_user"], cfg["github_repo"]
    return f"https://github.com/{u}/{r}/blob/main/{path_in_repo}"


def rewrite_markdown_links(text: str, cfg: dict, md_path_rel: str) -> str:
    """Adjust .md links and ../ paths before HTML conversion."""

    def sub_link(m: re.Match) -> str:
        inner = m.group(1).strip()
        if inner.startswith("#"):
            return m.group(0)
        if inner.startswith(("http://", "https://", "mailto:")):
            return m.group(0)
        anchor = ""
        if "#" in inner:
            inner, _, frag = inner.partition("#")
            anchor = "#" + frag
        # Repo-root relative links (keep .md targets on GitHub)
        if inner.startswith("../LICENSE"):
            return f"]({github_blob(cfg, 'LICENSE')}{anchor})"
        if inner.startswith("../AUTHORS.md"):
            return f"]({github_blob(cfg, 'AUTHORS.md')}{anchor})"
        if inner.startswith("../registry/"):
            rest = inner.replace("../", "")
            return f"]({github_blob(cfg, rest)}{anchor})"
        if inner.startswith("../README.md"):
            return f"]({github_blob(cfg, 'README.md')}{anchor})"
        if inner.endswith(".md"):
            inner = inner[:-3] + ".html"
        return f"]({inner}{anchor})"

    text = re.sub(r"\]\(([^)]+)\)", sub_link, text)
    return text


def md_to_html_fragment(source: str) -> str:
    return markdown.markdown(source, extensions=MD_EXTENSIONS, output_format="html5")


def rel_href(from_public_path: str, to_public_path: str) -> str:
    """POSIX href from one output file to another (both relative to PUBLIC)."""
    from_dir = os.path.dirname(from_public_path) or "."
    return Path(os.path.relpath(to_public_path, start=from_dir)).as_posix()


def asset_href(from_public_path: str) -> str:
    return rel_href(from_public_path, "assets/style.css")


def read_template(name: str) -> str:
    return (TEMPLATES / name).read_text(encoding="utf-8")


def render_page(
    template: str,
    *,
    title: str,
    site_name: str,
    body: str,
    active: str,
    home_href: str,
    css_href: str,
    nav_html: str,
    extra_head: str = FONTS_HEAD,
) -> str:
    return (
        template.replace("{{title}}", html.escape(title))
        .replace("{{site_name}}", html.escape(site_name))
        .replace("{{body}}", body)
        .replace("{{active}}", active)
        .replace("{{home_href}}", html.escape(home_href))
        .replace("{{css_href}}", html.escape(css_href))
        .replace("{{nav_html}}", nav_html)
        .replace("{{extra_head}}", extra_head)
    )


def finalize_page(html_page: str, cfg: dict, from_pub: str) -> str:
    """Inject repo URL, JS path, Cambridge link (templates use {{placeholders}})."""
    repo = f"https://github.com/{cfg['github_user']}/{cfg['github_repo']}"
    js = rel_href(from_pub, "assets/site.js")
    spec = cfg.get("cambridge_spec_url", "")
    return (
        html_page.replace("{{repo_url}}", html.escape(repo))
        .replace("{{js_href}}", html.escape(js))
        .replace("{{cambridge_spec_url}}", html.escape(spec))
    )


def nav_items(from_public_path: str, cfg: dict) -> list[tuple[str, str]]:
    """Label → target path relative to public/."""
    items = [
        ("Home", "index.html"),
        ("Documentation", "docs/index.html"),
        ("Tutorial", "docs/tutorial/index.html"),
        ("Language ref", "docs/reference/language.html"),
        ("Built-ins", "docs/reference/builtins.html"),
        ("Install", "docs/installation.html"),
        ("Contributing", "contributing.html"),
        ("Authors", "authors.html"),
        ("License", "license.html"),
    ]
    return [(lab, rel_href(from_public_path, p)) for lab, p in items]


def build_nav(from_public_path: str, cfg: dict) -> str:
    parts = []
    for lab, href in nav_items(from_public_path, cfg):
        parts.append(f'<a class="nav-link" href="{html.escape(href)}">{html.escape(lab)}</a>')
    return "\n".join(parts)


def build(cfg: dict) -> None:
    if PUBLIC.exists():
        shutil.rmtree(PUBLIC)
    PUBLIC.mkdir(parents=True)
    shutil.copytree(ASSETS_SRC, PUBLIC / "assets")

    layout = read_template("layout.html")
    landing_tpl = read_template("landing.html")

    # —— Landing (home) ——
    landing_md = (CONTENT / "index.md").read_text(encoding="utf-8")
    landing_md = rewrite_markdown_links(landing_md, cfg, "content/index.md")
    landing_body = md_to_html_fragment(landing_md)
    out_home = "index.html"
    css = asset_href(out_home)
    nav = build_nav(out_home, cfg)
    home_href = rel_href(out_home, "index.html")
    page = render_page(
        landing_tpl,
        title=cfg["site_tagline"],
        site_name=cfg["site_name"],
        body=f'<div class="prose landing-inner">{landing_body}</div>',
        active="home",
        home_href=home_href,
        css_href=css,
        nav_html=nav,
    )
    page = (
        page.replace("{{site_tagline}}", html.escape(cfg["site_tagline"]))
        .replace("{{description}}", html.escape(cfg["description"]))
    )
    page = finalize_page(page, cfg, out_home)
    (PUBLIC / "index.html").write_text(page, encoding="utf-8")

    # —— Manual: docs/**/*.md ——
    for md_path in sorted(DOCS_SRC.rglob("*.md")):
        if md_path.name == "README.md":
            continue
        rel = md_path.relative_to(DOCS_SRC).as_posix()
        text = md_path.read_text(encoding="utf-8")
        text = rewrite_markdown_links(text, cfg, f"docs/{rel}")
        body = md_to_html_fragment(text)
        title_match = re.search(r"^#\s+(.+)$", text, re.MULTILINE)
        page_title = title_match.group(1).strip() if title_match else rel
        out_rel = Path(rel).with_suffix(".html").as_posix()
        out_path = PUBLIC / "docs" / out_rel
        out_path.parent.mkdir(parents=True, exist_ok=True)
        from_pub = os.path.join("docs", out_rel)
        css = asset_href(from_pub)
        nav = build_nav(from_pub, cfg)
        home_href = rel_href(from_pub, "index.html")
        doc_title = f"{page_title} — {cfg['site_name']}"
        wrapped = render_page(
            layout,
            title=doc_title,
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{body}</article></main>',
            active="docs",
            home_href=home_href,
            css_href=css,
            nav_html=nav,
        )
        wrapped = finalize_page(wrapped, cfg, from_pub)
        out_path.write_text(wrapped, encoding="utf-8")

    # —— CONTRIBUTING ——
    for name, src_name, title in (
        ("contributing.html", "CONTRIBUTING.md", "Contributing"),
        ("authors.html", "AUTHORS.md", "Authors"),
    ):
        src = ROOT / src_name
        if not src.exists():
            continue
        t = src.read_text(encoding="utf-8")
        t = rewrite_markdown_links(t, cfg, src_name)
        body = md_to_html_fragment(t)
        from_pub = name
        page = render_page(
            layout,
            title=f"{title} — {cfg['site_name']}",
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{body}</article></main>',
            active="contrib" if "contributing" in name else "docs",
            home_href=rel_href(from_pub, "index.html"),
            css_href=asset_href(from_pub),
            nav_html=build_nav(from_pub, cfg),
        )
        page = finalize_page(page, cfg, from_pub)
        (PUBLIC / name).write_text(page, encoding="utf-8")

    # —— LICENSE (plain text) ——
    lic = ROOT / "LICENSE"
    if lic.exists():
        raw = lic.read_text(encoding="utf-8")
        lic_body = f"<pre class=\"license-pre\">{html.escape(raw)}</pre>"
        from_pub = "license.html"
        page = render_page(
            layout,
            title=f"License — {cfg['site_name']}",
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{lic_body}</article></main>',
            active="license",
            home_href=rel_href(from_pub, "index.html"),
            css_href=asset_href(from_pub),
            nav_html=build_nav(from_pub, cfg),
        )
        page = finalize_page(page, cfg, from_pub)
        (PUBLIC / "license.html").write_text(page, encoding="utf-8")

    print(f"Built site → {PUBLIC} ({sum(1 for _ in PUBLIC.rglob('*.html'))} HTML files)")


def serve() -> None:
    import http.server
    import socketserver

    os.chdir(PUBLIC)
    handler = http.server.SimpleHTTPRequestHandler
    with socketserver.TCPServer(("", 8765), handler) as httpd:
        print("Serving site/public at http://127.0.0.1:8765  (Ctrl+C to stop)")
        httpd.serve_forever()


def main() -> None:
    ap = argparse.ArgumentParser(description="Build static site to site/public/")
    ap.add_argument("--serve", action="store_true", help="serve site/public on :8765 after build")
    args = ap.parse_args()
    cfg = load_config()
    build(cfg)
    if args.serve:
        serve()


if __name__ == "__main__":
    main()
