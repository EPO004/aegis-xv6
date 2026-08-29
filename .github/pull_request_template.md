## Problem

Explain the limitation in baseline xv6.

## Design

Explain the new interfaces, data structures, ownership, and locking rules.

## Files changed

- `kernel/...`: reason
- `user/...`: reason

## Correctness tests

| Test | CPUS | Result |
|---|---:|---|
| `usertests -q` | 1 | PASS |
| feature test | 3 | PASS |

## Performance or overhead

Summarize measured results. Link the raw files in `reports/.../raw/`.

## Security considerations

State what is enforced, what is trusted, and known bypasses or non-goals.

## Known limitations

- Limitation 1
- Limitation 2

## Checklist

- [ ] Clean build
- [ ] `git diff --check`
- [ ] Focused tests
- [ ] `usertests -q`
- [ ] Documentation
- [ ] Raw output committed
