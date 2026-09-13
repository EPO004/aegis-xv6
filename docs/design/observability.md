# Kernel observability notes

## Requirements and non-goals

This work adds the basic measurements that later Aegis changes will need. We
keep a few counters for each process, allow selected syscalls to be traced, and
record recent kernel events in a small ring buffer. The scheduler still picks
processes exactly as it did before.

## Event ABI

The structs shared with user programs live in `kernel/aegis.h`. An event records
its sequence number, tick, type, PID, CPU, an event-specific code, one argument,
and a result. Sequence numbers always move forward. If the ABI needs another
field during version 0.x, it should be added at the end so existing fields do
not move.

## Ring wrap semantics

The ring holds 256 events. Once it fills up, a new event replaces the oldest
one. Readers pass the last sequence number they saw. If that event has already
been overwritten, the next read starts from the oldest event still available.

## Locking

`tracebuf.lock` protects both the ring and its next sequence number. Some event
writes happen while `p->lock` is already held, so the lock order matters: code
must never grab a process lock while holding `tracebuf.lock`.

The reader copies one event into a temporary kernel struct, drops the ring lock,
and only then calls `copyout()`. Holding a spinlock across `copyout()` would be
unsafe because copying to user memory may fault.

`p->lock` protects the process counters, timestamps, and trace mask. Tick reads
use relaxed atomics. These timestamps are approximate measurements, and reading
the tick value does not need to order other memory operations. During `fork`,
the parent mask is sampled atomically while the child is still locked.

## Accounting

When the scheduler chooses a runnable process, it adds the time spent waiting to
that process's ready counter. Runtime is added when the process returns to the
scheduler. If `pstat` looks at a process that is currently running, it also
includes the unfinished running interval.

A context switch is counted when the process is dispatched. Syscalls are
counted before their handlers run. Page faults are counted only when `vmfault()`
successfully handles a lazy-allocation fault.

## Overhead

Every event briefly takes the global ring lock. Scheduler events are always
recorded, and syscalls selected by the trace mask also print a line to the
console. The counters add a few tick reads and short process-lock sections to
the scheduler, syscall, and handled-fault paths.

## Test matrix

`obstest` covers the basic counters, lazy page faults, trace-mask inheritance
through `fork`, ring overflow, and sequence ordering. It should be run once with
one CPU and once with three CPUs. The normal `usertests -q` run should follow
those focused checks.

## Limitations

- There is one global ring, so an old event disappears after 256 newer events.
- Readers poll the ring; there is no blocking read yet.
- Runtime and ready time are only as precise as the kernel tick.
- Each reader keeps its own cursor. A jump in sequence numbers is the only sign
  that it missed overwritten events.
