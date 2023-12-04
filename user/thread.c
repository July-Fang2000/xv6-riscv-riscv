#include "kernel/types.h"
#include "user/user.h"
#include "user/thread.h"

#define PGSIZE 4096 

int thread_create(void *(*start_routine)(void *), void *arg) {
  void *stack = malloc(PGSIZE);
  if (stack == 0) {
    return -1; 
  }

  int thread_id = clone(stack + PGSIZE); 
  if (thread_id < 0) {
    free(stack);
    return -1; 
  }
  if (thread_id == 0) {

    start_routine(arg);
    exit(0);
  }
  return 0; 
}

void lock_init(struct lock_t *lock) { lock->locked = 0; }

void lock_acquire(struct lock_t *lock) {
  while (__sync_lock_test_and_set(&lock->locked, 1) != 0){
    __sync_synchronize();
  } 
}

void lock_release(struct lock_t *lock) {
  __sync_synchronize(); 
  __sync_lock_release(&lock->locked, 0);
}
