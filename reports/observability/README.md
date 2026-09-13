# Observability Branch Report

## Revision

Working tree based on `4b31f99`.

## Host

The tests used the same host, compiler, QEMU, and Make environment recorded in
`../baseline/raw/environment.txt`.

## Tests

| Test | CPUS | Result | Raw output |
|---|---:|---|---|
| `obstest` | 1 | PASS | `raw/cpu1.txt` |
| `obstest` | 3 | PASS | `raw/cpu3.txt` |
| Process statistics | 1 | PASS | `raw/cpu1.txt` |
| Lazy page-fault count | 1 | PASS | `raw/cpu1.txt` |
| Trace inheritance | 1, 3 | PASS | `raw/cpu1.txt`, `raw/cpu3.txt` |
| Ring overflow | 1, 3 | PASS | `raw/cpu1.txt`, `raw/cpu3.txt` |
| `usertests -q` | 1 | PASS | interactive QEMU run |

## Notes

`obstest` covers process statistics, lazy page faults, trace-mask inheritance,
ring overflow, and sequence ordering. It passed with both one and three CPUs.
The full one-CPU `usertests -q` run also printed `ALL TESTS PASSED`.

Runtime and ready time use kernel ticks, so very short commands may show zero.
The ring keeps the newest 256 events and overwrites older entries. Trace reads
are nonblocking, so a live monitor would need to poll for new events.

The trace lock is released before `copyout()`, which keeps user-memory faults
outside the locked section.
