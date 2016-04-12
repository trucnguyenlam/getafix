/*  Generated 0.0.0.0.0.0 (-t4 -r3 -u1 -bcbmc) 2016-04-07 09:56:15  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define THREADS 4
#define ROUNDS 3

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

unsigned __CPROVER_bitvector[1] active_thread[THREADS+1]={1};
unsigned __CPROVER_bitvector[6] pc[THREADS+1];
unsigned __CPROVER_bitvector[7] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[6] thread_lines[] = {7, 50, 50, 50, 50};

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

void *threadargs[THREADS+1];

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
int __cs_sched_yield(unsigned __CPROVER_bitvector[7] val) {pc_cs = val + 1; return 0;}    // Version 1
//int __cs_sched_yield(unsigned __CPROVER_bitvector[7] val) {return 0;}     // Version 2


void * __cs_safe_malloc(size_t size) { void * ptr = malloc(size); assume(ptr); return ptr; }
void * __cs_unsafe_malloc(size_t size) { if(__VERIFIER_nondet_int()) return 0; return malloc(size); }
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

unsigned __CPROVER_bitvector[6] guess_pc() {unsigned __CPROVER_bitvector[6] i; return i;}

/* here */


typedef struct Cell Cell;
                                                       struct Cell
                                                       {
                                                       		Cell *pnext;
                                                       		int pdata;
                                                       };
typedef struct ThreadInfo ThreadInfo;
                                                       struct ThreadInfo
                                                       {
                                                       		unsigned int id;
                                                       		int op;
                                                       		Cell cell;
                                                       };
typedef struct Simple_Stack Simple_Stack;
                                                       struct Simple_Stack
                                                       {
                                                       		Cell *ptop;
                                                       };
                                                       Simple_Stack S;
                                                       ThreadInfo *location[2];
                                                       int collision;
                                                       void StackOp(ThreadInfo *__csLOCALPARAM__p);
                                                       int TryPerformStackOp(ThreadInfo *__csLOCALPARAM__p);
                                                       int __CS_atomicint_cas(int *__csLOCALPARAM___CS_atomicint_cas_p, int __csLOCALPARAM___CS_atomicint_cas_cmp, int __csLOCALPARAM___CS_atomicint_cas_new)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomicint_cas_p) == __csLOCALPARAM___CS_atomicint_cas_cmp)
                                                         {
                                                           *__csLOCALPARAM___CS_atomicint_cas_p = __csLOCALPARAM___CS_atomicint_cas_new;
                                                           return 1;
                                                         }
                                                         else
                                                         {
                                                           return 0;
                                                         }
                                                       
                                                       }
                                                       
                                                       int __CS_atomicti_cas(ThreadInfo **__csLOCALPARAM___CS_atomicti_cas_p, ThreadInfo *__csLOCALPARAM___CS_atomicti_cas_cmp, ThreadInfo *__csLOCALPARAM___CS_atomicti_cas_new)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomicti_cas_p) == __csLOCALPARAM___CS_atomicti_cas_cmp)
                                                         {
                                                           *__csLOCALPARAM___CS_atomicti_cas_p = __csLOCALPARAM___CS_atomicti_cas_new;
                                                           return 1;
                                                         }
                                                         else
                                                         {
                                                           return 0;
                                                         }
                                                       
                                                       }
                                                       
                                                       int __CS_atomicc_cas(Cell **__csLOCALPARAM___CS_atomicc_cas_p, Cell *__csLOCALPARAM___CS_atomicc_cas_cmp, Cell *__csLOCALPARAM___CS_atomicc_cas_new)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomicc_cas_p) == __csLOCALPARAM___CS_atomicc_cas_cmp)
                                                         {
                                                           *__csLOCALPARAM___CS_atomicc_cas_p = __csLOCALPARAM___CS_atomicc_cas_new;
                                                           return 1;
                                                         }
                                                         else
                                                         {
                                                           return 0;
                                                         }
                                                       
                                                       }
                                                       
                                                       int taken_ids[2];
                                                       int __CS_atomic_chooseUniqueId()
                                                       {
                                                         static int __csLOCAL___CS_atomic_chooseUniqueId_i;
                                                         __csLOCAL___CS_atomic_chooseUniqueId_i = nondet_int();
                                                         __VERIFIER_assume((1 <= __csLOCAL___CS_atomic_chooseUniqueId_i) && (__csLOCAL___CS_atomic_chooseUniqueId_i <= 2));
                                                         __VERIFIER_assume(!taken_ids[__csLOCAL___CS_atomic_chooseUniqueId_i - 1]);
                                                         taken_ids[__csLOCAL___CS_atomic_chooseUniqueId_i - 1] = 1;
                                                         return __csLOCAL___CS_atomic_chooseUniqueId_i;
                                                       }
                                                       
                                                       int __cs_function_LesOP_inlined = 1;
                                                       int __cs_function_TryPerformStackOp_inlined = 1;
                                                       void __CS_atomic_FinishCollision(ThreadInfo *__csLOCALPARAM___CS_atomic_FinishCollision_p)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomic_FinishCollision_p).op == 0)
                                                         {
                                                           static int __csLOCAL___CS_atomic_FinishCollision_mypid;
                                                           __csLOCAL___CS_atomic_FinishCollision_mypid = (*__csLOCALPARAM___CS_atomic_FinishCollision_p).id;
                                                           location[__csLOCAL___CS_atomic_FinishCollision_mypid - 1] = 0;
                                                         }
                                                       
                                                       }
                                                       
                                                       int __CS_atomic_TryCollision(ThreadInfo *__csLOCALPARAM___CS_atomic_TryCollision_p, ThreadInfo *__csLOCALPARAM___CS_atomic_TryCollision_q, int __csLOCALPARAM___CS_atomic_TryCollision_him)
                                                       {
                                                         static int __csLOCAL___CS_atomic_TryCollision_mypid;
                                                         __csLOCAL___CS_atomic_TryCollision_mypid = (*__csLOCALPARAM___CS_atomic_TryCollision_p).id;
                                                         if ((*__csLOCALPARAM___CS_atomic_TryCollision_p).op == 1)
                                                         {
                                                           return __CS_atomicti_cas(&location[__csLOCALPARAM___CS_atomic_TryCollision_him - 1], __csLOCALPARAM___CS_atomic_TryCollision_q, __csLOCALPARAM___CS_atomic_TryCollision_p);
                                                         }
                                                       
                                                         if ((*__csLOCALPARAM___CS_atomic_TryCollision_p).op == 0)
                                                         {
                                                           ;
                                                           static _Bool __csLOCAL___CS_atomic_TryCollision___if_cond_5;
                                                           __csLOCAL___CS_atomic_TryCollision___if_cond_5 = __CS_atomicti_cas(&location[__csLOCALPARAM___CS_atomic_TryCollision_him - 1], __csLOCALPARAM___CS_atomic_TryCollision_q, 0);
                                                           if (__csLOCAL___CS_atomic_TryCollision___if_cond_5)
                                                           {
                                                             location[__csLOCAL___CS_atomic_TryCollision_mypid - 1] = 0;
                                                             return 1;
                                                           }
                                                           else
                                                           {
                                                             return 0;
                                                           }
                                                       
                                                         }
                                                       
                                                         return 0;
                                                       }
                                                       
                                                       int __cs_function_Init_inlined = 1;
                                                       int __cs_function_Push_inlined = 1;
                                                       int __cs_function_Pop_inlined = 1;
                                                       int tick = 0;
                                                       int tickBound = 1;
                                                       int PushOpen[2][2];
                                                       int PushDone[2][2][2];
                                                       int PopOpen[2];
                                                       int PopDone[2][2][3];
                                                       int __cs_function_checkInvariant_inlined = 1;
                                                       void __CS_atomic_atomicIncr_Push(int __csLOCALPARAM___CS_atomic_atomicIncr_Push_tickStart, int __csLOCALPARAM___CS_atomic_atomicIncr_Push_localPush1)
                                                       {
                                                         PushOpen[__csLOCALPARAM___CS_atomic_atomicIncr_Push_tickStart][__csLOCALPARAM___CS_atomic_atomicIncr_Push_localPush1]++;
                                                       }
                                                       
                                                       void __CS_atomic_atomicDecrIncr_Push(int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_tickStart, int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_tick, int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1)
                                                       {
                                                         PushOpen[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_tickStart][__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1]--;
                                                         PushDone[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_tickStart][__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_tick][__csLOCALPARAM___CS_atomic_atomicDecrIncr_Push_localPush1]++;
                                                         {
                                                           assert(((((((((((((((((((((((((((((((((PopDone[1][1][2] == 0) && ((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (PopOpen[1] >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (PopOpen[0] >= 0)) && (PopDone[0][1][2] >= 0)) && (PopDone[0][0][2] >= 0)) && (((((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) - PopDone[0][1][1]) >= PopDone[1][1][1])) && (PopDone[1][1][1] >= 0)) && (PopDone[0][1][1] >= 0)) && (((PushOpen[0][1] + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (PopDone[0][0][1] >= 0)) && (((((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) - PopDone[0][1][0]) >= PopDone[1][1][0])) && (PopDone[1][1][0] >= 0)) && (PopDone[0][1][0] >= 0)) && (((PushOpen[0][0] + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (PopDone[0][0][0] >= 0)) && (PushOpen[1][1] >= 0)) && (PushOpen[0][1] >= 0)) && (PushDone[1][1][1] >= 0)) && (PushDone[0][1][1] >= 0)) && (PushDone[0][0][1] >= 0)) && (PushOpen[1][0] >= 0)) && (PushOpen[0][0] >= 0)) && (PushDone[1][1][0] >= 0)) && (PushDone[0][1][0] >= 0)) && (PushDone[0][0][0] >= 0)) || (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + ((-1) * PushDone[0][0][1])) - PushDone[0][0][0]) >= 0) && ((((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) - PushDone[0][0][1]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[0][1]) - PushDone[0][0][0]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) >= 0)) && (((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + ((-1) * PushDone[0][0][1])) - PushDone[0][0][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) - PushDone[0][0][1]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[0][1]) - PushDone[0][0][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) >= 0)) && (((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PushOpen[0][1]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PushDone[0][0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PushDone[0][0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) - PopDone[0][0][1]) >= 0)) && (((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) - PopDone[0][0][1]) >= 0)) && (((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) - PushDone[0][0][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushOpen[0][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && (PopOpen[1] >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + (2 * PushDone[0][1][0])) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) >= 0)) && (PopOpen[0] >= 0)) && (PopDone[1][1][2] >= 1)) && (PopDone[0][1][2] >= 0)) && (PopDone[0][0][2] >= 0)) && (((((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) - PopDone[0][1][1]) >= PopDone[1][1][1])) && (PopDone[1][1][1] >= 0)) && (PopDone[0][1][1] >= 0)) && (((((PushOpen[1][1] + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (((PushOpen[0][1] + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (PopDone[0][0][1] >= 0)) && (((((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) - PopDone[0][1][0]) >= PopDone[1][1][0])) && (PopDone[1][1][0] >= 0)) && (PopDone[0][1][0] >= 0)) && (((((PushOpen[1][0] + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + (2 * PushDone[0][1][0])) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (((((PushOpen[1][0] + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (((PushOpen[0][0] + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (PopDone[0][0][0] >= 0)) && ((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && (PushOpen[1][1] >= 0)) && (PushOpen[0][1] >= 0)) && (PushDone[1][1][1] >= 0)) && (PushDone[0][1][1] >= 0)) && (PushDone[0][0][1] >= 0)) && ((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) >= 0)) && (PushOpen[1][0] >= 0)) && (PushOpen[0][0] >= 0)) && (PushDone[1][1][0] >= 0)) && (PushDone[0][1][0] >= 0)) && (PushDone[0][0][0] >= 0)));
                                                           __exit__checkInvariant_1:
                                                           ;
                                                       
                                                         }
                                                         0;
                                                       }
                                                       
                                                       void __CS_atomic_atomicIncr_Pop(int __csLOCALPARAM___CS_atomic_atomicIncr_Pop_tickStart)
                                                       {
                                                         PopOpen[__csLOCALPARAM___CS_atomic_atomicIncr_Pop_tickStart]++;
                                                       }
                                                       
                                                       void __CS_atomic_atomicDecrIncr_Pop(int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_tickStart, int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_tick, int __csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_localPop_ret)
                                                       {
                                                         PopOpen[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_tickStart]--;
                                                         PopDone[__csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_tickStart][__csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_tick][__csLOCALPARAM___CS_atomic_atomicDecrIncr_Pop_localPop_ret]++;
                                                         {
                                                           assert(((((((((((((((((((((((((((((((((PopDone[1][1][2] == 0) && ((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (PopOpen[1] >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (PopOpen[0] >= 0)) && (PopDone[0][1][2] >= 0)) && (PopDone[0][0][2] >= 0)) && (((((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) - PopDone[0][1][1]) >= PopDone[1][1][1])) && (PopDone[1][1][1] >= 0)) && (PopDone[0][1][1] >= 0)) && (((PushOpen[0][1] + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (PopDone[0][0][1] >= 0)) && (((((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) - PopDone[0][1][0]) >= PopDone[1][1][0])) && (PopDone[1][1][0] >= 0)) && (PopDone[0][1][0] >= 0)) && (((PushOpen[0][0] + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (PopDone[0][0][0] >= 0)) && (PushOpen[1][1] >= 0)) && (PushOpen[0][1] >= 0)) && (PushDone[1][1][1] >= 0)) && (PushDone[0][1][1] >= 0)) && (PushDone[0][0][1] >= 0)) && (PushOpen[1][0] >= 0)) && (PushOpen[0][0] >= 0)) && (PushDone[1][1][0] >= 0)) && (PushDone[0][1][0] >= 0)) && (PushDone[0][0][0] >= 0)) || (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + ((-1) * PushDone[0][0][1])) - PushDone[0][0][0]) >= 0) && ((((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) - PushDone[0][0][1]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[0][1]) - PushDone[0][0][0]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && (((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) >= 0)) && (((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) >= 0)) && ((((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((((PopOpen[1] + (2 * PopOpen[0])) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + ((-1) * PushDone[0][0][1])) - PushDone[0][0][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) - PushDone[0][0][1]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) - PushDone[0][0][1]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[0][0][1]) - PushDone[0][0][1]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[0][1]) - PushDone[0][0][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) + PushOpen[0][0]) >= 0)) && (((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][1]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) + PushOpen[0][1]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][1]) + PopDone[0][1][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PushDone[0][0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PushDone[0][0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) - PopDone[0][0][1]) >= 0)) && (((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) + PushDone[0][1][0]) - PopDone[0][0][1]) >= 0)) && (((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) + PushOpen[0][0]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) - PushDone[0][0][0]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PopDone[0][0][0]) - PushDone[0][0][0]) >= 0)) && ((((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushOpen[0][0]) >= 0)) && ((((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && ((((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) + PushDone[0][1][0]) >= 0)) && (((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) + PushOpen[0][0]) >= 0)) && ((((PopOpen[1] + PopOpen[0]) + PopDone[1][1][0]) + PopDone[0][1][0]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (((((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) >= 0)) && (((((PopOpen[1] + PushOpen[1][1]) + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && (PopOpen[1] >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + (2 * PushDone[0][1][0])) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) >= 0)) && (((((PopOpen[0] + PushOpen[1][0]) + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) >= 0)) && (PopOpen[0] >= 0)) && (PopDone[1][1][2] >= 1)) && (PopDone[0][1][2] >= 0)) && (PopDone[0][0][2] >= 0)) && (((((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) - PopDone[0][0][1]) - PopDone[0][1][1]) >= PopDone[1][1][1])) && (PopDone[1][1][1] >= 0)) && (PopDone[0][1][1] >= 0)) && (((((PushOpen[1][1] + (2 * PushOpen[0][1])) + PushDone[1][1][1]) + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (((PushOpen[0][1] + PushDone[0][1][1]) + PushDone[0][0][1]) >= PopDone[0][0][1])) && (PopDone[0][0][1] >= 0)) && (((((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) - PopDone[0][0][0]) - PopDone[0][1][0]) >= PopDone[1][1][0])) && (PopDone[1][1][0] >= 0)) && (PopDone[0][1][0] >= 0)) && (((((PushOpen[1][0] + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + (2 * PushDone[0][1][0])) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (((((PushOpen[1][0] + (2 * PushOpen[0][0])) + PushDone[1][1][0]) + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (((PushOpen[0][0] + PushDone[0][1][0]) + PushDone[0][0][0]) >= PopDone[0][0][0])) && (PopDone[0][0][0] >= 0)) && ((((PushOpen[1][1] + PushOpen[0][1]) + PushDone[1][1][1]) + PushDone[0][1][1]) >= 0)) && (PushOpen[1][1] >= 0)) && (PushOpen[0][1] >= 0)) && (PushDone[1][1][1] >= 0)) && (PushDone[0][1][1] >= 0)) && (PushDone[0][0][1] >= 0)) && ((((PushOpen[1][0] + PushOpen[0][0]) + PushDone[1][1][0]) + PushDone[0][1][0]) >= 0)) && (PushOpen[1][0] >= 0)) && (PushOpen[0][0] >= 0)) && (PushDone[1][1][0] >= 0)) && (PushDone[0][1][0] >= 0)) && (PushDone[0][0][0] >= 0)));
                                                           __exit__checkInvariant_2:
                                                           ;
                                                       
                                                         }
                                                         0;
                                                       }
                                                       
                                                       void *instrPush0_0(void *__csLOCALPARAM_instrPush0_unused)
                                                       {
                                                       static int __csLOCAL_instrPush0_tickStart;
IF(1,0,tinstrPush0_0_1)                                __csLOCAL_instrPush0_tickStart = tick;
tinstrPush0_0_1: IF(1,1,tinstrPush0_0_2)               __CS_atomic_atomicIncr_Push(__csLOCAL_instrPush0_tickStart, 1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
                                                       __cs_init_scalar(&__csLOCAL_Push_ti, sizeof(ThreadInfo *));
tinstrPush0_0_2: IF(1,2,tinstrPush0_0_3)               __csLOCAL_Push_ti = __cs_safe_malloc(sizeof(*__csLOCAL_Push_ti));
tinstrPush0_0_3: IF(1,3,tinstrPush0_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush0_0_4: IF(1,4,tinstrPush0_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_6;
                                                       static int __retval__TryPerformStackOp_1;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush0_0_5: IF(1,5,tinstrPush0_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush0_0_6: IF(1,6,tinstrPush0_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush0_0_7: IF(1,7,tinstrPush0_0_8)               __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush0_0_8: IF(1,8,tinstrPush0_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush0_0_9: IF(1,9,tinstrPush0_0_10)              __retval__TryPerformStackOp_1 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_1;
                                                       			}
                                                       			GUARD(1,10);
tinstrPush0_0_10: IF(1,10,tinstrPush0_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush0_0_11: IF(1,11,tinstrPush0_0_12)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush0_0_12: IF(1,12,tinstrPush0_0_13)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       				{
tinstrPush0_0_13: IF(1,13,tinstrPush0_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_14: IF(1,14,tinstrPush0_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_1 = 1;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				GUARD(1,15);
tinstrPush0_0_15: IF(1,15,tinstrPush0_0_16)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush0_0_16: IF(1,16,tinstrPush0_0_17)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
tinstrPush0_0_17: IF(1,17,tinstrPush0_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_1 = 1;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				else
                                                       				{ GUARD(1,18)
tinstrPush0_0_18: IF(1,18,tinstrPush0_0_19)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_19: IF(1,19,tinstrPush0_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_1 = 0;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				GUARD(1,20);
                                                       			}
                                                       			GUARD(1,20);
                                                       			__exit__TryPerformStackOp_1: GUARD(1,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_6 = !__retval__TryPerformStackOp_1;
                                                       if (__csLOCAL_Push___if_cond_6)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush0_0_20: IF(1,20,tinstrPush0_0_21)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
tinstrPush0_0_21: IF(1,21,tinstrPush0_0_22)            (*__csLOCALPARAM_LesOP_p).id = __CS_atomic_chooseUniqueId();
                                                       				{
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush0_0_22: IF(1,22,tinstrPush0_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush0_0_23: IF(1,23,tinstrPush0_0_24)            location[__csLOCAL_LesOP_mypid - 1] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush0_0_24: IF(1,24,tinstrPush0_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush0_0_25: IF(1,25,tinstrPush0_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       					{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush0_0_26: IF(1,26,tinstrPush0_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him - 1];
tinstrPush0_0_27: IF(1,27,tinstrPush0_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush0_0_28: IF(1,28,tinstrPush0_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       							{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush0_0_29: IF(1,29,tinstrPush0_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him);
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       								{
                                                       goto __exit__LesOP_1;
                                                       								}
                                                       								else
                                                       								{ 
                                                       goto stack;
                                                       								}
                                                       								;
                                                       							}
                                                       							else
                                                       							{ GUARD(1,30)
tinstrPush0_0_30: IF(1,30,tinstrPush0_0_31)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_1;
                                                       							}
                                                       							GUARD(1,31);
                                                       						}
                                                       						GUARD(1,31);
                                                       					}
                                                       					GUARD(1,31);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPush0_0_31: IF(1,31,tinstrPush0_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       					{
tinstrPush0_0_32: IF(1,32,tinstrPush0_0_33)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_1;
                                                       					}
                                                       					GUARD(1,33);
                                                       					stack: GUARD(1,33)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_2;
                                                       					{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush0_0_33: IF(1,33,tinstrPush0_0_34)            __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush0_0_34: IF(1,34,tinstrPush0_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       						{
tinstrPush0_0_35: IF(1,35,tinstrPush0_0_36)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush0_0_36: IF(1,36,tinstrPush0_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush0_0_37: IF(1,37,tinstrPush0_0_38)            __retval__TryPerformStackOp_2 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						GUARD(1,38);
tinstrPush0_0_38: IF(1,38,tinstrPush0_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       						{
tinstrPush0_0_39: IF(1,39,tinstrPush0_0_40)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush0_0_40: IF(1,40,tinstrPush0_0_41)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       							{
tinstrPush0_0_41: IF(1,41,tinstrPush0_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_42: IF(1,42,tinstrPush0_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_2 = 1;
                                                       goto __exit__TryPerformStackOp_2;
                                                       							}
                                                       							GUARD(1,43);
tinstrPush0_0_43: IF(1,43,tinstrPush0_0_44)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush0_0_44: IF(1,44,tinstrPush0_0_45)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       							{
tinstrPush0_0_45: IF(1,45,tinstrPush0_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_2 = 1;
                                                       goto __exit__TryPerformStackOp_2;
                                                       							}
                                                       							else
                                                       							{ GUARD(1,46)
tinstrPush0_0_46: IF(1,46,tinstrPush0_0_47)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_47: IF(1,47,tinstrPush0_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_2 = 0;
                                                       goto __exit__TryPerformStackOp_2;
                                                       							}
                                                       							GUARD(1,48);
                                                       						}
                                                       						GUARD(1,48);
                                                       						__exit__TryPerformStackOp_2: GUARD(1,48)
                                                       ;
                                                       					};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_2 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       					{
                                                       goto __exit__LesOP_1;
                                                       					}
                                                       					;
                                                       				};
tinstrPush0_0_48: IF(1,48,tinstrPush0_0_49)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_1: GUARD(1,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(1,49);
                                                       		__exit__Push_1: GUARD(1,49)
                                                       ;
                                                       	};
                                                       0;
tinstrPush0_0_49: IF(1,49,tinstrPush0_0_50)            __CS_atomic_atomicDecrIncr_Push(__csLOCAL_instrPush0_tickStart, tick, 1);
                                                       goto __exit_instrPush0;
                                                       	__exit_instrPush0: GUARD(1,50)
                                                       ;
tinstrPush0_0_50:                                      STOP_NONVOID(50);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush1_0(void *__csLOCALPARAM_instrPush1_unused)
                                                       {
                                                       static int __csLOCAL_instrPush1_tickStart;
IF(2,0,tinstrPush1_0_1)                                __csLOCAL_instrPush1_tickStart = tick;
tinstrPush1_0_1: IF(2,1,tinstrPush1_0_2)               __CS_atomic_atomicIncr_Push(__csLOCAL_instrPush1_tickStart, 1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
                                                       __cs_init_scalar(&__csLOCAL_Push_ti, sizeof(ThreadInfo *));
tinstrPush1_0_2: IF(2,2,tinstrPush1_0_3)               __csLOCAL_Push_ti = __cs_safe_malloc(sizeof(*__csLOCAL_Push_ti));
tinstrPush1_0_3: IF(2,3,tinstrPush1_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush1_0_4: IF(2,4,tinstrPush1_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_6;
                                                       static int __retval__TryPerformStackOp_3;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush1_0_5: IF(2,5,tinstrPush1_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush1_0_6: IF(2,6,tinstrPush1_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush1_0_7: IF(2,7,tinstrPush1_0_8)               __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush1_0_8: IF(2,8,tinstrPush1_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush1_0_9: IF(2,9,tinstrPush1_0_10)              __retval__TryPerformStackOp_3 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_3;
                                                       			}
                                                       			GUARD(2,10);
tinstrPush1_0_10: IF(2,10,tinstrPush1_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush1_0_11: IF(2,11,tinstrPush1_0_12)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush1_0_12: IF(2,12,tinstrPush1_0_13)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       				{
tinstrPush1_0_13: IF(2,13,tinstrPush1_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_14: IF(2,14,tinstrPush1_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_3 = 1;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				GUARD(2,15);
tinstrPush1_0_15: IF(2,15,tinstrPush1_0_16)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush1_0_16: IF(2,16,tinstrPush1_0_17)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
tinstrPush1_0_17: IF(2,17,tinstrPush1_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_3 = 1;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				else
                                                       				{ GUARD(2,18)
tinstrPush1_0_18: IF(2,18,tinstrPush1_0_19)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_19: IF(2,19,tinstrPush1_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_3 = 0;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				GUARD(2,20);
                                                       			}
                                                       			GUARD(2,20);
                                                       			__exit__TryPerformStackOp_3: GUARD(2,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_6 = !__retval__TryPerformStackOp_3;
                                                       if (__csLOCAL_Push___if_cond_6)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush1_0_20: IF(2,20,tinstrPush1_0_21)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
tinstrPush1_0_21: IF(2,21,tinstrPush1_0_22)            (*__csLOCALPARAM_LesOP_p).id = __CS_atomic_chooseUniqueId();
                                                       				{
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush1_0_22: IF(2,22,tinstrPush1_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush1_0_23: IF(2,23,tinstrPush1_0_24)            location[__csLOCAL_LesOP_mypid - 1] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush1_0_24: IF(2,24,tinstrPush1_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush1_0_25: IF(2,25,tinstrPush1_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       					{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush1_0_26: IF(2,26,tinstrPush1_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him - 1];
tinstrPush1_0_27: IF(2,27,tinstrPush1_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush1_0_28: IF(2,28,tinstrPush1_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       							{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush1_0_29: IF(2,29,tinstrPush1_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him);
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       								{
                                                       goto __exit__LesOP_2;
                                                       								}
                                                       								else
                                                       								{ 
                                                       goto stack;
                                                       								}
                                                       								;
                                                       							}
                                                       							else
                                                       							{ GUARD(2,30)
tinstrPush1_0_30: IF(2,30,tinstrPush1_0_31)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_2;
                                                       							}
                                                       							GUARD(2,31);
                                                       						}
                                                       						GUARD(2,31);
                                                       					}
                                                       					GUARD(2,31);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPush1_0_31: IF(2,31,tinstrPush1_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       					{
tinstrPush1_0_32: IF(2,32,tinstrPush1_0_33)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_2;
                                                       					}
                                                       					GUARD(2,33);
                                                       					stack: GUARD(2,33)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_4;
                                                       					{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush1_0_33: IF(2,33,tinstrPush1_0_34)            __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush1_0_34: IF(2,34,tinstrPush1_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       						{
tinstrPush1_0_35: IF(2,35,tinstrPush1_0_36)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush1_0_36: IF(2,36,tinstrPush1_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush1_0_37: IF(2,37,tinstrPush1_0_38)            __retval__TryPerformStackOp_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						GUARD(2,38);
tinstrPush1_0_38: IF(2,38,tinstrPush1_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       						{
tinstrPush1_0_39: IF(2,39,tinstrPush1_0_40)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush1_0_40: IF(2,40,tinstrPush1_0_41)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       							{
tinstrPush1_0_41: IF(2,41,tinstrPush1_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_42: IF(2,42,tinstrPush1_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_4 = 1;
                                                       goto __exit__TryPerformStackOp_4;
                                                       							}
                                                       							GUARD(2,43);
tinstrPush1_0_43: IF(2,43,tinstrPush1_0_44)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush1_0_44: IF(2,44,tinstrPush1_0_45)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       							{
tinstrPush1_0_45: IF(2,45,tinstrPush1_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_4 = 1;
                                                       goto __exit__TryPerformStackOp_4;
                                                       							}
                                                       							else
                                                       							{ GUARD(2,46)
tinstrPush1_0_46: IF(2,46,tinstrPush1_0_47)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_47: IF(2,47,tinstrPush1_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_4 = 0;
                                                       goto __exit__TryPerformStackOp_4;
                                                       							}
                                                       							GUARD(2,48);
                                                       						}
                                                       						GUARD(2,48);
                                                       						__exit__TryPerformStackOp_4: GUARD(2,48)
                                                       ;
                                                       					};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_4 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       					{
                                                       goto __exit__LesOP_2;
                                                       					}
                                                       					;
                                                       				};
tinstrPush1_0_48: IF(2,48,tinstrPush1_0_49)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_2: GUARD(2,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(2,49);
                                                       		__exit__Push_2: GUARD(2,49)
                                                       ;
                                                       	};
                                                       0;
tinstrPush1_0_49: IF(2,49,tinstrPush1_0_50)            __CS_atomic_atomicDecrIncr_Push(__csLOCAL_instrPush1_tickStart, tick, 1);
                                                       goto __exit_instrPush1;
                                                       	__exit_instrPush1: GUARD(2,50)
                                                       ;
tinstrPush1_0_50:                                      STOP_NONVOID(50);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush2_0(void *__csLOCALPARAM_instrPush2_unused)
                                                       {
                                                       static int __csLOCAL_instrPush2_tickStart;
IF(3,0,tinstrPush2_0_1)                                __csLOCAL_instrPush2_tickStart = tick;
tinstrPush2_0_1: IF(3,1,tinstrPush2_0_2)               __CS_atomic_atomicIncr_Push(__csLOCAL_instrPush2_tickStart, 1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
                                                       __cs_init_scalar(&__csLOCAL_Push_ti, sizeof(ThreadInfo *));
tinstrPush2_0_2: IF(3,2,tinstrPush2_0_3)               __csLOCAL_Push_ti = __cs_safe_malloc(sizeof(*__csLOCAL_Push_ti));
tinstrPush2_0_3: IF(3,3,tinstrPush2_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush2_0_4: IF(3,4,tinstrPush2_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_6;
                                                       static int __retval__TryPerformStackOp_5;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush2_0_5: IF(3,5,tinstrPush2_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush2_0_6: IF(3,6,tinstrPush2_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush2_0_7: IF(3,7,tinstrPush2_0_8)               __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush2_0_8: IF(3,8,tinstrPush2_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush2_0_9: IF(3,9,tinstrPush2_0_10)              __retval__TryPerformStackOp_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_5;
                                                       			}
                                                       			GUARD(3,10);
tinstrPush2_0_10: IF(3,10,tinstrPush2_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush2_0_11: IF(3,11,tinstrPush2_0_12)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush2_0_12: IF(3,12,tinstrPush2_0_13)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       				{
tinstrPush2_0_13: IF(3,13,tinstrPush2_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_14: IF(3,14,tinstrPush2_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_5 = 1;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				GUARD(3,15);
tinstrPush2_0_15: IF(3,15,tinstrPush2_0_16)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush2_0_16: IF(3,16,tinstrPush2_0_17)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
tinstrPush2_0_17: IF(3,17,tinstrPush2_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_5 = 1;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				else
                                                       				{ GUARD(3,18)
tinstrPush2_0_18: IF(3,18,tinstrPush2_0_19)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_19: IF(3,19,tinstrPush2_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_5 = 0;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				GUARD(3,20);
                                                       			}
                                                       			GUARD(3,20);
                                                       			__exit__TryPerformStackOp_5: GUARD(3,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_6 = !__retval__TryPerformStackOp_5;
                                                       if (__csLOCAL_Push___if_cond_6)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush2_0_20: IF(3,20,tinstrPush2_0_21)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
tinstrPush2_0_21: IF(3,21,tinstrPush2_0_22)            (*__csLOCALPARAM_LesOP_p).id = __CS_atomic_chooseUniqueId();
                                                       				{
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush2_0_22: IF(3,22,tinstrPush2_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush2_0_23: IF(3,23,tinstrPush2_0_24)            location[__csLOCAL_LesOP_mypid - 1] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush2_0_24: IF(3,24,tinstrPush2_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush2_0_25: IF(3,25,tinstrPush2_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       					{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush2_0_26: IF(3,26,tinstrPush2_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him - 1];
tinstrPush2_0_27: IF(3,27,tinstrPush2_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush2_0_28: IF(3,28,tinstrPush2_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       							{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush2_0_29: IF(3,29,tinstrPush2_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him);
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       								{
                                                       goto __exit__LesOP_3;
                                                       								}
                                                       								else
                                                       								{ 
                                                       goto stack;
                                                       								}
                                                       								;
                                                       							}
                                                       							else
                                                       							{ GUARD(3,30)
tinstrPush2_0_30: IF(3,30,tinstrPush2_0_31)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_3;
                                                       							}
                                                       							GUARD(3,31);
                                                       						}
                                                       						GUARD(3,31);
                                                       					}
                                                       					GUARD(3,31);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPush2_0_31: IF(3,31,tinstrPush2_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       					{
tinstrPush2_0_32: IF(3,32,tinstrPush2_0_33)            __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_3;
                                                       					}
                                                       					GUARD(3,33);
                                                       					stack: GUARD(3,33)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_6;
                                                       					{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush2_0_33: IF(3,33,tinstrPush2_0_34)            __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPush2_0_34: IF(3,34,tinstrPush2_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       						{
tinstrPush2_0_35: IF(3,35,tinstrPush2_0_36)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush2_0_36: IF(3,36,tinstrPush2_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPush2_0_37: IF(3,37,tinstrPush2_0_38)            __retval__TryPerformStackOp_6 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						GUARD(3,38);
tinstrPush2_0_38: IF(3,38,tinstrPush2_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       						{
tinstrPush2_0_39: IF(3,39,tinstrPush2_0_40)            __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPush2_0_40: IF(3,40,tinstrPush2_0_41)            if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       							{
tinstrPush2_0_41: IF(3,41,tinstrPush2_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_42: IF(3,42,tinstrPush2_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_6 = 1;
                                                       goto __exit__TryPerformStackOp_6;
                                                       							}
                                                       							GUARD(3,43);
tinstrPush2_0_43: IF(3,43,tinstrPush2_0_44)            __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush2_0_44: IF(3,44,tinstrPush2_0_45)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       							{
tinstrPush2_0_45: IF(3,45,tinstrPush2_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_6 = 1;
                                                       goto __exit__TryPerformStackOp_6;
                                                       							}
                                                       							else
                                                       							{ GUARD(3,46)
tinstrPush2_0_46: IF(3,46,tinstrPush2_0_47)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_47: IF(3,47,tinstrPush2_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_6 = 0;
                                                       goto __exit__TryPerformStackOp_6;
                                                       							}
                                                       							GUARD(3,48);
                                                       						}
                                                       						GUARD(3,48);
                                                       						__exit__TryPerformStackOp_6: GUARD(3,48)
                                                       ;
                                                       					};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_6 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       					{
                                                       goto __exit__LesOP_3;
                                                       					}
                                                       					;
                                                       				};
tinstrPush2_0_48: IF(3,48,tinstrPush2_0_49)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_3: GUARD(3,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(3,49);
                                                       		__exit__Push_3: GUARD(3,49)
                                                       ;
                                                       	};
                                                       0;
tinstrPush2_0_49: IF(3,49,tinstrPush2_0_50)            __CS_atomic_atomicDecrIncr_Push(__csLOCAL_instrPush2_tickStart, tick, 1);
                                                       goto __exit_instrPush2;
                                                       	__exit_instrPush2: GUARD(3,50)
                                                       ;
tinstrPush2_0_50:                                      STOP_NONVOID(50);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop3_0(void *__csLOCALPARAM_instrPop3_unused)
                                                       {
                                                       static int __csLOCAL_instrPop3_tickStart;
IF(4,0,tinstrPop3_0_1)                                 __csLOCAL_instrPop3_tickStart = tick;
tinstrPop3_0_1: IF(4,1,tinstrPop3_0_2)                 __CS_atomic_atomicIncr_Pop(__csLOCAL_instrPop3_tickStart);
                                                       static int __csLOCAL_instrPop3_localPop_ret;
                                                       static int __retval__Pop_1;
                                                       	{
                                                       static ThreadInfo *__csLOCAL_Pop_ti;
                                                       __cs_init_scalar(&__csLOCAL_Pop_ti, sizeof(ThreadInfo *));
tinstrPop3_0_2: IF(4,2,tinstrPop3_0_3)                 __csLOCAL_Pop_ti = __cs_safe_malloc(sizeof(*__csLOCAL_Pop_ti));
tinstrPop3_0_3: IF(4,3,tinstrPop3_0_4)                 (*__csLOCAL_Pop_ti).op = 0;
                                                       ;
                                                       static _Bool __csLOCAL_Pop___if_cond_7;
                                                       static int __retval__TryPerformStackOp_7;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop3_0_4: IF(4,4,tinstrPop3_0_5)                 __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Pop_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPop3_0_5: IF(4,5,tinstrPop3_0_6)                 if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPop3_0_6: IF(4,6,tinstrPop3_0_7)                 __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPop3_0_7: IF(4,7,tinstrPop3_0_8)                 (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPop3_0_8: IF(4,8,tinstrPop3_0_9)                 __retval__TryPerformStackOp_7 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_7;
                                                       			}
                                                       			GUARD(4,9);
tinstrPop3_0_9: IF(4,9,tinstrPop3_0_10)                if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPop3_0_10: IF(4,10,tinstrPop3_0_11)              __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPop3_0_11: IF(4,11,tinstrPop3_0_12)              if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       				{
tinstrPop3_0_12: IF(4,12,tinstrPop3_0_13)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_13: IF(4,13,tinstrPop3_0_14)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_7 = 1;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				GUARD(4,14);
tinstrPop3_0_14: IF(4,14,tinstrPop3_0_15)              __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop3_0_15: IF(4,15,tinstrPop3_0_16)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
tinstrPop3_0_16: IF(4,16,tinstrPop3_0_17)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_7 = 1;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				else
                                                       				{ GUARD(4,17)
tinstrPop3_0_17: IF(4,17,tinstrPop3_0_18)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_18: IF(4,18,tinstrPop3_0_19)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_7 = 0;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				GUARD(4,19);
                                                       			}
                                                       			GUARD(4,19);
                                                       			__exit__TryPerformStackOp_7: GUARD(4,19)
                                                       ;
                                                       		};
                                                       __csLOCAL_Pop___if_cond_7 = !__retval__TryPerformStackOp_7;
                                                       if (__csLOCAL_Pop___if_cond_7)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPop3_0_19: IF(4,19,tinstrPop3_0_20)              __csLOCALPARAM_LesOP_p = __csLOCAL_Pop_ti;
tinstrPop3_0_20: IF(4,20,tinstrPop3_0_21)              (*__csLOCALPARAM_LesOP_p).id = __CS_atomic_chooseUniqueId();
                                                       				{
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPop3_0_21: IF(4,21,tinstrPop3_0_22)              __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPop3_0_22: IF(4,22,tinstrPop3_0_23)              location[__csLOCAL_LesOP_mypid - 1] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPop3_0_23: IF(4,23,tinstrPop3_0_24)              __csLOCAL_LesOP_him = collision;
tinstrPop3_0_24: IF(4,24,tinstrPop3_0_25)              __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       					{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPop3_0_25: IF(4,25,tinstrPop3_0_26)              __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him - 1];
tinstrPop3_0_26: IF(4,26,tinstrPop3_0_27)              if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPop3_0_27: IF(4,27,tinstrPop3_0_28)              __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       							{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPop3_0_28: IF(4,28,tinstrPop3_0_29)              __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him);
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       								{
                                                       goto __exit__LesOP_4;
                                                       								}
                                                       								else
                                                       								{ 
                                                       goto stack;
                                                       								}
                                                       								;
                                                       							}
                                                       							else
                                                       							{ GUARD(4,29)
tinstrPop3_0_29: IF(4,29,tinstrPop3_0_30)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_4;
                                                       							}
                                                       							GUARD(4,30);
                                                       						}
                                                       						GUARD(4,30);
                                                       					}
                                                       					GUARD(4,30);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPop3_0_30: IF(4,30,tinstrPop3_0_31)              __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid - 1], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       					{
tinstrPop3_0_31: IF(4,31,tinstrPop3_0_32)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_4;
                                                       					}
                                                       					GUARD(4,32);
                                                       					stack: GUARD(4,32)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_8;
                                                       					{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop3_0_32: IF(4,32,tinstrPop3_0_33)              __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_thead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_tnext;
tinstrPop3_0_33: IF(4,33,tinstrPop3_0_34)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       						{
tinstrPop3_0_34: IF(4,34,tinstrPop3_0_35)              __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPop3_0_35: IF(4,35,tinstrPop3_0_36)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_thead;
tinstrPop3_0_36: IF(4,36,tinstrPop3_0_37)              __retval__TryPerformStackOp_8 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						GUARD(4,37);
tinstrPop3_0_37: IF(4,37,tinstrPop3_0_38)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       						{
tinstrPop3_0_38: IF(4,38,tinstrPop3_0_39)              __csLOCAL_TryPerformStackOp_thead = S.ptop;
tinstrPop3_0_39: IF(4,39,tinstrPop3_0_40)              if (__csLOCAL_TryPerformStackOp_thead == 0)
                                                       							{
tinstrPop3_0_40: IF(4,40,tinstrPop3_0_41)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_41: IF(4,41,tinstrPop3_0_42)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_8 = 1;
                                                       goto __exit__TryPerformStackOp_8;
                                                       							}
                                                       							GUARD(4,42);
tinstrPop3_0_42: IF(4,42,tinstrPop3_0_43)              __csLOCAL_TryPerformStackOp_tnext = (*__csLOCAL_TryPerformStackOp_thead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop3_0_43: IF(4,43,tinstrPop3_0_44)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_thead, __csLOCAL_TryPerformStackOp_tnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       							{
tinstrPop3_0_44: IF(4,44,tinstrPop3_0_45)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_thead;
                                                       __retval__TryPerformStackOp_8 = 1;
                                                       goto __exit__TryPerformStackOp_8;
                                                       							}
                                                       							else
                                                       							{ GUARD(4,45)
tinstrPop3_0_45: IF(4,45,tinstrPop3_0_46)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_46: IF(4,46,tinstrPop3_0_47)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_8 = 0;
                                                       goto __exit__TryPerformStackOp_8;
                                                       							}
                                                       							GUARD(4,47);
                                                       						}
                                                       						GUARD(4,47);
                                                       						__exit__TryPerformStackOp_8: GUARD(4,47)
                                                       ;
                                                       					};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_8 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       					{
                                                       goto __exit__LesOP_4;
                                                       					}
                                                       					;
                                                       				};
tinstrPop3_0_47: IF(4,47,tinstrPop3_0_48)              __VERIFIER_assume(0);
                                                       				__exit__LesOP_4: GUARD(4,48)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(4,48);
                                                       static int __csLOCAL_Pop_v;
tinstrPop3_0_48: IF(4,48,tinstrPop3_0_49)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_ti).cell.pdata;
                                                       __retval__Pop_1 = __csLOCAL_Pop_v;
                                                       goto __exit__Pop_1;
                                                       		__exit__Pop_1: GUARD(4,49)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop3_localPop_ret = __retval__Pop_1;
tinstrPop3_0_49: IF(4,49,tinstrPop3_0_50)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop3_tickStart, tick, __csLOCAL_instrPop3_localPop_ret);
                                                       goto __exit_instrPop3;
                                                       	__exit_instrPop3: GUARD(4,50)
                                                       ;
tinstrPop3_0_50:                                       STOP_NONVOID(50);
                                                       }
                                                       
                                                       
                                                       
                                                       int main_thread(void)
                                                       {
IF(0,0,tmain_1)                                        	{
                                                       S.ptop = 0;
                                                       		__exit__Init_1: GUARD(0,1)
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
                                                       __cs_create(&__csLOCAL_main_tid1, 0, &instrPush0_0, 0, 1);
tmain_1: IF(0,1,tmain_2)                               __cs_create(&__csLOCAL_main_tid2, 0, &instrPush1_0, 0, 2);
tmain_2: IF(0,2,tmain_3)                               __cs_create(&__csLOCAL_main_tid3, 0, &instrPush2_0, 0, 3);
tmain_3: IF(0,3,tmain_4)                               __cs_create(&__csLOCAL_main_tid4, 0, &instrPop3_0, 0, 4);
tmain_4: IF(0,4,tmain_5)                               if (!(tick < tickBound))
                                                       	{
                                                       goto __exit_loop_1;
                                                       	}
                                                       	;
                                                       	{
tmain_5: IF(0,5,tmain_6)                               tick++;
                                                       	};
tmain_6: IF(0,6,tmain_7)                               assume(!(tick < tickBound));
                                                       	__exit_loop_1: GUARD(0,7)
                                                       ;
                                                       	__exit_main: GUARD(0,7)
                                                       ;
tmain_7:                                               STOP_NONVOID(7);
                                                       }
                                                       
                                                       
                                                       
                                                       int main(void) {
                                                                 // round 0
                                                                 thread_index = 0;
                                                                 pc_cs = pc[0] + guess_pc();
                                                                 assume((pc_cs > 0) && (pc_cs <= 7));
                                                                 main_thread();
                                                                 pc[0] = pc_cs;
                                                       
                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** instrPush0_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 50));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 50));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 50));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 50));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 // round 1
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume((pc_cs >= pc[0]) && (pc_cs <= 7));
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** instrPush0_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 50));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 50));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 50));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= pc[4]) && (pc_cs <= 50));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 // round 2
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume((pc_cs >= pc[0]) && (pc_cs <= 7));
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** instrPush0_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 50));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 50));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 50));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= pc[4]) && (pc_cs <= 50));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) {
                                                                    pc_cs = pc[0] + guess_pc();
                                                                    assume((pc_cs >= pc[0]) && (pc_cs <= 7));
                                                                    main_thread();
                                                                 }
                                                       
                                                          return 0;
                                                       }
                                                       


