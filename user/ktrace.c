#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/aegis.h"
#include "user/user.h"

int
main(void)
{
  unsigned long after = 0;
  struct aegis_event ev;
  int result;

  printf("seq,tick,type,pid,cpu,code,arg0,result\n");
  while ((result = ktrace_read(after, &ev)) > 0) {
    printf("%lu,%lu,%d,%d,%d,%d,%ld,%ld\n", ev.seq, ev.tick, ev.type, ev.pid,
           ev.cpu, ev.code, ev.arg0, ev.result);
    after = ev.seq;
  }
  if (result < 0) {
    fprintf(2, "ktrace: read failed\n");
    exit(1);
  }
  exit(0);
}
