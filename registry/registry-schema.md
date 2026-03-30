# Pseudocode package registry (design)

This document describes a **future-facing** HTTP registry for `pkg`, similar in spirit to PyPI or npm.

## Base URL

Configure one or more registry base URLs in `~/.pseudocode/registry.txt` (one URL per line). Example:

```
https://registry.pseudocode.dev/v1
```

## Index endpoint

`GET /packages.json` returns a JSON array of package metadata:

```json
[
  {
    "name": "cs9618-helpers",
    "version": "1.0.0",
    "description": "Utilities for A Level 9618 practice",
    "tarball_url": "https://.../cs9618-helpers-1.0.0.tar.gz",
    "sha256": "...",
    "dependencies": {}
  }
]
```

## Tarball layout

Each package archive should expand to a directory containing `.pseudo` or `.pseudocode` source files (and optional `manifest.json` with name, version, dependencies).

## Local cache

`pkg install` may cache tarballs under `~/.pseudocode/cache/<name>-<version>.tar.gz` before extracting into `~/.pseudocode/packages/<name>/`.

## Versioning

Semantic versioning (`MAJOR.MINOR.PATCH`) is recommended. The CLI may later support `pkg install name@1.2.0`.

The current reference implementation ships **local folder install** only; online fetch can be layered on this schema without breaking the CLI.
