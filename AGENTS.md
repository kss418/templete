# AGENTS.md (PS C++ templates repo)

## Goal
Maintain a clean, reliable competitive-programming template repository.
Prefer small, surgical diffs; avoid rewriting unrelated files.

## Snippets sync
- If you change any reusable template code, also update the corresponding editor snippets
  (e.g., VS Code user/project snippets) in the same change.
- Keep snippet triggers/prefixes and the expanded code consistent with the repository templates.
- Do not defer snippet updates; treat snippets as part of the template interface.

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
