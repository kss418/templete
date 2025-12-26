# AGENTS.md (PS C++ templates repo)

## Goal
Maintain a clean, reliable competitive-programming template repository.
Prefer small, surgical diffs; avoid rewriting unrelated files.

## Repo sync (web tasks)
- At the start of a task, run:
  - git fetch --all --prune
- If working on the default branch:
  - ensure `git status --porcelain` is empty
  - git pull --ff-only
- Never use `git reset --hard` unless explicitly instructed.

- ## Style / conventions
- C++17/20/23 as used in the repo (follow existing).
- Keep headers minimal; prefer `#include <bits/stdc++.h>` if the repo standard uses it.
- Use consistent type aliases/macros as already present.
- No extra I/O prompts; code should be contest-ready.
