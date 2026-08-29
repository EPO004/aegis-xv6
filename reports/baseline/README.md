# Baseline Report

## Revision

`25348327e624e91a51673b3808862e156c751212`

## Host

The complete host, compiler, QEMU, and Make environment is recorded in
`raw/environment.txt`.

## Tests

| Test | CPUS | Result | Raw output |
|---|---:|---|---|
| Build and shell boot | 1 | PASS | `raw/smoke-cpu1.txt` |
| Build and shell boot | 3 | PASS | `raw/smoke-cpu3.txt` |
| `usertests -q` | 1 | PASS | `raw/qemu-cpu1.txt` |
| `usertests -q` | 3 | PASS | `raw/qemu-cpu3.txt` |

## Notes

The baseline uses the pinned MIT xv6-riscv revision.

The local smoke-test runner uses `expect` to provide reliable terminal-aware
QEMU boot detection. The build uses a bounded parallel job count on the local
host.
