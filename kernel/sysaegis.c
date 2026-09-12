#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "aegis.h"

uint64
sys_trace(void)
{
  uint64 mask;
  struct proc *p = myproc();

  argaddr(0, &mask);
  acquire(&p->lock);
  p->trace_mask = mask;
  release(&p->lock);
  return 0;
}

uint64
sys_pstat(void)
{
  int pid;
  uint64 dst;
  struct aegis_pstat st;
  struct proc *p = myproc();

  argint(0, &pid);
  argaddr(1, &dst);
  if (proc_get_pstat(pid, &st) < 0)
    return -1;
  if (copyout(p->pagetable, p->sz, dst, (char *)&st, sizeof(st)) < 0)
    return -1;
  return 0;
}

uint64
sys_ktrace_read(void)
{
  uint64 after;
  uint64 dst;
  struct aegis_event ev;
  struct proc *p = myproc();

  argaddr(0, &after);
  argaddr(1, &dst);
  if (aegis_trace_next(after, &ev) == 0)
    return 0;
  if (copyout(p->pagetable, p->sz, dst, (char *)&ev, sizeof(ev)) < 0)
    return -1;
  return 1;
}
