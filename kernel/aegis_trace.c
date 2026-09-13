#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "aegis.h"

static struct {
  struct spinlock lock;
  unsigned long next_seq;
  struct aegis_event events[AEGIS_TRACE_CAP];
} tracebuf;

void
aegis_trace_init(void)
{
  initlock(&tracebuf.lock, "aegis_trace");
  tracebuf.next_seq = 1;
}

void
aegis_trace_emit(int type, int code, long arg0, long result)
{
  struct proc *p = myproc();
  struct aegis_event ev;

  ev.tick = __atomic_load_n(&ticks, __ATOMIC_RELAXED);
  ev.type = type;
  ev.pid = p ? p->pid : 0;
  push_off();
  ev.cpu = cpuid();
  pop_off();
  ev.code = code;
  ev.arg0 = arg0;
  ev.result = result;

  acquire(&tracebuf.lock);
  ev.seq = tracebuf.next_seq++;
  tracebuf.events[ev.seq % AEGIS_TRACE_CAP] = ev;
  release(&tracebuf.lock);
}

int
aegis_trace_next(unsigned long after_seq, struct aegis_event *out)
{
  unsigned long oldest;
  unsigned long wanted;

  acquire(&tracebuf.lock);
  oldest = tracebuf.next_seq > AEGIS_TRACE_CAP
             ? tracebuf.next_seq - AEGIS_TRACE_CAP
             : 1;
  wanted = after_seq + 1;
  if (wanted < oldest)
    wanted = oldest;
  if (wanted >= tracebuf.next_seq) {
    release(&tracebuf.lock);
    return 0;
  }
  *out = tracebuf.events[wanted % AEGIS_TRACE_CAP];
  release(&tracebuf.lock);
  return 1;
}
