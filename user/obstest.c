#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/aegis.h"
#include "user/user.h"

#define GETPID_MASK (1UL << 11)

static void
fail(char *name)
{
  printf("obstest: %s: FAIL\n", name);
  exit(1);
}

int
main(void)
{
  struct aegis_pstat before, after;
  struct aegis_event ev;
  unsigned long seq, previous;
  int child, found, n, status;
  volatile int sink;
  char *memory;

  if (pstat(getpid(), &before) < 0)
    fail("basic stats");
  for (int i = 0; i < 4; i++)
    sink = getpid();
  if (pstat(getpid(), &after) < 0 || after.syscall_count <= before.syscall_count)
    fail("basic stats");
  printf("obstest: basic stats: PASS\n");

  before = after;
  memory = sbrklazy(2 * 4096);
  if (memory == SBRK_ERROR)
    fail("page faults");
  memory[0] = 1;
  memory[4096] = 2;
  if (pstat(getpid(), &after) < 0 || after.page_faults < before.page_faults + 2)
    fail("page faults");
  printf("obstest: page faults: PASS\n");

  if (trace(GETPID_MASK) < 0)
    fail("trace inheritance");
  child = fork();
  if (child < 0)
    fail("trace inheritance");
  if (child == 0) {
    sink = getpid();
    exit(sink < 0);
  }
  if (wait(&status) != child || status != 0)
    fail("trace inheritance");
  found = 0;
  seq = 0;
  while ((n = ktrace_read(seq, &ev)) > 0) {
    seq = ev.seq;
    if (ev.type == AEGIS_EV_SYSCALL && ev.pid == child && ev.code == 11)
      found = 1;
  }
  if (n < 0 || !found)
    fail("trace inheritance");
  printf("obstest: trace inheritance: PASS\n");

  for (int i = 0; i < AEGIS_TRACE_CAP * 2; i++)
    sink = getpid();
  previous = 0;
  seq = 0;
  n = 0;
  while (n < AEGIS_TRACE_CAP && ktrace_read(seq, &ev) > 0) {
    if (previous != 0 && ev.seq <= previous)
      fail("ring overflow");
    previous = ev.seq;
    seq = ev.seq;
    n++;
  }
  trace(0);
  if (n == 0 || previous == 0)
    fail("ring overflow");
  printf("obstest: ring overflow: PASS\n");
  printf("obstest: ALL PASS\n");
  exit(0);
}
