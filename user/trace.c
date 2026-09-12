#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/aegis.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  unsigned long mask;

  if (argc < 3) {
    fprintf(2, "usage: trace MASK COMMAND [ARG...]\n");
    exit(1);
  }
  mask = atoi(argv[1]);
  if (trace(mask) < 0) {
    fprintf(2, "trace: cannot set mask\n");
    exit(1);
  }
  exec(argv[2], &argv[2]);
  fprintf(2, "trace: exec %s failed\n", argv[2]);
  exit(1);
}
