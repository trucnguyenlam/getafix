/*  Generated 0.0.0.0.0.0 (-t7 -r2 -u1 -bcbmc) 2016-04-14 18:25:28  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define THREADS 7
#define ROUNDS 2

#define STOP_VOID(A) return;
#define STOP_NONVOID(A) return 0;

#define IF(T,A,B)  if ((pc[T] > A) | (A >= pc_cs)) goto B;
#define GUARD(T,B)  assume( pc_cs >= B );

#ifndef NULL
#define NULL 0
#endif

#define assume(x) __CPROVER_assume(x)
#define __VERIFIER_assume(x) __CPROVER_assume(x)

#define assert(x) assert(x)
#define __VERIFIER_assert(x) assert(x)

#define __VERIFIER_nondet_int() nondet_int()
int nondet_int();
#define __VERIFIER_nondet_uint() nondet_uint()
unsigned int nondet_uint();
#define __VERIFIER_nondet_bool() nondet_bool()
_Bool nondet_bool();
#define __VERIFIER_nondet_char() nondet_char()
char nondet_char();
#define __VERIFIER_nondet_uchar() nondet_uchar()
unsigned char nondet_uchar();

unsigned __CPROVER_bitvector[1] active_thread[THREADS + 1] = {1};
unsigned __CPROVER_bitvector[4] pc[THREADS + 1];
unsigned __CPROVER_bitvector[5] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[4] thread_lines[] = {7, 10, 10, 10, 15, 15, 15, 15};

/* pthread API */
typedef int __cs_t;
typedef int __cs_attr_t;
typedef int __cs_mutex_t;
typedef int __cs_mutexattr_t;
typedef int __cs_cond_t;
typedef int __cs_condattr_t;
typedef int __cs_spinlock_t;
typedef int __cs_key_t;
typedef int __cs_once_t;
typedef int __cs_rwlock_t;
typedef int __cs_rwlockattr_t;
typedef int __cs_barrier_t;
typedef int __cs_barrierattr_t;

/* define PTHREAD CONSTANTS */
#define PTHREAD_MUTEX_INITIALIZER -1
#define PTHREAD_COND_INITIALIZER -1
#define PTHREAD_RWLOCK_INITIALIZER 0
#define PTHREAD_BARRIER_SERIAL_THREAD 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 0
#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_DISABLE 0
#define PTHREAD_CANCELED 0
#define PTHREAD_CREATE_DETACHED 0
#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_EXPLICIT_SCHED 0
#define PTHREAD_INHERIT_SCHED 0
#define PTHREAD_MUTEX_DEFAULT 0
#define PTHREAD_MUTEX_ERRORCHECK 0
#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_RECURSIVE 0
#define PTHREAD_MUTEX_ROBUST 0
#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_ONCE_INIT 0
#define PTHREAD_PRIO_INHERIT 0
#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_PROTECT 0
#define PTHREAD_PROCESS_SHARED 0
#define PTHREAD_PROCESS_PRIVATE 0
#define PTHREAD_SCOPE_PROCESS 0
#define PTHREAD_SCOPE_SYSTEM 0

void *threadargs[THREADS + 1];

int __cs_create(__cs_t *id, void *attr, void *(*t)(void*), void *arg, int threadID) { if (threadID > THREADS) return 0;  *id = threadID; active_thread[threadID] = 1; threadargs[threadID] = arg; return 0; }
int __cs_join(__cs_t id, void **value_ptr) { assume(pc[id] == thread_lines[id]); return 0; }

void __cs_exit(void *value_ptr) { return; }   // only for parsing
__cs_t __cs_self() {  return thread_index;  }
int __cs_cancel(__cs_t id) {  return 0;  }

int __cs_mutex_init (__cs_mutex_t *m, int val) { *m = -1; return 0; }
int __cs_mutex_destroy(__cs_mutex_t *m) { return 0; }
int __cs_mutex_lock(__cs_mutex_t *m) { assume(*m == -1); *m = thread_index; return 0; }
int __cs_mutex_unlock(__cs_mutex_t *m) { assume(*m == thread_index); *m = -1; return 0; }

int __cs_cond_init(__cs_cond_t *cond, void *attr) { *cond = -1; return 0; }
int __cs_cond_destroy(__cs_cond_t *cond) { return 0; }
/* Already handle in lock optimizer module */
// int __cs_cond_wait(__cs_cond_t *cond, __cs_mutex_t *mutex) { __cs_mutex_unlock(mutex); assume(*cond != -1); __cs_mutex_lock(mutex); return 0;}
// int __cs_cond_timedwait(__cs_cond_t *cond, __cs_mutex_t *mutex, struct timespec *abstime) { if(__VERIFIER_nondet_bool()) {__cs_mutex_unlock(mutex); assume(*cond != -1); __cs_mutex_lock(mutex); return 0;} else {return ETIMEDOUT;}}
int __cs_cond_signal(__cs_cond_t *cond) { *cond = 1; return 0; }
int __cs_cond_broadcast(__cs_cond_t *cond) { *cond = 2; return 0; }

int __cs_spin_init(__cs_spinlock_t *l, int val) {*l = -1; return 0;}
int __cs_spin_destroy(__cs_spinlock_t *l) {return 0;}
int __cs_spin_lock(__cs_spinlock_t *l) { assume(*l == -1); *l = thread_index; return 0; }
int __cs_spin_unlock(__cs_spinlock_t *l) { assume(*l == thread_index); *l = -1; return 0; }

// Dirty fix for those macro in this situation val = __cs_..MACRO
#define __cs_mutex_lock_MACRO(m) 0; assume(m == -1); m = thread_index;
#define __cs_mutex_unlock_MACRO(m) 0; assume(m == thread_index); m = -1;
#define __cs_cond_signal_MACRO(cond) 0; cond = 1;
#define __cs_cond_broadcast_MACRO(cond) 0; cond = 2;
#define __cs_spin_unlock_MACRO(l) 0; assume(l == thread_index); l = -1;
#define __cs_spin_lock_MACRO(l) 0; assume(l == -1); l = thread_index;

// For schedule yield function
int __cs_sched_yield(unsigned __CPROVER_bitvector[5] val) {pc_cs = val + 1; return 0;}    // Version 1
//int __cs_sched_yield(unsigned __CPROVER_bitvector[5] val) {return 0;}     // Version 2


void * __cs_safe_malloc(size_t size) { void * ptr = malloc(size); assume(ptr); return ptr; }
void * __cs_unsafe_malloc(size_t size) { if (__VERIFIER_nondet_int()) return 0; return malloc(size); }
#define __cs_safe_malloc_LOCAL __cs_safe_malloc
#define __cs_unsafe_malloc malloc

void __cs_init_scalar(void *var, size_t size) {
  if (size == sizeof(int))
    *(int *)var = __VERIFIER_nondet_int();
  else {
    char * ptr = (char *)var;
    // size_t j;
    // for (j=0; j<size; j++)
    //     ptr[j] = __cs_nondet_char();
  }
}
size_t nondet_size_t();

unsigned __CPROVER_bitvector[4] guess_pc() {unsigned __CPROVER_bitvector[4] i; return i;}

/* here */


int __CS_atomic_CAS(void **__csLOCALPARAM___CS_atomic_CAS_address, void *__csLOCALPARAM___CS_atomic_CAS_old_value, void *__csLOCALPARAM___CS_atomic_CAS_new_value)
{
  if ((*__csLOCALPARAM___CS_atomic_CAS_address) == __csLOCALPARAM___CS_atomic_CAS_old_value)
  {
    *__csLOCALPARAM___CS_atomic_CAS_address = __csLOCALPARAM___CS_atomic_CAS_new_value;
    return 1;
  }

  return 0;
}

typedef struct Node Node;
typedef
struct Node
{
  Node *next;
  int val;
} Node;
typedef
struct Stack
{
  Node *top;
} Stack;
Stack gstack;
Node nodepointers[2];
int allocated[2];
Node *__CS_atomic_malloc_Node()
{
  static int __csLOCAL___CS_atomic_malloc_Node_i;
  __csLOCAL___CS_atomic_malloc_Node_i = nondet_int();
  __VERIFIER_assume((0 <= __csLOCAL___CS_atomic_malloc_Node_i) && (__csLOCAL___CS_atomic_malloc_Node_i < 2));
  __VERIFIER_assume(!allocated[__csLOCAL___CS_atomic_malloc_Node_i]);
  allocated[__csLOCAL___CS_atomic_malloc_Node_i] = 1;
  return &nodepointers[__csLOCAL___CS_atomic_malloc_Node_i];
}

int __cs_function_malloc_Node_inlined = 1;
int __cs_function_free_Node_inlined = 1;
int __cs_function_push_aux_inlined = 1;
int __cs_function_pop_aux_inlined = 1;
int __cs_function_Push_inlined = 1;
int __cs_function_Pop_inlined = 1;
int __cs_function_Init_inlined = 1;
int PushOpen[2];
int PushDone[2];
int PopOpen;
int PopDone[3];
int __cs_function_checkInvariant_inlined = 1;
void __CS_atomic_atomicIncr_Push(int __csLOCALPARAM___CS_atomic_atomicIncr_Push_localPush1)
{
  PushOpen[__csLOCALPARAM___CS_atomic_atomicIncr_Push_localPush1]++;
}

void __CS_atomic_atomicDecrIncr_Push(int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1)
{
  PushOpen[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1]--;
  PushDone[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1]++;
  {
    assert((PopDone[0] <= (PushDone[0] + PushOpen[0])) && (PopDone[1] <= (PushDone[1] + PushOpen[1])));
__exit__checkInvariant_1:
    ;

  }
  0;
}

void __CS_atomic_atomicIncr_Pop()
{
  PopOpen++;
}

void __CS_atomic_atomicDecrIncr_Pop(int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_localPop_ret)
{
  PopOpen--;
  PopDone[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_localPop_ret]++;
  {
    assert((PopDone[0] <= (PushDone[0] + PushOpen[0])) && (PopDone[1] <= (PushDone[1] + PushOpen[1])));
__exit__checkInvariant_2:
    ;

  }
  0;
}

void *instrPush0_0(void *__csLOCALPARAM_instrPush0_unused)
{
  IF(1, 0, tinstrPush0_0_1)                                __CS_atomic_atomicIncr_Push(1);
  {
    static int __csLOCALPARAM_Push_i;
    __csLOCALPARAM_Push_i = 1;
    static Node *__csLOCAL_Push_n;
    static Node *__retval__malloc_Node_1;
    {
tinstrPush0_0_1: IF(1, 1, tinstrPush0_0_2)               __retval__malloc_Node_1 = __CS_atomic_malloc_Node();
      goto __exit__malloc_Node_1;
__exit__malloc_Node_1: GUARD(1, 2)
      ;
    };
tinstrPush0_0_2: IF(1, 2, tinstrPush0_0_3)               __csLOCAL_Push_n = __retval__malloc_Node_1;
tinstrPush0_0_3: IF(1, 3, tinstrPush0_0_4)               (*__csLOCAL_Push_n).val = __csLOCALPARAM_Push_i;
    {
      static Node *__csLOCALPARAM_push_aux_n;
tinstrPush0_0_4: IF(1, 4, tinstrPush0_0_5)               __csLOCALPARAM_push_aux_n = __csLOCAL_Push_n;
      static Node *__csLOCAL_push_aux_oldtop;
tinstrPush0_0_5: IF(1, 5, tinstrPush0_0_6)               __csLOCAL_push_aux_oldtop = gstack.top;
tinstrPush0_0_6: IF(1, 6, tinstrPush0_0_7)               (*__csLOCALPARAM_push_aux_n).next = __csLOCAL_push_aux_oldtop;
      ;
      static _Bool __csLOCAL_push_aux___if_cond_0;
tinstrPush0_0_7: IF(1, 7, tinstrPush0_0_8)               __csLOCAL_push_aux___if_cond_0 = __CS_atomic_CAS(&gstack.top, __csLOCAL_push_aux_oldtop, __csLOCALPARAM_push_aux_n);
      if (__csLOCAL_push_aux___if_cond_0)
      {
        goto __exit__push_aux_1;
      }
      ;
tinstrPush0_0_8: IF(1, 8, tinstrPush0_0_9)               __VERIFIER_assume(0);
__exit__push_aux_1: GUARD(1, 9)
      ;
    };
    0;
__exit__Push_1: GUARD(1, 9)
    ;
  };
  0;
tinstrPush0_0_9: IF(1, 9, tinstrPush0_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
  goto __exit_instrPush0;
__exit_instrPush0: GUARD(1, 10)
  ;
tinstrPush0_0_10:                                      STOP_NONVOID(10);
}



void *instrPush1_0(void *__csLOCALPARAM_instrPush1_unused)
{
  IF(2, 0, tinstrPush1_0_1)                                __CS_atomic_atomicIncr_Push(1);
  {
    static int __csLOCALPARAM_Push_i;
    __csLOCALPARAM_Push_i = 1;
    static Node *__csLOCAL_Push_n;
    static Node *__retval__malloc_Node_2;
    {
tinstrPush1_0_1: IF(2, 1, tinstrPush1_0_2)               __retval__malloc_Node_2 = __CS_atomic_malloc_Node();
      goto __exit__malloc_Node_2;
__exit__malloc_Node_2: GUARD(2, 2)
      ;
    };
tinstrPush1_0_2: IF(2, 2, tinstrPush1_0_3)               __csLOCAL_Push_n = __retval__malloc_Node_2;
tinstrPush1_0_3: IF(2, 3, tinstrPush1_0_4)               (*__csLOCAL_Push_n).val = __csLOCALPARAM_Push_i;
    {
      static Node *__csLOCALPARAM_push_aux_n;
tinstrPush1_0_4: IF(2, 4, tinstrPush1_0_5)               __csLOCALPARAM_push_aux_n = __csLOCAL_Push_n;
      static Node *__csLOCAL_push_aux_oldtop;
tinstrPush1_0_5: IF(2, 5, tinstrPush1_0_6)               __csLOCAL_push_aux_oldtop = gstack.top;
tinstrPush1_0_6: IF(2, 6, tinstrPush1_0_7)               (*__csLOCALPARAM_push_aux_n).next = __csLOCAL_push_aux_oldtop;
      ;
      static _Bool __csLOCAL_push_aux___if_cond_0;
tinstrPush1_0_7: IF(2, 7, tinstrPush1_0_8)               __csLOCAL_push_aux___if_cond_0 = __CS_atomic_CAS(&gstack.top, __csLOCAL_push_aux_oldtop, __csLOCALPARAM_push_aux_n);
      if (__csLOCAL_push_aux___if_cond_0)
      {
        goto __exit__push_aux_2;
      }
      ;
tinstrPush1_0_8: IF(2, 8, tinstrPush1_0_9)               __VERIFIER_assume(0);
__exit__push_aux_2: GUARD(2, 9)
      ;
    };
    0;
__exit__Push_2: GUARD(2, 9)
    ;
  };
  0;
tinstrPush1_0_9: IF(2, 9, tinstrPush1_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
  goto __exit_instrPush1;
__exit_instrPush1: GUARD(2, 10)
  ;
tinstrPush1_0_10:                                      STOP_NONVOID(10);
}



void *instrPush2_0(void *__csLOCALPARAM_instrPush2_unused)
{
  IF(3, 0, tinstrPush2_0_1)                                __CS_atomic_atomicIncr_Push(1);
  {
    static int __csLOCALPARAM_Push_i;
    __csLOCALPARAM_Push_i = 1;
    static Node *__csLOCAL_Push_n;
    static Node *__retval__malloc_Node_3;
    {
tinstrPush2_0_1: IF(3, 1, tinstrPush2_0_2)               __retval__malloc_Node_3 = __CS_atomic_malloc_Node();
      goto __exit__malloc_Node_3;
__exit__malloc_Node_3: GUARD(3, 2)
      ;
    };
tinstrPush2_0_2: IF(3, 2, tinstrPush2_0_3)               __csLOCAL_Push_n = __retval__malloc_Node_3;
tinstrPush2_0_3: IF(3, 3, tinstrPush2_0_4)               (*__csLOCAL_Push_n).val = __csLOCALPARAM_Push_i;
    {
      static Node *__csLOCALPARAM_push_aux_n;
tinstrPush2_0_4: IF(3, 4, tinstrPush2_0_5)               __csLOCALPARAM_push_aux_n = __csLOCAL_Push_n;
      static Node *__csLOCAL_push_aux_oldtop;
tinstrPush2_0_5: IF(3, 5, tinstrPush2_0_6)               __csLOCAL_push_aux_oldtop = gstack.top;
tinstrPush2_0_6: IF(3, 6, tinstrPush2_0_7)               (*__csLOCALPARAM_push_aux_n).next = __csLOCAL_push_aux_oldtop;
      ;
      static _Bool __csLOCAL_push_aux___if_cond_0;
tinstrPush2_0_7: IF(3, 7, tinstrPush2_0_8)               __csLOCAL_push_aux___if_cond_0 = __CS_atomic_CAS(&gstack.top, __csLOCAL_push_aux_oldtop, __csLOCALPARAM_push_aux_n);
      if (__csLOCAL_push_aux___if_cond_0)
      {
        goto __exit__push_aux_3;
      }
      ;
tinstrPush2_0_8: IF(3, 8, tinstrPush2_0_9)               __VERIFIER_assume(0);
__exit__push_aux_3: GUARD(3, 9)
      ;
    };
    0;
__exit__Push_3: GUARD(3, 9)
    ;
  };
  0;
tinstrPush2_0_9: IF(3, 9, tinstrPush2_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
  goto __exit_instrPush2;
__exit_instrPush2: GUARD(3, 10)
  ;
tinstrPush2_0_10:                                      STOP_NONVOID(10);
}



void *instrPop3_0(void *__csLOCALPARAM_instrPop3_unused)
{
  IF(4, 0, tinstrPop3_0_1)                                 __CS_atomic_atomicIncr_Pop();
  static int __csLOCAL_instrPop3_localPop_ret;
  static int __retval__Pop_1;
  {
    static Node *__csLOCAL_Pop_n;
    static Node *__retval__pop_aux_1;
    {
      static Node *__csLOCAL_pop_aux_oldtop;
      static Node *__csLOCAL_pop_aux_next;
tinstrPop3_0_1: IF(4, 1, tinstrPop3_0_2)                 __csLOCAL_pop_aux_oldtop = gstack.top;
tinstrPop3_0_2: IF(4, 2, tinstrPop3_0_3)                 if (__csLOCAL_pop_aux_oldtop == 0)
      {
tinstrPop3_0_3: IF(4, 3, tinstrPop3_0_4)                 __retval__pop_aux_1 = 0;
        goto __exit__pop_aux_1;
      }
      GUARD(4, 4);
tinstrPop3_0_4: IF(4, 4, tinstrPop3_0_5)                 __csLOCAL_pop_aux_next = (*__csLOCAL_pop_aux_oldtop).next;
      ;
      static _Bool __csLOCAL_pop_aux___if_cond_1;
tinstrPop3_0_5: IF(4, 5, tinstrPop3_0_6)                 __csLOCAL_pop_aux___if_cond_1 = __CS_atomic_CAS(&gstack.top, __csLOCAL_pop_aux_oldtop, __csLOCAL_pop_aux_next);
      if (__csLOCAL_pop_aux___if_cond_1)
      {
tinstrPop3_0_6: IF(4, 6, tinstrPop3_0_7)                 __retval__pop_aux_1 = __csLOCAL_pop_aux_oldtop;
        goto __exit__pop_aux_1;
      }
      GUARD(4, 7);
tinstrPop3_0_7: IF(4, 7, tinstrPop3_0_8)                 __VERIFIER_assume(0);
__exit__pop_aux_1: GUARD(4, 8)
      ;
    };
tinstrPop3_0_8: IF(4, 8, tinstrPop3_0_9)                 __csLOCAL_Pop_n = __retval__pop_aux_1;
tinstrPop3_0_9: IF(4, 9, tinstrPop3_0_10)                if (__csLOCAL_Pop_n == 0)
    {
      __retval__Pop_1 = 2;
      goto __exit__Pop_1;
    }
    else
    {
      static int __csLOCAL_Pop_v;
tinstrPop3_0_10: IF(4, 10, tinstrPop3_0_11)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_n).val;
      {
        static Node *__csLOCALPARAM_free_Node_n;
tinstrPop3_0_11: IF(4, 11, tinstrPop3_0_12)              __csLOCALPARAM_free_Node_n = __csLOCAL_Pop_n;
        static int __csLOCAL_free_Node_i;
        __csLOCAL_free_Node_i = nondet_int();
tinstrPop3_0_12: IF(4, 12, tinstrPop3_0_13)              __VERIFIER_assume((&nodepointers[__csLOCAL_free_Node_i]) == __csLOCALPARAM_free_Node_n);
tinstrPop3_0_13: IF(4, 13, tinstrPop3_0_14)              allocated[__csLOCAL_free_Node_i] = 0;
__exit__free_Node_1: GUARD(4, 14)
        ;
      };
      0;
      __retval__Pop_1 = __csLOCAL_Pop_v;
      goto __exit__Pop_1;
    }
    GUARD(4, 14);
__exit__Pop_1: GUARD(4, 14)
    ;
  };
  __csLOCAL_instrPop3_localPop_ret = __retval__Pop_1;
tinstrPop3_0_14: IF(4, 14, tinstrPop3_0_15)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop3_localPop_ret);
  goto __exit_instrPop3;
__exit_instrPop3: GUARD(4, 15)
  ;
tinstrPop3_0_15:                                       STOP_NONVOID(15);
}



void *instrPop4_0(void *__csLOCALPARAM_instrPop4_unused)
{
  IF(5, 0, tinstrPop4_0_1)                                 __CS_atomic_atomicIncr_Pop();
  static int __csLOCAL_instrPop4_localPop_ret;
  static int __retval__Pop_2;
  {
    static Node *__csLOCAL_Pop_n;
    static Node *__retval__pop_aux_2;
    {
      static Node *__csLOCAL_pop_aux_oldtop;
      static Node *__csLOCAL_pop_aux_next;
tinstrPop4_0_1: IF(5, 1, tinstrPop4_0_2)                 __csLOCAL_pop_aux_oldtop = gstack.top;
tinstrPop4_0_2: IF(5, 2, tinstrPop4_0_3)                 if (__csLOCAL_pop_aux_oldtop == 0)
      {
tinstrPop4_0_3: IF(5, 3, tinstrPop4_0_4)                 __retval__pop_aux_2 = 0;
        goto __exit__pop_aux_2;
      }
      GUARD(5, 4);
tinstrPop4_0_4: IF(5, 4, tinstrPop4_0_5)                 __csLOCAL_pop_aux_next = (*__csLOCAL_pop_aux_oldtop).next;
      ;
      static _Bool __csLOCAL_pop_aux___if_cond_1;
tinstrPop4_0_5: IF(5, 5, tinstrPop4_0_6)                 __csLOCAL_pop_aux___if_cond_1 = __CS_atomic_CAS(&gstack.top, __csLOCAL_pop_aux_oldtop, __csLOCAL_pop_aux_next);
      if (__csLOCAL_pop_aux___if_cond_1)
      {
tinstrPop4_0_6: IF(5, 6, tinstrPop4_0_7)                 __retval__pop_aux_2 = __csLOCAL_pop_aux_oldtop;
        goto __exit__pop_aux_2;
      }
      GUARD(5, 7);
tinstrPop4_0_7: IF(5, 7, tinstrPop4_0_8)                 __VERIFIER_assume(0);
__exit__pop_aux_2: GUARD(5, 8)
      ;
    };
tinstrPop4_0_8: IF(5, 8, tinstrPop4_0_9)                 __csLOCAL_Pop_n = __retval__pop_aux_2;
tinstrPop4_0_9: IF(5, 9, tinstrPop4_0_10)                if (__csLOCAL_Pop_n == 0)
    {
      __retval__Pop_2 = 2;
      goto __exit__Pop_2;
    }
    else
    {
      static int __csLOCAL_Pop_v;
tinstrPop4_0_10: IF(5, 10, tinstrPop4_0_11)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_n).val;
      {
        static Node *__csLOCALPARAM_free_Node_n;
tinstrPop4_0_11: IF(5, 11, tinstrPop4_0_12)              __csLOCALPARAM_free_Node_n = __csLOCAL_Pop_n;
        static int __csLOCAL_free_Node_i;
        __csLOCAL_free_Node_i = nondet_int();
tinstrPop4_0_12: IF(5, 12, tinstrPop4_0_13)              __VERIFIER_assume((&nodepointers[__csLOCAL_free_Node_i]) == __csLOCALPARAM_free_Node_n);
tinstrPop4_0_13: IF(5, 13, tinstrPop4_0_14)              allocated[__csLOCAL_free_Node_i] = 0;
__exit__free_Node_2: GUARD(5, 14)
        ;
      };
      0;
      __retval__Pop_2 = __csLOCAL_Pop_v;
      goto __exit__Pop_2;
    }
    GUARD(5, 14);
__exit__Pop_2: GUARD(5, 14)
    ;
  };
  __csLOCAL_instrPop4_localPop_ret = __retval__Pop_2;
tinstrPop4_0_14: IF(5, 14, tinstrPop4_0_15)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop4_localPop_ret);
  goto __exit_instrPop4;
__exit_instrPop4: GUARD(5, 15)
  ;
tinstrPop4_0_15:                                       STOP_NONVOID(15);
}



void *instrPop5_0(void *__csLOCALPARAM_instrPop5_unused)
{
  IF(6, 0, tinstrPop5_0_1)                                 __CS_atomic_atomicIncr_Pop();
  static int __csLOCAL_instrPop5_localPop_ret;
  static int __retval__Pop_3;
  {
    static Node *__csLOCAL_Pop_n;
    static Node *__retval__pop_aux_3;
    {
      static Node *__csLOCAL_pop_aux_oldtop;
      static Node *__csLOCAL_pop_aux_next;
tinstrPop5_0_1: IF(6, 1, tinstrPop5_0_2)                 __csLOCAL_pop_aux_oldtop = gstack.top;
tinstrPop5_0_2: IF(6, 2, tinstrPop5_0_3)                 if (__csLOCAL_pop_aux_oldtop == 0)
      {
tinstrPop5_0_3: IF(6, 3, tinstrPop5_0_4)                 __retval__pop_aux_3 = 0;
        goto __exit__pop_aux_3;
      }
      GUARD(6, 4);
tinstrPop5_0_4: IF(6, 4, tinstrPop5_0_5)                 __csLOCAL_pop_aux_next = (*__csLOCAL_pop_aux_oldtop).next;
      ;
      static _Bool __csLOCAL_pop_aux___if_cond_1;
tinstrPop5_0_5: IF(6, 5, tinstrPop5_0_6)                 __csLOCAL_pop_aux___if_cond_1 = __CS_atomic_CAS(&gstack.top, __csLOCAL_pop_aux_oldtop, __csLOCAL_pop_aux_next);
      if (__csLOCAL_pop_aux___if_cond_1)
      {
tinstrPop5_0_6: IF(6, 6, tinstrPop5_0_7)                 __retval__pop_aux_3 = __csLOCAL_pop_aux_oldtop;
        goto __exit__pop_aux_3;
      }
      GUARD(6, 7);
tinstrPop5_0_7: IF(6, 7, tinstrPop5_0_8)                 __VERIFIER_assume(0);
__exit__pop_aux_3: GUARD(6, 8)
      ;
    };
tinstrPop5_0_8: IF(6, 8, tinstrPop5_0_9)                 __csLOCAL_Pop_n = __retval__pop_aux_3;
tinstrPop5_0_9: IF(6, 9, tinstrPop5_0_10)                if (__csLOCAL_Pop_n == 0)
    {
      __retval__Pop_3 = 2;
      goto __exit__Pop_3;
    }
    else
    {
      static int __csLOCAL_Pop_v;
tinstrPop5_0_10: IF(6, 10, tinstrPop5_0_11)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_n).val;
      {
        static Node *__csLOCALPARAM_free_Node_n;
tinstrPop5_0_11: IF(6, 11, tinstrPop5_0_12)              __csLOCALPARAM_free_Node_n = __csLOCAL_Pop_n;
        static int __csLOCAL_free_Node_i;
        __csLOCAL_free_Node_i = nondet_int();
tinstrPop5_0_12: IF(6, 12, tinstrPop5_0_13)              __VERIFIER_assume((&nodepointers[__csLOCAL_free_Node_i]) == __csLOCALPARAM_free_Node_n);
tinstrPop5_0_13: IF(6, 13, tinstrPop5_0_14)              allocated[__csLOCAL_free_Node_i] = 0;
__exit__free_Node_3: GUARD(6, 14)
        ;
      };
      0;
      __retval__Pop_3 = __csLOCAL_Pop_v;
      goto __exit__Pop_3;
    }
    GUARD(6, 14);
__exit__Pop_3: GUARD(6, 14)
    ;
  };
  __csLOCAL_instrPop5_localPop_ret = __retval__Pop_3;
tinstrPop5_0_14: IF(6, 14, tinstrPop5_0_15)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop5_localPop_ret);
  goto __exit_instrPop5;
__exit_instrPop5: GUARD(6, 15)
  ;
tinstrPop5_0_15:                                       STOP_NONVOID(15);
}



void *instrPop6_0(void *__csLOCALPARAM_instrPop6_unused)
{
  IF(7, 0, tinstrPop6_0_1)                                 __CS_atomic_atomicIncr_Pop();
  static int __csLOCAL_instrPop6_localPop_ret;
  static int __retval__Pop_4;
  {
    static Node *__csLOCAL_Pop_n;
    static Node *__retval__pop_aux_4;
    {
      static Node *__csLOCAL_pop_aux_oldtop;
      static Node *__csLOCAL_pop_aux_next;
tinstrPop6_0_1: IF(7, 1, tinstrPop6_0_2)                 __csLOCAL_pop_aux_oldtop = gstack.top;
tinstrPop6_0_2: IF(7, 2, tinstrPop6_0_3)                 if (__csLOCAL_pop_aux_oldtop == 0)
      {
tinstrPop6_0_3: IF(7, 3, tinstrPop6_0_4)                 __retval__pop_aux_4 = 0;
        goto __exit__pop_aux_4;
      }
      GUARD(7, 4);
tinstrPop6_0_4: IF(7, 4, tinstrPop6_0_5)                 __csLOCAL_pop_aux_next = (*__csLOCAL_pop_aux_oldtop).next;
      ;
      static _Bool __csLOCAL_pop_aux___if_cond_1;
tinstrPop6_0_5: IF(7, 5, tinstrPop6_0_6)                 __csLOCAL_pop_aux___if_cond_1 = __CS_atomic_CAS(&gstack.top, __csLOCAL_pop_aux_oldtop, __csLOCAL_pop_aux_next);
      if (__csLOCAL_pop_aux___if_cond_1)
      {
tinstrPop6_0_6: IF(7, 6, tinstrPop6_0_7)                 __retval__pop_aux_4 = __csLOCAL_pop_aux_oldtop;
        goto __exit__pop_aux_4;
      }
      GUARD(7, 7);
tinstrPop6_0_7: IF(7, 7, tinstrPop6_0_8)                 __VERIFIER_assume(0);
__exit__pop_aux_4: GUARD(7, 8)
      ;
    };
tinstrPop6_0_8: IF(7, 8, tinstrPop6_0_9)                 __csLOCAL_Pop_n = __retval__pop_aux_4;
tinstrPop6_0_9: IF(7, 9, tinstrPop6_0_10)                if (__csLOCAL_Pop_n == 0)
    {
      __retval__Pop_4 = 2;
      goto __exit__Pop_4;
    }
    else
    {
      static int __csLOCAL_Pop_v;
tinstrPop6_0_10: IF(7, 10, tinstrPop6_0_11)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_n).val;
      {
        static Node *__csLOCALPARAM_free_Node_n;
tinstrPop6_0_11: IF(7, 11, tinstrPop6_0_12)              __csLOCALPARAM_free_Node_n = __csLOCAL_Pop_n;
        static int __csLOCAL_free_Node_i;
        __csLOCAL_free_Node_i = nondet_int();
tinstrPop6_0_12: IF(7, 12, tinstrPop6_0_13)              __VERIFIER_assume((&nodepointers[__csLOCAL_free_Node_i]) == __csLOCALPARAM_free_Node_n);
tinstrPop6_0_13: IF(7, 13, tinstrPop6_0_14)              allocated[__csLOCAL_free_Node_i] = 0;
__exit__free_Node_4: GUARD(7, 14)
        ;
      };
      0;
      __retval__Pop_4 = __csLOCAL_Pop_v;
      goto __exit__Pop_4;
    }
    GUARD(7, 14);
__exit__Pop_4: GUARD(7, 14)
    ;
  };
  __csLOCAL_instrPop6_localPop_ret = __retval__Pop_4;
tinstrPop6_0_14: IF(7, 14, tinstrPop6_0_15)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop6_localPop_ret);
  goto __exit_instrPop6;
__exit_instrPop6: GUARD(7, 15)
  ;
tinstrPop6_0_15:                                       STOP_NONVOID(15);
}



int main_thread(void)
{
  IF(0, 0, tmain_1)                                         {
__exit__Init_1: GUARD(0, 1)
    ;
  };
  0;
  static __cs_t __csLOCAL_main_tid1;
  __cs_init_scalar(&__csLOCAL_main_tid1, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid2;
  __cs_init_scalar(&__csLOCAL_main_tid2, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid3;
  __cs_init_scalar(&__csLOCAL_main_tid3, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid4;
  __cs_init_scalar(&__csLOCAL_main_tid4, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid5;
  __cs_init_scalar(&__csLOCAL_main_tid5, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid6;
  __cs_init_scalar(&__csLOCAL_main_tid6, sizeof(__cs_t));
  static __cs_t __csLOCAL_main_tid7;
  __cs_init_scalar(&__csLOCAL_main_tid7, sizeof(__cs_t));
  __cs_create(&__csLOCAL_main_tid1, 0, &instrPush0_0, 0, 1);
tmain_1: IF(0, 1, tmain_2)                               __cs_create(&__csLOCAL_main_tid2, 0, &instrPush1_0, 0, 2);
tmain_2: IF(0, 2, tmain_3)                               __cs_create(&__csLOCAL_main_tid3, 0, &instrPush2_0, 0, 3);
tmain_3: IF(0, 3, tmain_4)                               __cs_create(&__csLOCAL_main_tid4, 0, &instrPop3_0, 0, 4);
tmain_4: IF(0, 4, tmain_5)                               __cs_create(&__csLOCAL_main_tid5, 0, &instrPop4_0, 0, 5);
tmain_5: IF(0, 5, tmain_6)                               __cs_create(&__csLOCAL_main_tid6, 0, &instrPop5_0, 0, 6);
tmain_6: IF(0, 6, tmain_7)                               __cs_create(&__csLOCAL_main_tid7, 0, &instrPop6_0, 0, 7);
__exit_main: GUARD(0, 7)
  ;
tmain_7:                                               STOP_NONVOID(7);
}



int main(void) {
  // round 0
  thread_index = 0;
pc_cs = 7;
//  assume((pc_cs > 0) && (pc_cs <= 7));
  main_thread();
  pc[0] = pc_cs;

  thread_index = 1;
  if (active_thread[1] == 1) { /** instrPush0_0 **/
pc_cs = 10;
//    assume((pc_cs >= 0) && (pc_cs <= 10));
    instrPush0_0(threadargs[1]);
    pc[1] = pc_cs;
  }

  thread_index = 2;
  if (active_thread[2] == 1) { /** instrPush1_0 **/
pc_cs = 9;
//    assume((pc_cs >= 0) && (pc_cs <= 10));
    instrPush1_0(threadargs[2]);
    pc[2] = pc_cs;
  }

  thread_index = 3;
  if (active_thread[3] == 1) { /** instrPush2_0 **/
pc_cs = 1;
//    assume((pc_cs >= 0) && (pc_cs <= 10));
    instrPush2_0(threadargs[3]);
    pc[3] = pc_cs;
  }

  thread_index = 4;
  if (active_thread[4] == 1) { /** instrPop3_0 **/
pc_cs = 5;
//    assume((pc_cs >= 0) && (pc_cs <= 15));
    instrPop3_0(threadargs[4]);
    pc[4] = pc_cs;
  }

  thread_index = 5;
  if (active_thread[5] == 1) { /** instrPop4_0 **/
pc_cs = 0;
//    assume((pc_cs >= 0) && (pc_cs <= 15));
    instrPop4_0(threadargs[5]);
    pc[5] = pc_cs;
  }

  thread_index = 6;
  if (active_thread[6] == 1) { /** instrPop5_0 **/
pc_cs = 15;
//    assume((pc_cs >= 0) && (pc_cs <= 15));
    instrPop5_0(threadargs[6]);
    pc[6] = pc_cs;
  }

  thread_index = 7;
  if (active_thread[7] == 1) { /** instrPop6_0 **/
pc_cs = 15;
//    assume((pc_cs >= 0) && (pc_cs <= 15));
    instrPop6_0(threadargs[7]);
    pc[7] = pc_cs;
  }

  // round 1
  thread_index = 0;
  if (active_thread[0] == 1) { /** main **/
pc_cs = 7;
//    assume((pc_cs >= pc[0]) && (pc_cs <= 7));
    main_thread();
    pc[0] = pc_cs;
  }

  thread_index = 1;
  if (active_thread[1] == 1) { /** instrPush0_0 **/
pc_cs = 10;
//    assume((pc_cs >= pc[1]) && (pc_cs <= 10));
    instrPush0_0(threadargs[1]);
    pc[1] = pc_cs;
  }

  thread_index = 2;
  if (active_thread[2] == 1) { /** instrPush1_0 **/
pc_cs = 9;
//    assume((pc_cs >= pc[2]) && (pc_cs <= 10));
    instrPush1_0(threadargs[2]);
    pc[2] = pc_cs;
  }

  thread_index = 3;
  if (active_thread[3] == 1) { /** instrPush2_0 **/
pc_cs = 10;
//    assume((pc_cs >= pc[3]) && (pc_cs <= 10));
    instrPush2_0(threadargs[3]);
    pc[3] = pc_cs;
  }


  thread_index = 4;
  if (active_thread[4] == 1) { /** instrPop3_0 **/
pc_cs = 15;
//    assume((pc_cs >= pc[4]) && (pc_cs <= 15));
    instrPop3_0(threadargs[4]);
    pc[4] = pc_cs;
  }

  thread_index = 5;
  if (active_thread[5] == 1) { /** instrPop4_0 **/
pc_cs = 15;
//    assume((pc_cs >= pc[5]) && (pc_cs <= 15));
    instrPop4_0(threadargs[5]);
    pc[5] = pc_cs;
  }
// assert(0);

  thread_index = 6;
  if (active_thread[6] == 1) { /** instrPop5_0 **/
    pc_cs = pc[6] + guess_pc();
    assume((pc_cs >= pc[6]) && (pc_cs <= 15));
    instrPop5_0(threadargs[6]);
    pc[6] = pc_cs;
  }

  thread_index = 7;
  if (active_thread[7] == 1) { /** instrPop6_0 **/
    pc_cs = pc[7] + guess_pc();
    assume((pc_cs >= pc[7]) && (pc_cs <= 15));
    instrPop6_0(threadargs[7]);
    pc[7] = pc_cs;
  }

  thread_index = 0;
  if (active_thread[0] == 1) {
    pc_cs = pc[0] + guess_pc();
    assume((pc_cs >= pc[0]) && (pc_cs <= 7));
    main_thread();
  }

  return 0;
}



