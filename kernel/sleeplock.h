// kernel/sleeplock.h

#ifndef SLEEPLOCK_H
#define SLEEPLOCK_H

#include "types.h"
#include "riscv.h"
#include "spinlock.h"

struct sleeplock {
  uint locked;           // Is the lock held?
  struct spinlock lk;    // Spinlock protecting this sleep lock

  // For debugging:
  char *name;            // Name of lock.
  int pid;               // Process holding lock
};

#endif // SLEEPLOCK_H
