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
from html import unescape
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
<link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&family=Outfit:wght@500;600;700;800&family=Source+Sans+3:ital,wght@0,400;0,600;0,700;1,400&display=swap" rel="stylesheet">
""".strip()


def esc_attr(s: str) -> str:
    return html.escape(s, quote=True)


def syllabus_codes(cfg: dict) -> str:
    """Short labels for SEO titles and JSON-LD (config key optional)."""
    return (cfg.get("syllabus_codes") or "CS0478, CS9618, IT9626").strip()


def canonical_base(cfg: dict) -> str:
    b = (cfg.get("canonical_base") or "").strip()
    if b and not b.endswith("/"):
        b += "/"
    return b


def abs_url(cfg: dict, relpath: str) -> str:
    base = canonical_base(cfg)
    if not base:
        return ""
    return base + relpath.lstrip("/")


def safe_json_ld(obj: dict) -> str:
    s = json.dumps(obj, ensure_ascii=False)
    return s.replace("</", "<\\/")


def excerpt_from_md(md: str, fallback: str, max_len: int = 158) -> str:
    for line in md.splitlines():
        s = line.strip()
        if not s or s.startswith("#") or s.startswith("---") or s.startswith("|"):
            continue
        s = re.sub(r"\[([^\]]+)\]\([^)]+\)", r"\1", s)
        s = re.sub(r"[*_`]", "", s)
        if len(s) > max_len:
            s = s[: max_len - 1].rsplit(" ", 1)[0] + "…"
        return s
    return fallback[:max_len] if len(fallback) <= max_len else fallback[: max_len - 1] + "…"


def decorate_code_blocks(html: str) -> str:
    html = html.replace(
        '<pre><code class="language-text">',
        '<pre class="code-block code-pseudo"><code class="language-pseudocode">',
    )
    html = html.replace("<pre><code>", '<pre class="code-block code-pseudo"><code class="language-pseudocode">')
    # Fenced blocks: language-specific chrome (bash vs pseudocode vs generic)
    html = html.replace(
        '<pre><code class="language-bash">',
        '<pre class="code-block code-shell"><code class="language-bash">',
    )
    html = html.replace(
        '<pre><code class="language-sh">',
        '<pre class="code-block code-shell"><code class="language-sh">',
    )
    html = html.replace(
        '<pre><code class="language-pseudocode">',
        '<pre class="code-block code-pseudo"><code class="language-pseudocode">',
    )
    html = html.replace(
        '<pre><code class="language-',
        '<pre class="code-block code-generic"><code class="language-',
    )
    return html


# —— Pseudocode syntax colours (tokenize raw source → safe HTML spans) ——
_PC_KW = frozenset(
    """
    declare constant integer real string char boolean date array of type endtype
    if then else endif case endcase otherwise for to step next repeat until while endwhile
    procedure endprocedure function endfunction returns call return byval byref
    input output openfile closefile readfile writefile seek getrecord putrecord
    and or not mod div true false
    """.split()
)

_PC_BUILTIN = frozenset(
    """
    length mid substring right concat int tointeger toreal random rand round
    uppercase lowercase ucase lcase eof
    sqrt sin cos tan asin acos atan atan2 hypot log log10 exp pow floor ceil trunc fmod modreal
    abs pi euler e_const radians degrees time clockms nowstring getenv system
    randomseed seedrandom seed randint randomint
    """.split()
)


def _pseudocode_highlight_raw(src: str) -> str:
    i = 0
    n = len(src)
    parts: list[str] = []
    while i < n:
        c = src[i]
        if c in " \t\r":
            parts.append(html.escape(c))
            i += 1
            continue
        if c == "\n":
            parts.append("\n")
            i += 1
            continue
        if c == "/" and i + 1 < n and src[i + 1] == "/":
            j = src.find("\n", i)
            if j < 0:
                j = n
            parts.append(f'<span class="pc-comment">{html.escape(src[i:j])}</span>')
            i = j
            continue
        if c == '"':
            j = i + 1
            while j < n and src[j] != '"':
                if src[j] == "\\" and j + 1 < n:
                    j += 2
                else:
                    j += 1
            if j < n and src[j] == '"':
                j += 1
            parts.append(f'<span class="pc-str">{html.escape(src[i:j])}</span>')
            i = j
            continue
        if c.isdigit() or (c == "." and i + 1 < n and src[i + 1].isdigit()):
            j = i + 1
            while j < n and (src[j].isdigit() or src[j] == "."):
                j += 1
            parts.append(f'<span class="pc-num">{html.escape(src[i:j])}</span>')
            i = j
            continue
        if c.isalpha() or c == "_":
            j = i + 1
            while j < n and (src[j].isalnum() or src[j] == "_"):
                j += 1
            word = src[i:j]
            wl = word.lower()
            if wl in _PC_KW:
                parts.append(f'<span class="pc-kw">{html.escape(word)}</span>')
            elif wl in _PC_BUILTIN:
                parts.append(f'<span class="pc-builtin">{html.escape(word)}</span>')
            else:
                parts.append(html.escape(word))
            i = j
            continue
        if c == "<" and i + 1 < n and src[i + 1] == "-":
            parts.append(f'<span class="pc-op">{html.escape("<-")}</span>')
            i += 2
            continue
        if c == "\u2190":  # ←
            parts.append(f'<span class="pc-op">{html.escape(c)}</span>')
            i += 1
            continue
        parts.append(html.escape(c))
        i += 1
    return "".join(parts)


def _shell_highlight_lines(src: str) -> str:
    lines = src.split("\n")
    out_lines: list[str] = []
    for line in lines:
        if not line:
            out_lines.append("")
            continue
        hash_i = line.find("#")
        if hash_i >= 0:
            core, com = line[:hash_i], line[hash_i:]
            out_lines.append(_shell_highlight_strings(core) + f'<span class="sh-comment">{html.escape(com)}</span>')
        else:
            out_lines.append(_shell_highlight_strings(line))
    return "\n".join(out_lines)


def _shell_highlight_strings(seg: str) -> str:
    if not seg:
        return ""
    parts: list[str] = []
    i = 0
    n = len(seg)
    while i < n:
        c = seg[i]
        if c == '"':
            j = i + 1
            while j < n and seg[j] != '"':
                if seg[j] == "\\" and j + 1 < n:
                    j += 2
                else:
                    j += 1
            if j < n:
                j += 1
            parts.append(f'<span class="sh-str">{html.escape(seg[i:j])}</span>')
            i = j
            continue
        if c == "'":
            j = i + 1
            while j < n and seg[j] != "'":
                if seg[j] == "\\" and j + 1 < n:
                    j += 2
                else:
                    j += 1
            if j < n:
                j += 1
            parts.append(f'<span class="sh-str">{html.escape(seg[i:j])}</span>')
            i = j
            continue
        parts.append(html.escape(c))
        i += 1
    return "".join(parts)


def highlight_fenced_code(html: str) -> str:
    """Add semantic spans inside pseudocode and shell fenced blocks."""

    def pseudo_sub(m: re.Match) -> str:
        inner = m.group(1)
        raw = unescape(inner)
        return f'<pre class="code-block code-pseudo"><code class="language-pseudocode">{_pseudocode_highlight_raw(raw)}</code></pre>'

    html = re.sub(
        r'<pre class="code-block code-pseudo"><code class="language-pseudocode">(.*?)</code></pre>',
        pseudo_sub,
        html,
        flags=re.DOTALL,
    )

    html = re.sub(
        r'<pre class="code-block code-shell"><code class="language-bash">(.*?)</code></pre>',
        lambda m: f'<pre class="code-block code-shell"><code class="language-bash">{_shell_highlight_lines(unescape(m.group(1)))}</code></pre>',
        html,
        flags=re.DOTALL,
    )
    html = re.sub(
        r'<pre class="code-block code-shell"><code class="language-sh">(.*?)</code></pre>',
        lambda m: f'<pre class="code-block code-shell"><code class="language-sh">{_shell_highlight_lines(unescape(m.group(1)))}</code></pre>',
        html,
        flags=re.DOTALL,
    )
    return html


def build_seo_block(
    cfg: dict,
    *,
    page_title: str,
    description: str,
    path: str,
    extra_json_ld: list[str] | None = None,
) -> str:
    parts: list[str] = []
    parts.append(f'<meta name="description" content="{esc_attr(description)}">')
    kw = cfg.get("keywords") or ""
    if kw:
        parts.append(f'<meta name="keywords" content="{esc_attr(kw)}">')
    if cfg.get("author"):
        parts.append(f'<meta name="author" content="{esc_attr(cfg["author"])}">')
    parts.append('<meta name="robots" content="index, follow, max-image-preview:large">')
    full = abs_url(cfg, path)
    if full:
        parts.append(f'<link rel="canonical" href="{esc_attr(full)}">')
    parts.append('<meta property="og:type" content="website">')
    parts.append(f'<meta property="og:title" content="{esc_attr(page_title)}">')
    parts.append(f'<meta property="og:description" content="{esc_attr(description)}">')
    if full:
        parts.append(f'<meta property="og:url" content="{esc_attr(full)}">')
    parts.append(f'<meta property="og:site_name" content="{esc_attr(cfg["site_name"])}">')
    parts.append('<meta property="og:locale" content="en_GB">')
    og_img = (cfg.get("og_image") or "").strip()
    if og_img:
        parts.append(f'<meta property="og:image" content="{esc_attr(og_img)}">')
    parts.append('<meta name="twitter:card" content="summary_large_image">')
    parts.append(f'<meta name="twitter:title" content="{esc_attr(page_title)}">')
    parts.append(f'<meta name="twitter:description" content="{esc_attr(description)}">')
    tw = (cfg.get("twitter_site") or "").strip()
    if tw:
        parts.append(f'<meta name="twitter:site" content="{esc_attr(tw)}">')
    if og_img:
        parts.append(f'<meta name="twitter:image" content="{esc_attr(og_img)}">')
    parts.append('<meta name="theme-color" content="#2d1b69">')
    parts.append('<meta name="color-scheme" content="dark light">')
    for blob in extra_json_ld or []:
        parts.append(f'<script type="application/ld+json">{blob}</script>')
    return "\n  ".join(parts)


def json_ld_software(cfg: dict) -> str:
    u = abs_url(cfg, "index.html")
    repo = f"https://github.com/{cfg['github_user']}/{cfg['github_repo']}"
    obj: dict = {
        "@context": "https://schema.org",
        "@type": "SoftwareApplication",
        "name": f"{cfg['site_name']} — {syllabus_codes(cfg)} reference interpreter",
        "operatingSystem": "Windows, macOS, Linux",
        "applicationCategory": "EducationalApplication",
        "offers": {"@type": "Offer", "price": "0", "priceCurrency": "USD"},
        "description": cfg["description"],
        "url": u or repo,
        "codeRepository": repo,
        "programmingLanguage": "C",
        "runtimePlatform": "Native CLI",
    }
    if cfg.get("author"):
        obj["author"] = {"@type": "Person", "name": cfg["author"]}
    if cfg.get("keywords"):
        obj["keywords"] = cfg["keywords"]
    obj["featureList"] = [
        f"{syllabus_codes(cfg)} pseudocode program execution",
        "Static syntax check for CI",
        "Interactive REPL",
        "Built-in maths, random, time, and environment helpers",
        "Optional pkg catalog packages",
        "VS Code extension and snippets",
    ]
    lic = abs_url(cfg, "license.html")
    obj["license"] = lic or "https://opensource.org/licenses/MIT"
    return safe_json_ld(obj)


def json_ld_faq() -> str:
    faq = {
        "@context": "https://schema.org",
        "@type": "FAQPage",
        "mainEntity": [
            {
                "@type": "Question",
                "name": "What Cambridge syllabi does this pseudocode suit?",
                "acceptedAnswer": {
                    "@type": "Answer",
                    "text": "The same Cambridge International pseudocode style appears across IGCSE Computer Science (0478, CS0478), AS & A Level Computer Science (9618, CS9618), and AS & A Level Information Technology (9626, IT9626). This site documents a free, open-source interpreter that runs that notation on your computer; always follow your official syllabus and teacher for exams.",
                },
            },
            {
                "@type": "Question",
                "name": "Is this interpreter free to use?",
                "acceptedAnswer": {
                    "@type": "Answer",
                    "text": "Yes. The project is open source under the MIT License. You can run programs locally, use the REPL, and follow the tutorial and language reference at no cost.",
                },
            },
            {
                "@type": "Question",
                "name": "Does this replace the official Cambridge pseudocode guide?",
                "acceptedAnswer": {
                    "@type": "Answer",
                    "text": "No. Always follow your teacher and the official syllabus materials for exams. This interpreter is a practice tool; a few teaching extensions are documented separately from strict exam style.",
                },
            },
        ],
    }
    return safe_json_ld(faq)


def json_ld_website(cfg: dict) -> str:
    base = canonical_base(cfg)
    obj: dict = {
        "@context": "https://schema.org",
        "@type": "WebSite",
        "name": cfg["site_name"],
        "description": cfg["description"],
    }
    if base:
        obj["url"] = base.rstrip("/")
    return safe_json_ld(obj)


def write_sitemap_and_robots(cfg: dict, public: Path) -> None:
    base = canonical_base(cfg)
    if not base:
        return
    urls = sorted(p.relative_to(public).as_posix() for p in public.rglob("*.html"))
    lines = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">',
    ]
    for u in urls:
        loc = abs_url(cfg, u)
        lines.append("  <url>")
        lines.append(f"    <loc>{html.escape(loc)}</loc>")
        lines.append("    <changefreq>weekly</changefreq>")
        lines.append("  </url>")
    lines.append("</urlset>")
    (public / "sitemap.xml").write_text("\n".join(lines) + "\n", encoding="utf-8")
    robots = (
        "User-agent: *\n"
        "Allow: /\n\n"
        f"Sitemap: {abs_url(cfg, 'sitemap.xml')}\n"
    )
    (public / "robots.txt").write_text(robots, encoding="utf-8")


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
    seo_block: str = "",
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
        .replace("{{seo_block}}", seo_block)
        .replace("{{extra_head}}", extra_head)
    )


def finalize_page(html_page: str, cfg: dict, from_pub: str) -> str:
    """Inject repo URL, JS path, Cambridge links (templates use {{placeholders}})."""
    repo = f"https://github.com/{cfg['github_user']}/{cfg['github_repo']}"
    js = rel_href(from_pub, "assets/site.js")
    spec = (cfg.get("cambridge_spec_url") or "").strip()
    igcse = (cfg.get("cambridge_igcse_cs_url") or "").strip()
    it_url = (cfg.get("cambridge_it_url") or "").strip()
    return (
        html_page.replace("{{repo_url}}", html.escape(repo))
        .replace("{{js_href}}", html.escape(js))
        .replace("{{cambridge_spec_url}}", html.escape(spec))
        .replace("{{cambridge_igcse_cs_url}}", html.escape(igcse))
        .replace("{{cambridge_it_url}}", html.escape(it_url))
    )


def nav_items(from_public_path: str, cfg: dict) -> list[tuple[str, str]]:
    """Label → target path relative to public/."""
    items = [
        ("Home", "index.html"),
        ("Docs", "docs/index.html"),
        ("Tutorial", "docs/tutorial/index.html"),
        ("Try online", "docs/try-online.html"),
        ("Language", "docs/reference/language.html"),
        ("Built-ins", "docs/reference/builtins.html"),
        ("Stdlib", "docs/stdlib/index.html"),
        ("Install", "docs/installation.html"),
        ("Brew & packages", "docs/package-managers.html"),
        ("Packages", "docs/package-manager.html"),
        ("Contributing", "contributing.html"),
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
    landing_body = highlight_fenced_code(decorate_code_blocks(md_to_html_fragment(landing_md)))
    out_home = "index.html"
    css = asset_href(out_home)
    nav = build_nav(out_home, cfg)
    home_href = rel_href(out_home, "index.html")
    home_title = (
        f"{cfg['site_name']} | {syllabus_codes(cfg)} — Cambridge pseudocode interpreter & tutorial"
    )
    home_seo = build_seo_block(
        cfg,
        page_title=home_title,
        description=cfg["description"],
        path="index.html",
        extra_json_ld=[
            json_ld_software(cfg),
            json_ld_faq(),
            json_ld_website(cfg),
        ],
    )
    page = render_page(
        landing_tpl,
        title=home_title,
        site_name=cfg["site_name"],
        body=f'<div class="prose landing-inner">{landing_body}</div>',
        active="home",
        home_href=home_href,
        css_href=css,
        nav_html=nav,
        seo_block=home_seo,
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
        body = highlight_fenced_code(decorate_code_blocks(md_to_html_fragment(text)))
        title_match = re.search(r"^#\s+(.+)$", text, re.MULTILINE)
        page_title = title_match.group(1).strip() if title_match else rel
        out_rel = Path(rel).with_suffix(".html").as_posix()
        out_path = PUBLIC / "docs" / out_rel
        out_path.parent.mkdir(parents=True, exist_ok=True)
        from_pub = os.path.join("docs", out_rel)
        css = asset_href(from_pub)
        nav = build_nav(from_pub, cfg)
        home_href = rel_href(from_pub, "index.html")
        doc_title = f"{page_title} — {cfg['site_name']} | {syllabus_codes(cfg)} pseudocode"
        doc_desc = excerpt_from_md(text, cfg["description"])
        doc_seo = build_seo_block(
            cfg,
            page_title=doc_title,
            description=doc_desc,
            path=from_pub.replace("\\", "/"),
        )
        wrapped = render_page(
            layout,
            title=doc_title,
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{body}</article></main>',
            active="docs",
            home_href=home_href,
            css_href=css,
            nav_html=nav,
            seo_block=doc_seo,
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
        body = highlight_fenced_code(decorate_code_blocks(md_to_html_fragment(t)))
        from_pub = name
        aux_title = f"{title} — {cfg['site_name']} | {syllabus_codes(cfg)} pseudocode project"
        aux_desc = excerpt_from_md(t, f"{title} — {cfg['site_name']}.")
        aux_seo = build_seo_block(cfg, page_title=aux_title, description=aux_desc, path=name)
        page = render_page(
            layout,
            title=aux_title,
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{body}</article></main>',
            active="contrib" if "contributing" in name else "docs",
            home_href=rel_href(from_pub, "index.html"),
            css_href=asset_href(from_pub),
            nav_html=build_nav(from_pub, cfg),
            seo_block=aux_seo,
        )
        page = finalize_page(page, cfg, from_pub)
        (PUBLIC / name).write_text(page, encoding="utf-8")

    # —— LICENSE (plain text) ——
    lic = ROOT / "LICENSE"
    if lic.exists():
        raw = lic.read_text(encoding="utf-8")
        lic_body = f"<pre class=\"license-pre\">{html.escape(raw)}</pre>"
        from_pub = "license.html"
        lic_title = f"MIT License — {cfg['site_name']} | Open-source {syllabus_codes(cfg)} pseudocode interpreter"
        lic_seo = build_seo_block(
            cfg,
            page_title=lic_title,
            description="MIT License for the Pseudocode reference interpreter project.",
            path="license.html",
        )
        page = render_page(
            layout,
            title=lic_title,
            site_name=cfg["site_name"],
            body=f'<main id="main" class="doc-main"><article class="prose">{lic_body}</article></main>',
            active="license",
            home_href=rel_href(from_pub, "index.html"),
            css_href=asset_href(from_pub),
            nav_html=build_nav(from_pub, cfg),
            seo_block=lic_seo,
        )
        page = finalize_page(page, cfg, from_pub)
        (PUBLIC / "license.html").write_text(page, encoding="utf-8")

    # Tell GitHub Pages not to run Jekyll on this static export (belt-and-suspenders).
    (PUBLIC / ".nojekyll").write_text("", encoding="utf-8")

    write_sitemap_and_robots(cfg, PUBLIC)
    nhtml = sum(1 for _ in PUBLIC.rglob("*.html"))
    print(f"Built site → {PUBLIC} ({nhtml} HTML files, sitemap + robots if canonical_base set)")


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
