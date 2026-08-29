# Aegis-xv6

Aegis-xv6 is an observable, policy-driven, capability-oriented extension of
MIT xv6 for 64-bit RISC-V.

> Status: active educational systems project; not a production operating system.

## Project goals

- Kernel and process observability
- Replaceable scheduling policies
- Copy-on-write and file-backed virtual memory
- Irreversible process sandboxing
- Descriptor-oriented capability restrictions
- Measured multicore scalability improvements

## Upstream baseline

- Project: MIT PDOS xv6-riscv
- Commit: `25348327e624e91a51673b3808862e156c751212`
- Date pinned: 2026-08-20

The original MIT README is preserved in `docs/MIT_XV6_README.md`. The original
license remains in `LICENSE`.

## Quick start

```bash
sudo apt install gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu qemu-system-misc make
make -j"$(nproc)"
make CPUS=1 qemu
```

Exit QEMU with `Ctrl-a`, then `x`.

## Branches and milestones

| Branch | Purpose | Status |
|---|---|---|
| `feature/kernel-observability` | Tracing and process statistics | Planned |
| `feature/scheduler-framework` | RR, priority, and MLFQ | Planned |
| `feature/advanced-vm` | COW and memory mappings | Planned |
| `feature/process-sandbox` | Syscall and resource restrictions | Planned |
| `feature/capability-security` | Per-descriptor rights | Planned |
| `feature/multicore-scalability` | Allocator and cache contention | Planned |

## Correctness

Every integrated feature must pass its focused tests and the baseline
`usertests -q` suite. Raw experiment output and reports are stored under
`reports/`.

## Documentation

- `docs/architecture/`: integrated architecture
- `docs/design/`: per-feature design decisions and invariants
- `reports/`: test and benchmark evidence

## Author

Mohammadfarhan Bahrami — [GitHub](https://github.com/EPO004)

## License and attribution

This project is derived from MIT xv6-riscv. See `LICENSE` and
`docs/MIT_XV6_README.md` for the upstream copyright and acknowledgements.
