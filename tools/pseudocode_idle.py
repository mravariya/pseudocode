#!/usr/bin/env python3
"""
Pseudocode IDLE — a small Tkinter shell + editor (stdlib only; no PyQt/Jekyll).

Looks a bit like Python's IDLE: script window, Run, output pane.

Requires: Python 3 with tkinter (macOS framework build; Linux python3-tk).

Set PSEUDOCODE_EXE to the full path of the `pseudocode` binary if it is not on PATH.
"""
from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path
import tkinter as tk
from tkinter import font as tkfont
from tkinter import messagebox, scrolledtext, ttk


def find_pseudocode_exe() -> str:
    env = os.environ.get("PSEUDOCODE_EXE", "").strip()
    if env and Path(env).is_file():
        return env
    here = Path(__file__).resolve().parent.parent
    for candidate in (
        here / "build" / "pseudocode",
        here / "build" / "pseudocode.exe",
    ):
        if candidate.is_file():
            return str(candidate)
    return "pseudocode"


DEFAULT_SCRIPT = '''// Pseudocode — press F5 or Run to execute (saves to a temp file)

OUTPUT "Hello from Pseudocode IDLE!"
'''


class PseudoIdle(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.title("Pseudocode IDLE")
        self.geometry("920x640")
        self.exe = find_pseudocode_exe()
        self._build_ui()
        self.editor.insert("1.0", DEFAULT_SCRIPT)

    def _build_ui(self) -> None:
        menubar = tk.Menu(self)
        file_m = tk.Menu(menubar, tearoff=0)
        file_m.add_command(label="Exit", command=self.destroy)
        menubar.add_cascade(label="File", menu=file_m)
        run_m = tk.Menu(menubar, tearoff=0)
        run_m.add_command(label="Run module", accelerator="F5", command=self.run_module)
        run_m.add_command(label="Check only", accelerator="F6", command=self.check_only)
        menubar.add_cascade(label="Run", menu=run_m)
        help_m = tk.Menu(menubar, tearoff=0)
        help_m.add_command(label="About", command=self.about)
        menubar.add_cascade(label="Help", menu=help_m)
        self.config(menu=menubar)

        status = ttk.Frame(self)
        status.pack(side=tk.BOTTOM, fill=tk.X)
        self.status = ttk.Label(status, text=f"Interpreter: {self.exe}", anchor=tk.W)
        self.status.pack(side=tk.LEFT, padx=6, pady=2)

        paned = ttk.Panedwindow(self, orient=tk.VERTICAL)
        paned.pack(fill=tk.BOTH, expand=True)

        ed_frame = ttk.Frame(paned)
        ttk.Label(ed_frame, text="Script (UTF-8)").pack(anchor=tk.W, padx=4)
        mono = tkfont.nametofont("TkFixedFont").actual()
        self.editor = scrolledtext.ScrolledText(
            ed_frame,
            wrap=tk.NONE,
            undo=True,
            font=(mono["family"], max(11, int(mono["size"]))),
            bg="#1e1e1e",
            fg="#d4d4d4",
            insertbackground="#ffffff",
            selectbackground="#264f78",
        )
        self.editor.pack(fill=tk.BOTH, expand=True, padx=4, pady=4)
        paned.add(ed_frame, weight=3)

        out_frame = ttk.Frame(paned)
        ttk.Label(out_frame, text="Shell / output").pack(anchor=tk.W, padx=4)
        self.output = scrolledtext.ScrolledText(
            out_frame,
            height=12,
            wrap=tk.WORD,
            font=(mono["family"], max(10, int(mono["size"]) - 1)),
            bg="#0d1117",
            fg="#c9d1d9",
            state=tk.DISABLED,
        )
        self.output.pack(fill=tk.BOTH, expand=True, padx=4, pady=4)
        paned.add(out_frame, weight=2)

        self.bind("<F5>", lambda e: self.run_module())
        self.bind("<F6>", lambda e: self.check_only())

    def _append_output(self, text: str, clear: bool = False) -> None:
        self.output.configure(state=tk.NORMAL)
        if clear:
            self.output.delete("1.0", tk.END)
        self.output.insert(tk.END, text)
        self.output.see(tk.END)
        self.output.configure(state=tk.DISABLED)

    def run_module(self) -> None:
        self._run(["run"], "Run")

    def check_only(self) -> None:
        self._run(["check"], "Check")

    def _run(self, args: list[str], label: str) -> None:
        src = self.editor.get("1.0", "end-1c")
        try:
            fd, path = tempfile.mkstemp(suffix=".pseudo", prefix="pseudo_idle_")
            with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as f:
                f.write(src)
        except OSError as e:
            messagebox.showerror("Pseudocode IDLE", f"Could not write temp file: {e}")
            return

        env = os.environ.copy()
        env.setdefault("PSEUDO_NO_COLOR", "1")
        cmd = [self.exe] + args + [path]
        proc = None
        try:
            proc = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                env=env,
                timeout=120,
                cwd=str(Path.home()),
            )
        except FileNotFoundError:
            messagebox.showerror(
                "Pseudocode IDLE",
                f"Could not run `{self.exe}`.\nBuild the project or set PSEUDOCODE_EXE.",
            )
            return
        except subprocess.TimeoutExpired:
            self._append_output(f"\n--- {label}: timeout ---\n", clear=True)
            return
        finally:
            try:
                Path(path).unlink(missing_ok=True)
            except OSError:
                pass

        if proc is None:
            return
        out = ""
        if proc.stdout:
            out += proc.stdout
        if proc.stderr:
            if out and not out.endswith("\n"):
                out += "\n"
            out += proc.stderr
        if not out.strip():
            out = f"(no output, exit {proc.returncode})\n"
        self._append_output(f"\n--- {label} (exit {proc.returncode}) ---\n{out}", clear=True)
        self.status.configure(
            text=f"Interpreter: {self.exe}  |  last exit: {proc.returncode}  (2=parse 3=runtime 4=I/O 5=usage)"
        )

    def about(self) -> None:
        messagebox.showinfo(
            "Pseudocode IDLE",
            "Pseudocode IDLE\n\n"
            "F5 = run script\n"
            "F6 = check (parse/validate)\n\n"
            "Uses the `pseudocode` CLI. See docs/error-codes.md for exit codes.",
        )


def main() -> None:
    try:
        app = PseudoIdle()
    except tk.TclError as e:
        print("Tkinter failed to start (install python3-tk or use framework Python on macOS):", e, file=sys.stderr)
        sys.exit(1)
    app.mainloop()


if __name__ == "__main__":
    main()
