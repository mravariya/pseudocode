# Agent session log (local file, format spec)

This page describes a **local-only** work log for Cursor/agent sessions. It does **not** contain your private session history (that file is **not** in git).

## Where the log lives

| Item | Path | On GitHub? |
|------|------|------------|
| **Your running log** | **`AGENT.local.md`** (repository root, next to `README.md`) | **No** — listed in **`.gitignore`** |
| **This specification** | **`docs/agent-session-log.md`** | **Yes** — format and rules only |

**Why not on the site with live chat content?** Anything in the public repo (and thus GitHub Pages) is world-readable. Session text can paraphrase requests and list file paths; keeping **`AGENT.local.md`** gitignored avoids publishing that by accident. The **table layout** below is what you paste into **`AGENT.local.md`**; the same Markdown style is used so it reads like the rest of the manual.

---

## Non‑negotiable agent behaviour

After **substantive** work (edits, commands, or multi-step answers), the agent should **append** a new block to **`AGENT.local.md`** (create the file if missing) containing:

1. **Refined request** — one short, improved sentence restating what you asked for.  
2. **Actions taken** — bullet list of what was done (files, commands, outcomes).  
3. **Permissions** — e.g. sandbox, `git_write`, `network`, `all`, or “none”.  
4. **Checklist table** — docs / code / site / git columns with ✅ or N/A and brief notes.  
5. **Git** — if pushed: **commit hash** (short `abc1234`) and branch (e.g. `main`).  
6. **Context** — anything useful for the next session (paths, caveats, follow-ups) **without** waiting for you to ask.

If there were **no** repo changes and **no** push, still log a minimal row (e.g. “advice only”) so the trail stays consistent.

---

## Template (copy into `AGENT.local.md`)

```markdown
# Agent session log (local — do not commit)

## Session — YYYY-MM-DD HH:MM (timezone)

### Refined request
> One clear sentence.

### Actions
- …

### Permissions asked / used
- …

### Checklist

| Area | Done | Notes |
|------|:----:|-------|
| Docs updated | ✅ / — | `path/to/file.md` |
| Code changed | ✅ / — | `src/…` |
| Site rebuilt (`python3 site/build.py`) | ✅ / — | N/A if not applicable |
| Git pushed | ✅ / — | `main` @ `abc1234` or “not pushed” |

### Context for next time
- …
```

---

## README pointer

The project **`README.md`** links here so contributors find the rule quickly. The Cursor rule **`.cursor/rules/agent-session-log.mdc`** sets this as a **default** obligation for the agent.

---

## See also

- [Recent collaboration log](recent-sessions.md) — dated theme summary (public, not raw chat)  
- [Contributing](CONTRIBUTING.md)  
- [Implementation notes](implementation.md)
