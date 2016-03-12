#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "thread.h"
#include "param.h"
#include "mmu.h"

typedef struct __args {
  int *throws;
  int *thread_num;
  struct lock_t *lock;
} thread_args;

void *frisbee(void *args){
  thread_args *arg = (thread_args*)args;

  int x = 0;

  for(x=0; x<*arg->throws;x++){
    lock_acquire(arg->lock);
    printf(2, "Fly, my loving morning dove! %d throws\n", getpid());
    printf(2, "Thread Number: %d\n", arg->thread_num);
    lock_release(arg->lock);
  }
  return 0;
}

  int
main(int argc, char *argv[])
{
  int pid;
  int threads = 0;
  int throws = 0;
  thread_args args;

  if(argc != 3) {
    throws = 3;
    threads = 3;
  } else {
    throws = atoi(argv[2]);
    threads = atoi(argv[1]);
  }

  printf(2, "Using %d throws with %d threads\n", throws, threads);

  struct lock_t lock;
  init_lock(&lock);

  args.throws = &throws;
  args.lock = &lock;

  int x = 0;

  for(x = 0;x < threads; x++) {
    args.thread_num = &x;
    pid = thread_create(frisbee, (void*) &args);
    printf(2, "Starting thread: %d\n", pid);
  }

  for(x = 0; x < threads; x++) {
    thread_join();
  }

  printf(1, "done\n");
  exit();
}
