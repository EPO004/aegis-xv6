#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/aegis.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int pid;
  struct aegis_pstat st;

  if (argc > 2) {
    fprintf(2, "usage: pstat [pid]\n");
    exit(1);
  }
  pid = argc == 2 ? atoi(argv[1]) : getpid();
  if (pstat(pid, &st) < 0) {
    fprintf(2, "pstat: process %d not found\n", pid);
    exit(1);
  }
  printf("pid,state,name,created,run,ready,switches,syscalls,faults\n");
  printf("%d,%d,%s,%lu,%lu,%lu,%lu,%lu,%lu\n", st.pid, st.state, st.name,
         st.created_tick, st.run_ticks, st.ready_ticks, st.context_switches,
         st.syscall_count, st.page_faults);
  exit(0);
}
