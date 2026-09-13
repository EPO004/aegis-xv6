#ifndef _AEGIS_H_
#define _AEGIS_H_

#define AEGIS_TRACE_CAP 256
#define AEGIS_NAME_LEN  16

enum aegis_event_type {
  AEGIS_EV_SYSCALL = 1,
  AEGIS_EV_PAGE_FAULT = 2,
  AEGIS_EV_SCHED_IN = 3,
  AEGIS_EV_SCHED_OUT = 4,
  AEGIS_EV_POLICY_DENY = 5,
};

struct aegis_event {
  unsigned long seq;
  unsigned long tick;
  int type;
  int pid;
  int cpu;
  int code;
  long arg0;
  long result;
};

struct aegis_pstat {
  int pid;
  int state;
  char name[AEGIS_NAME_LEN];
  unsigned long created_tick;
  unsigned long run_ticks;
  unsigned long ready_ticks;
  unsigned long context_switches;
  unsigned long syscall_count;
  unsigned long page_faults;
};

#endif
