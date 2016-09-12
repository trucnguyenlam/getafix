/*  Generated 0.0.0.0.0.0 (-t7 -r2 -u1 -bcbmc) 2016-04-19 13:39:03  */
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

unsigned __CPROVER_bitvector[1] active_thread[THREADS+1]={1};
unsigned __CPROVER_bitvector[6] pc[THREADS+1];
unsigned __CPROVER_bitvector[7] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[6] thread_lines[] = {7, 51, 51, 51, 52, 52, 52, 52};

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
                                                       ThreadInfo *location[8];
                                                       int collision;
                                                       int unique_id = 0;
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
                                                       
                                                       ThreadInfo threads[4];
                                                       int allocated[4];
                                                       ThreadInfo *__CS_atomic_malloc_ThreadInfo()
                                                       {
                                                         static int __csLOCAL___CS_atomic_malloc_ThreadInfo_i;
                                                         __csLOCAL___CS_atomic_malloc_ThreadInfo_i = nondet_int();
                                                         __VERIFIER_assume((0 <= __csLOCAL___CS_atomic_malloc_ThreadInfo_i) && (__csLOCAL___CS_atomic_malloc_ThreadInfo_i < 4));
                                                         __VERIFIER_assume(!allocated[__csLOCAL___CS_atomic_malloc_ThreadInfo_i]);
                                                         allocated[__csLOCAL___CS_atomic_malloc_ThreadInfo_i] = 1;
                                                         return &threads[__csLOCAL___CS_atomic_malloc_ThreadInfo_i];
                                                       }
                                                       
                                                       void __CS_atomic_free_ThreadInfo(ThreadInfo *__csLOCALPARAM___CS_atomic_free_ThreadInfo_ti)
                                                       {
                                                         static int __csLOCAL___CS_atomic_free_ThreadInfo_i;
                                                         __csLOCAL___CS_atomic_free_ThreadInfo_i = nondet_int();
                                                         __VERIFIER_assume((0 <= __csLOCAL___CS_atomic_free_ThreadInfo_i) && (__csLOCAL___CS_atomic_free_ThreadInfo_i < 4));
                                                         __VERIFIER_assume((&threads[__csLOCAL___CS_atomic_free_ThreadInfo_i]) == __csLOCALPARAM___CS_atomic_free_ThreadInfo_ti);
                                                         allocated[__csLOCAL___CS_atomic_free_ThreadInfo_i] = 0;
                                                       }
                                                       
                                                       int __cs_function_LesOP_inlined = 1;
                                                       int __cs_function_TryPerformStackOp_inlined = 1;
                                                       void __CS_atomic_FinishCollision(ThreadInfo *__csLOCALPARAM___CS_atomic_FinishCollision_p)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomic_FinishCollision_p).op == 0)
                                                         {
                                                           static int __csLOCAL___CS_atomic_FinishCollision_mypid;
                                                           __csLOCAL___CS_atomic_FinishCollision_mypid = (*__csLOCALPARAM___CS_atomic_FinishCollision_p).id;
                                                           (*__csLOCALPARAM___CS_atomic_FinishCollision_p).cell = (*location[__csLOCAL___CS_atomic_FinishCollision_mypid]).cell;
                                                           location[__csLOCAL___CS_atomic_FinishCollision_mypid] = 0;
                                                         }
                                                       
                                                       }
                                                       
                                                       int __CS_atomic_TryCollision(ThreadInfo *__csLOCALPARAM___CS_atomic_TryCollision_p, ThreadInfo *__csLOCALPARAM___CS_atomic_TryCollision_q, int __csLOCALPARAM___CS_atomic_TryCollision_him)
                                                       {
                                                         static int __csLOCAL___CS_atomic_TryCollision_mypid;
                                                         __csLOCAL___CS_atomic_TryCollision_mypid = (*__csLOCALPARAM___CS_atomic_TryCollision_p).id;
                                                         if ((*__csLOCALPARAM___CS_atomic_TryCollision_p).op == 1)
                                                         {
                                                           ;
                                                           static _Bool __csLOCAL___CS_atomic_TryCollision___if_cond_6;
                                                           __csLOCAL___CS_atomic_TryCollision___if_cond_6 = __CS_atomicti_cas(&location[__csLOCALPARAM___CS_atomic_TryCollision_him], __csLOCALPARAM___CS_atomic_TryCollision_q, __csLOCALPARAM___CS_atomic_TryCollision_p);
                                                           if (__csLOCAL___CS_atomic_TryCollision___if_cond_6)
                                                           {
                                                             return 1;
                                                           }
                                                           else
                                                           {
                                                             return 0;
                                                           }
                                                       
                                                         }
                                                       
                                                         if ((*__csLOCALPARAM___CS_atomic_TryCollision_p).op == 0)
                                                         {
                                                           ;
                                                           static _Bool __csLOCAL___CS_atomic_TryCollision___if_cond_7;
                                                           __csLOCAL___CS_atomic_TryCollision___if_cond_7 = __CS_atomicti_cas(&location[__csLOCALPARAM___CS_atomic_TryCollision_him], __csLOCALPARAM___CS_atomic_TryCollision_q, 0);
                                                           if (__csLOCAL___CS_atomic_TryCollision___if_cond_7)
                                                           {
                                                             (*__csLOCALPARAM___CS_atomic_TryCollision_p).cell = (*__csLOCALPARAM___CS_atomic_TryCollision_q).cell;
                                                             location[__csLOCAL___CS_atomic_TryCollision_mypid] = 0;
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
IF(1,0,tinstrPush0_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
tinstrPush0_0_1: IF(1,1,tinstrPush0_0_2)               __csLOCAL_Push_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPush0_0_2: IF(1,2,tinstrPush0_0_3)               (*__csLOCAL_Push_ti).id = ++unique_id;
tinstrPush0_0_3: IF(1,3,tinstrPush0_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush0_0_4: IF(1,4,tinstrPush0_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_8;
                                                       static int __retval__TryPerformStackOp_1;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush0_0_5: IF(1,5,tinstrPush0_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush0_0_6: IF(1,6,tinstrPush0_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush0_0_7: IF(1,7,tinstrPush0_0_8)               __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush0_0_8: IF(1,8,tinstrPush0_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush0_0_9: IF(1,9,tinstrPush0_0_10)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_1 = 1;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_1 = 0;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(1,10);
tinstrPush0_0_10: IF(1,10,tinstrPush0_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush0_0_11: IF(1,11,tinstrPush0_0_12)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush0_0_12: IF(1,12,tinstrPush0_0_13)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPush0_0_13: IF(1,13,tinstrPush0_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_14: IF(1,14,tinstrPush0_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_1 = 1;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				GUARD(1,15);
tinstrPush0_0_15: IF(1,15,tinstrPush0_0_16)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush0_0_16: IF(1,16,tinstrPush0_0_17)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPush0_0_17: IF(1,17,tinstrPush0_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush0_0_18: IF(1,18,tinstrPush0_0_19)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_1 = 1;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				else
                                                       				{ GUARD(1,19)
tinstrPush0_0_19: IF(1,19,tinstrPush0_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_20: IF(1,20,tinstrPush0_0_21)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_1 = 0;
                                                       goto __exit__TryPerformStackOp_1;
                                                       				}
                                                       				GUARD(1,21);
                                                       			}
                                                       			GUARD(1,21);
                                                       			__exit__TryPerformStackOp_1: GUARD(1,21)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_8 = __retval__TryPerformStackOp_1 == 0;
                                                       if (__csLOCAL_Push___if_cond_8)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush0_0_21: IF(1,21,tinstrPush0_0_22)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush0_0_22: IF(1,22,tinstrPush0_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush0_0_23: IF(1,23,tinstrPush0_0_24)            location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush0_0_24: IF(1,24,tinstrPush0_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush0_0_25: IF(1,25,tinstrPush0_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush0_0_26: IF(1,26,tinstrPush0_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPush0_0_27: IF(1,27,tinstrPush0_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush0_0_28: IF(1,28,tinstrPush0_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush0_0_29: IF(1,29,tinstrPush0_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
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
tinstrPush0_0_31: IF(1,31,tinstrPush0_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
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
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush0_0_34: IF(1,34,tinstrPush0_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPush0_0_35: IF(1,35,tinstrPush0_0_36)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush0_0_36: IF(1,36,tinstrPush0_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush0_0_37: IF(1,37,tinstrPush0_0_38)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_2 = 1;
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_2 = 0;
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(1,38);
tinstrPush0_0_38: IF(1,38,tinstrPush0_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPush0_0_39: IF(1,39,tinstrPush0_0_40)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush0_0_40: IF(1,40,tinstrPush0_0_41)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPush0_0_41: IF(1,41,tinstrPush0_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_42: IF(1,42,tinstrPush0_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_2 = 1;
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						GUARD(1,43);
tinstrPush0_0_43: IF(1,43,tinstrPush0_0_44)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush0_0_44: IF(1,44,tinstrPush0_0_45)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPush0_0_45: IF(1,45,tinstrPush0_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush0_0_46: IF(1,46,tinstrPush0_0_47)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_2 = 1;
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,47)
tinstrPush0_0_47: IF(1,47,tinstrPush0_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush0_0_48: IF(1,48,tinstrPush0_0_49)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_2 = 0;
                                                       goto __exit__TryPerformStackOp_2;
                                                       						}
                                                       						GUARD(1,49);
                                                       					}
                                                       					GUARD(1,49);
                                                       					__exit__TryPerformStackOp_2: GUARD(1,49)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_2 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_1;
                                                       				}
                                                       				;
tinstrPush0_0_49: IF(1,49,tinstrPush0_0_50)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_1: GUARD(1,50)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(1,50);
                                                       		__exit__Push_1: GUARD(1,50)
                                                       ;
                                                       	};
                                                       0;
tinstrPush0_0_50: IF(1,50,tinstrPush0_0_51)            __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush0;
                                                       	__exit_instrPush0: GUARD(1,51)
                                                       ;
tinstrPush0_0_51:                                      STOP_NONVOID(51);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush1_0(void *__csLOCALPARAM_instrPush1_unused)
                                                       {
IF(2,0,tinstrPush1_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
tinstrPush1_0_1: IF(2,1,tinstrPush1_0_2)               __csLOCAL_Push_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPush1_0_2: IF(2,2,tinstrPush1_0_3)               (*__csLOCAL_Push_ti).id = ++unique_id;
tinstrPush1_0_3: IF(2,3,tinstrPush1_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush1_0_4: IF(2,4,tinstrPush1_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_8;
                                                       static int __retval__TryPerformStackOp_3;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush1_0_5: IF(2,5,tinstrPush1_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush1_0_6: IF(2,6,tinstrPush1_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush1_0_7: IF(2,7,tinstrPush1_0_8)               __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush1_0_8: IF(2,8,tinstrPush1_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush1_0_9: IF(2,9,tinstrPush1_0_10)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_3 = 1;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_3 = 0;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(2,10);
tinstrPush1_0_10: IF(2,10,tinstrPush1_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush1_0_11: IF(2,11,tinstrPush1_0_12)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush1_0_12: IF(2,12,tinstrPush1_0_13)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPush1_0_13: IF(2,13,tinstrPush1_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_14: IF(2,14,tinstrPush1_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_3 = 1;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				GUARD(2,15);
tinstrPush1_0_15: IF(2,15,tinstrPush1_0_16)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush1_0_16: IF(2,16,tinstrPush1_0_17)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPush1_0_17: IF(2,17,tinstrPush1_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush1_0_18: IF(2,18,tinstrPush1_0_19)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_3 = 1;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				else
                                                       				{ GUARD(2,19)
tinstrPush1_0_19: IF(2,19,tinstrPush1_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_20: IF(2,20,tinstrPush1_0_21)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_3 = 0;
                                                       goto __exit__TryPerformStackOp_3;
                                                       				}
                                                       				GUARD(2,21);
                                                       			}
                                                       			GUARD(2,21);
                                                       			__exit__TryPerformStackOp_3: GUARD(2,21)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_8 = __retval__TryPerformStackOp_3 == 0;
                                                       if (__csLOCAL_Push___if_cond_8)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush1_0_21: IF(2,21,tinstrPush1_0_22)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush1_0_22: IF(2,22,tinstrPush1_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush1_0_23: IF(2,23,tinstrPush1_0_24)            location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush1_0_24: IF(2,24,tinstrPush1_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush1_0_25: IF(2,25,tinstrPush1_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush1_0_26: IF(2,26,tinstrPush1_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPush1_0_27: IF(2,27,tinstrPush1_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush1_0_28: IF(2,28,tinstrPush1_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush1_0_29: IF(2,29,tinstrPush1_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
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
tinstrPush1_0_31: IF(2,31,tinstrPush1_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
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
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush1_0_34: IF(2,34,tinstrPush1_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPush1_0_35: IF(2,35,tinstrPush1_0_36)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush1_0_36: IF(2,36,tinstrPush1_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush1_0_37: IF(2,37,tinstrPush1_0_38)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_4 = 1;
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_4 = 0;
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(2,38);
tinstrPush1_0_38: IF(2,38,tinstrPush1_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPush1_0_39: IF(2,39,tinstrPush1_0_40)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush1_0_40: IF(2,40,tinstrPush1_0_41)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPush1_0_41: IF(2,41,tinstrPush1_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_42: IF(2,42,tinstrPush1_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_4 = 1;
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						GUARD(2,43);
tinstrPush1_0_43: IF(2,43,tinstrPush1_0_44)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush1_0_44: IF(2,44,tinstrPush1_0_45)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPush1_0_45: IF(2,45,tinstrPush1_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush1_0_46: IF(2,46,tinstrPush1_0_47)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_4 = 1;
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,47)
tinstrPush1_0_47: IF(2,47,tinstrPush1_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush1_0_48: IF(2,48,tinstrPush1_0_49)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_4 = 0;
                                                       goto __exit__TryPerformStackOp_4;
                                                       						}
                                                       						GUARD(2,49);
                                                       					}
                                                       					GUARD(2,49);
                                                       					__exit__TryPerformStackOp_4: GUARD(2,49)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_4 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_2;
                                                       				}
                                                       				;
tinstrPush1_0_49: IF(2,49,tinstrPush1_0_50)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_2: GUARD(2,50)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(2,50);
                                                       		__exit__Push_2: GUARD(2,50)
                                                       ;
                                                       	};
                                                       0;
tinstrPush1_0_50: IF(2,50,tinstrPush1_0_51)            __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush1;
                                                       	__exit_instrPush1: GUARD(2,51)
                                                       ;
tinstrPush1_0_51:                                      STOP_NONVOID(51);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush2_0(void *__csLOCALPARAM_instrPush2_unused)
                                                       {
IF(3,0,tinstrPush2_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_x;
                                                       __csLOCALPARAM_Push_x = 1;
                                                       static ThreadInfo *__csLOCAL_Push_ti;
tinstrPush2_0_1: IF(3,1,tinstrPush2_0_2)               __csLOCAL_Push_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPush2_0_2: IF(3,2,tinstrPush2_0_3)               (*__csLOCAL_Push_ti).id = ++unique_id;
tinstrPush2_0_3: IF(3,3,tinstrPush2_0_4)               (*__csLOCAL_Push_ti).op = 1;
tinstrPush2_0_4: IF(3,4,tinstrPush2_0_5)               (*__csLOCAL_Push_ti).cell.pdata = __csLOCALPARAM_Push_x;
                                                       ;
                                                       static _Bool __csLOCAL_Push___if_cond_8;
                                                       static int __retval__TryPerformStackOp_5;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPush2_0_5: IF(3,5,tinstrPush2_0_6)               __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Push_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush2_0_6: IF(3,6,tinstrPush2_0_7)               if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPush2_0_7: IF(3,7,tinstrPush2_0_8)               __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush2_0_8: IF(3,8,tinstrPush2_0_9)               (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush2_0_9: IF(3,9,tinstrPush2_0_10)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_5 = 1;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_5 = 0;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(3,10);
tinstrPush2_0_10: IF(3,10,tinstrPush2_0_11)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPush2_0_11: IF(3,11,tinstrPush2_0_12)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush2_0_12: IF(3,12,tinstrPush2_0_13)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPush2_0_13: IF(3,13,tinstrPush2_0_14)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_14: IF(3,14,tinstrPush2_0_15)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_5 = 1;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				GUARD(3,15);
tinstrPush2_0_15: IF(3,15,tinstrPush2_0_16)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush2_0_16: IF(3,16,tinstrPush2_0_17)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPush2_0_17: IF(3,17,tinstrPush2_0_18)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush2_0_18: IF(3,18,tinstrPush2_0_19)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_5 = 1;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				else
                                                       				{ GUARD(3,19)
tinstrPush2_0_19: IF(3,19,tinstrPush2_0_20)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_20: IF(3,20,tinstrPush2_0_21)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_5 = 0;
                                                       goto __exit__TryPerformStackOp_5;
                                                       				}
                                                       				GUARD(3,21);
                                                       			}
                                                       			GUARD(3,21);
                                                       			__exit__TryPerformStackOp_5: GUARD(3,21)
                                                       ;
                                                       		};
                                                       __csLOCAL_Push___if_cond_8 = __retval__TryPerformStackOp_5 == 0;
                                                       if (__csLOCAL_Push___if_cond_8)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPush2_0_21: IF(3,21,tinstrPush2_0_22)            __csLOCALPARAM_LesOP_p = __csLOCAL_Push_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPush2_0_22: IF(3,22,tinstrPush2_0_23)            __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPush2_0_23: IF(3,23,tinstrPush2_0_24)            location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPush2_0_24: IF(3,24,tinstrPush2_0_25)            __csLOCAL_LesOP_him = collision;
tinstrPush2_0_25: IF(3,25,tinstrPush2_0_26)            __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPush2_0_26: IF(3,26,tinstrPush2_0_27)            __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPush2_0_27: IF(3,27,tinstrPush2_0_28)            if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPush2_0_28: IF(3,28,tinstrPush2_0_29)            __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPush2_0_29: IF(3,29,tinstrPush2_0_30)            __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
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
tinstrPush2_0_31: IF(3,31,tinstrPush2_0_32)            __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
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
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPush2_0_34: IF(3,34,tinstrPush2_0_35)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPush2_0_35: IF(3,35,tinstrPush2_0_36)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush2_0_36: IF(3,36,tinstrPush2_0_37)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPush2_0_37: IF(3,37,tinstrPush2_0_38)            __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_6 = 1;
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_6 = 0;
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(3,38);
tinstrPush2_0_38: IF(3,38,tinstrPush2_0_39)            if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPush2_0_39: IF(3,39,tinstrPush2_0_40)            __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPush2_0_40: IF(3,40,tinstrPush2_0_41)            if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPush2_0_41: IF(3,41,tinstrPush2_0_42)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_42: IF(3,42,tinstrPush2_0_43)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_6 = 1;
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						GUARD(3,43);
tinstrPush2_0_43: IF(3,43,tinstrPush2_0_44)            __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPush2_0_44: IF(3,44,tinstrPush2_0_45)            __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPush2_0_45: IF(3,45,tinstrPush2_0_46)            (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPush2_0_46: IF(3,46,tinstrPush2_0_47)            //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_6 = 1;
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,47)
tinstrPush2_0_47: IF(3,47,tinstrPush2_0_48)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPush2_0_48: IF(3,48,tinstrPush2_0_49)            (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_6 = 0;
                                                       goto __exit__TryPerformStackOp_6;
                                                       						}
                                                       						GUARD(3,49);
                                                       					}
                                                       					GUARD(3,49);
                                                       					__exit__TryPerformStackOp_6: GUARD(3,49)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_6 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_3;
                                                       				}
                                                       				;
tinstrPush2_0_49: IF(3,49,tinstrPush2_0_50)            __VERIFIER_assume(0);
                                                       				__exit__LesOP_3: GUARD(3,50)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(3,50);
                                                       		__exit__Push_3: GUARD(3,50)
                                                       ;
                                                       	};
                                                       0;
tinstrPush2_0_50: IF(3,50,tinstrPush2_0_51)            __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush2;
                                                       	__exit_instrPush2: GUARD(3,51)
                                                       ;
tinstrPush2_0_51:                                      STOP_NONVOID(51);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop3_0(void *__csLOCALPARAM_instrPop3_unused)
                                                       {
IF(4,0,tinstrPop3_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop3_localPop_ret;
                                                       static int __retval__Pop_1;
                                                       	{
                                                       static ThreadInfo *__csLOCAL_Pop_ti;
tinstrPop3_0_1: IF(4,1,tinstrPop3_0_2)                 __csLOCAL_Pop_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPop3_0_2: IF(4,2,tinstrPop3_0_3)                 (*__csLOCAL_Pop_ti).id = ++unique_id;
tinstrPop3_0_3: IF(4,3,tinstrPop3_0_4)                 (*__csLOCAL_Pop_ti).op = 0;
                                                       ;
                                                       static _Bool __csLOCAL_Pop___if_cond_9;
                                                       static int __retval__TryPerformStackOp_7;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop3_0_4: IF(4,4,tinstrPop3_0_5)                 __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Pop_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop3_0_5: IF(4,5,tinstrPop3_0_6)                 if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPop3_0_6: IF(4,6,tinstrPop3_0_7)                 __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop3_0_7: IF(4,7,tinstrPop3_0_8)                 (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop3_0_8: IF(4,8,tinstrPop3_0_9)                 __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_7 = 1;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_7 = 0;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(4,9);
tinstrPop3_0_9: IF(4,9,tinstrPop3_0_10)                if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPop3_0_10: IF(4,10,tinstrPop3_0_11)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop3_0_11: IF(4,11,tinstrPop3_0_12)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPop3_0_12: IF(4,12,tinstrPop3_0_13)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_13: IF(4,13,tinstrPop3_0_14)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_7 = 1;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				GUARD(4,14);
tinstrPop3_0_14: IF(4,14,tinstrPop3_0_15)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop3_0_15: IF(4,15,tinstrPop3_0_16)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPop3_0_16: IF(4,16,tinstrPop3_0_17)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop3_0_17: IF(4,17,tinstrPop3_0_18)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_7 = 1;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				else
                                                       				{ GUARD(4,18)
tinstrPop3_0_18: IF(4,18,tinstrPop3_0_19)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_19: IF(4,19,tinstrPop3_0_20)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_7 = 0;
                                                       goto __exit__TryPerformStackOp_7;
                                                       				}
                                                       				GUARD(4,20);
                                                       			}
                                                       			GUARD(4,20);
                                                       			__exit__TryPerformStackOp_7: GUARD(4,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Pop___if_cond_9 = __retval__TryPerformStackOp_7 == 0;
                                                       if (__csLOCAL_Pop___if_cond_9)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPop3_0_20: IF(4,20,tinstrPop3_0_21)              __csLOCALPARAM_LesOP_p = __csLOCAL_Pop_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPop3_0_21: IF(4,21,tinstrPop3_0_22)              __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPop3_0_22: IF(4,22,tinstrPop3_0_23)              location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPop3_0_23: IF(4,23,tinstrPop3_0_24)              __csLOCAL_LesOP_him = collision;
tinstrPop3_0_24: IF(4,24,tinstrPop3_0_25)              __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPop3_0_25: IF(4,25,tinstrPop3_0_26)              __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPop3_0_26: IF(4,26,tinstrPop3_0_27)              if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPop3_0_27: IF(4,27,tinstrPop3_0_28)              __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPop3_0_28: IF(4,28,tinstrPop3_0_29)              __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
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
tinstrPop3_0_30: IF(4,30,tinstrPop3_0_31)              __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
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
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop3_0_33: IF(4,33,tinstrPop3_0_34)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPop3_0_34: IF(4,34,tinstrPop3_0_35)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop3_0_35: IF(4,35,tinstrPop3_0_36)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop3_0_36: IF(4,36,tinstrPop3_0_37)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_8 = 1;
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_8 = 0;
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(4,37);
tinstrPop3_0_37: IF(4,37,tinstrPop3_0_38)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPop3_0_38: IF(4,38,tinstrPop3_0_39)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop3_0_39: IF(4,39,tinstrPop3_0_40)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPop3_0_40: IF(4,40,tinstrPop3_0_41)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_41: IF(4,41,tinstrPop3_0_42)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_8 = 1;
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						GUARD(4,42);
tinstrPop3_0_42: IF(4,42,tinstrPop3_0_43)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop3_0_43: IF(4,43,tinstrPop3_0_44)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPop3_0_44: IF(4,44,tinstrPop3_0_45)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop3_0_45: IF(4,45,tinstrPop3_0_46)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_8 = 1;
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						else
                                                       						{ GUARD(4,46)
tinstrPop3_0_46: IF(4,46,tinstrPop3_0_47)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop3_0_47: IF(4,47,tinstrPop3_0_48)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_8 = 0;
                                                       goto __exit__TryPerformStackOp_8;
                                                       						}
                                                       						GUARD(4,48);
                                                       					}
                                                       					GUARD(4,48);
                                                       					__exit__TryPerformStackOp_8: GUARD(4,48)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_8 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_4;
                                                       				}
                                                       				;
tinstrPop3_0_48: IF(4,48,tinstrPop3_0_49)              __VERIFIER_assume(0);
                                                       				__exit__LesOP_4: GUARD(4,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(4,49);
                                                       static int __csLOCAL_Pop_v;
tinstrPop3_0_49: IF(4,49,tinstrPop3_0_50)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_ti).cell.pdata;
tinstrPop3_0_50: IF(4,50,tinstrPop3_0_51)              __CS_atomic_free_ThreadInfo(__csLOCAL_Pop_ti);
                                                       __retval__Pop_1 = __csLOCAL_Pop_v;
                                                       goto __exit__Pop_1;
                                                       		__exit__Pop_1: GUARD(4,51)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop3_localPop_ret = __retval__Pop_1;
tinstrPop3_0_51: IF(4,51,tinstrPop3_0_52)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop3_localPop_ret);
                                                       goto __exit_instrPop3;
                                                       	__exit_instrPop3: GUARD(4,52)
                                                       ;
tinstrPop3_0_52:                                       STOP_NONVOID(52);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop4_0(void *__csLOCALPARAM_instrPop4_unused)
                                                       {
IF(5,0,tinstrPop4_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop4_localPop_ret;
                                                       static int __retval__Pop_2;
                                                       	{
                                                       static ThreadInfo *__csLOCAL_Pop_ti;
tinstrPop4_0_1: IF(5,1,tinstrPop4_0_2)                 __csLOCAL_Pop_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPop4_0_2: IF(5,2,tinstrPop4_0_3)                 (*__csLOCAL_Pop_ti).id = ++unique_id;
tinstrPop4_0_3: IF(5,3,tinstrPop4_0_4)                 (*__csLOCAL_Pop_ti).op = 0;
                                                       ;
                                                       static _Bool __csLOCAL_Pop___if_cond_9;
                                                       static int __retval__TryPerformStackOp_9;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop4_0_4: IF(5,4,tinstrPop4_0_5)                 __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Pop_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop4_0_5: IF(5,5,tinstrPop4_0_6)                 if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPop4_0_6: IF(5,6,tinstrPop4_0_7)                 __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop4_0_7: IF(5,7,tinstrPop4_0_8)                 (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop4_0_8: IF(5,8,tinstrPop4_0_9)                 __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_9 = 1;
                                                       goto __exit__TryPerformStackOp_9;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_9 = 0;
                                                       goto __exit__TryPerformStackOp_9;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(5,9);
tinstrPop4_0_9: IF(5,9,tinstrPop4_0_10)                if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPop4_0_10: IF(5,10,tinstrPop4_0_11)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop4_0_11: IF(5,11,tinstrPop4_0_12)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPop4_0_12: IF(5,12,tinstrPop4_0_13)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop4_0_13: IF(5,13,tinstrPop4_0_14)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_9 = 1;
                                                       goto __exit__TryPerformStackOp_9;
                                                       				}
                                                       				GUARD(5,14);
tinstrPop4_0_14: IF(5,14,tinstrPop4_0_15)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop4_0_15: IF(5,15,tinstrPop4_0_16)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPop4_0_16: IF(5,16,tinstrPop4_0_17)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop4_0_17: IF(5,17,tinstrPop4_0_18)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_9 = 1;
                                                       goto __exit__TryPerformStackOp_9;
                                                       				}
                                                       				else
                                                       				{ GUARD(5,18)
tinstrPop4_0_18: IF(5,18,tinstrPop4_0_19)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop4_0_19: IF(5,19,tinstrPop4_0_20)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_9 = 0;
                                                       goto __exit__TryPerformStackOp_9;
                                                       				}
                                                       				GUARD(5,20);
                                                       			}
                                                       			GUARD(5,20);
                                                       			__exit__TryPerformStackOp_9: GUARD(5,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Pop___if_cond_9 = __retval__TryPerformStackOp_9 == 0;
                                                       if (__csLOCAL_Pop___if_cond_9)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPop4_0_20: IF(5,20,tinstrPop4_0_21)              __csLOCALPARAM_LesOP_p = __csLOCAL_Pop_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPop4_0_21: IF(5,21,tinstrPop4_0_22)              __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPop4_0_22: IF(5,22,tinstrPop4_0_23)              location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPop4_0_23: IF(5,23,tinstrPop4_0_24)              __csLOCAL_LesOP_him = collision;
tinstrPop4_0_24: IF(5,24,tinstrPop4_0_25)              __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPop4_0_25: IF(5,25,tinstrPop4_0_26)              __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPop4_0_26: IF(5,26,tinstrPop4_0_27)              if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPop4_0_27: IF(5,27,tinstrPop4_0_28)              __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPop4_0_28: IF(5,28,tinstrPop4_0_29)              __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       							{
                                                       goto __exit__LesOP_5;
                                                       							}
                                                       							else
                                                       							{ 
                                                       goto stack;
                                                       							}
                                                       							;
                                                       						}
                                                       						else
                                                       						{ GUARD(5,29)
tinstrPop4_0_29: IF(5,29,tinstrPop4_0_30)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_5;
                                                       						}
                                                       						GUARD(5,30);
                                                       					}
                                                       					GUARD(5,30);
                                                       				}
                                                       				GUARD(5,30);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPop4_0_30: IF(5,30,tinstrPop4_0_31)              __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       				{
tinstrPop4_0_31: IF(5,31,tinstrPop4_0_32)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_5;
                                                       				}
                                                       				GUARD(5,32);
                                                       				stack: GUARD(5,32)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_10;
                                                       				{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop4_0_32: IF(5,32,tinstrPop4_0_33)              __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop4_0_33: IF(5,33,tinstrPop4_0_34)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPop4_0_34: IF(5,34,tinstrPop4_0_35)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop4_0_35: IF(5,35,tinstrPop4_0_36)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop4_0_36: IF(5,36,tinstrPop4_0_37)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_10 = 1;
                                                       goto __exit__TryPerformStackOp_10;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_10 = 0;
                                                       goto __exit__TryPerformStackOp_10;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(5,37);
tinstrPop4_0_37: IF(5,37,tinstrPop4_0_38)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPop4_0_38: IF(5,38,tinstrPop4_0_39)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop4_0_39: IF(5,39,tinstrPop4_0_40)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPop4_0_40: IF(5,40,tinstrPop4_0_41)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop4_0_41: IF(5,41,tinstrPop4_0_42)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_10 = 1;
                                                       goto __exit__TryPerformStackOp_10;
                                                       						}
                                                       						GUARD(5,42);
tinstrPop4_0_42: IF(5,42,tinstrPop4_0_43)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop4_0_43: IF(5,43,tinstrPop4_0_44)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPop4_0_44: IF(5,44,tinstrPop4_0_45)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop4_0_45: IF(5,45,tinstrPop4_0_46)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_10 = 1;
                                                       goto __exit__TryPerformStackOp_10;
                                                       						}
                                                       						else
                                                       						{ GUARD(5,46)
tinstrPop4_0_46: IF(5,46,tinstrPop4_0_47)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop4_0_47: IF(5,47,tinstrPop4_0_48)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_10 = 0;
                                                       goto __exit__TryPerformStackOp_10;
                                                       						}
                                                       						GUARD(5,48);
                                                       					}
                                                       					GUARD(5,48);
                                                       					__exit__TryPerformStackOp_10: GUARD(5,48)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_10 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_5;
                                                       				}
                                                       				;
tinstrPop4_0_48: IF(5,48,tinstrPop4_0_49)              __VERIFIER_assume(0);
                                                       				__exit__LesOP_5: GUARD(5,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(5,49);
                                                       static int __csLOCAL_Pop_v;
tinstrPop4_0_49: IF(5,49,tinstrPop4_0_50)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_ti).cell.pdata;
tinstrPop4_0_50: IF(5,50,tinstrPop4_0_51)              __CS_atomic_free_ThreadInfo(__csLOCAL_Pop_ti);
                                                       __retval__Pop_2 = __csLOCAL_Pop_v;
                                                       goto __exit__Pop_2;
                                                       		__exit__Pop_2: GUARD(5,51)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop4_localPop_ret = __retval__Pop_2;
tinstrPop4_0_51: IF(5,51,tinstrPop4_0_52)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop4_localPop_ret);
                                                       goto __exit_instrPop4;
                                                       	__exit_instrPop4: GUARD(5,52)
                                                       ;
tinstrPop4_0_52:                                       STOP_NONVOID(52);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop5_0(void *__csLOCALPARAM_instrPop5_unused)
                                                       {
IF(6,0,tinstrPop5_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop5_localPop_ret;
                                                       static int __retval__Pop_3;
                                                       	{
                                                       static ThreadInfo *__csLOCAL_Pop_ti;
tinstrPop5_0_1: IF(6,1,tinstrPop5_0_2)                 __csLOCAL_Pop_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPop5_0_2: IF(6,2,tinstrPop5_0_3)                 (*__csLOCAL_Pop_ti).id = ++unique_id;
tinstrPop5_0_3: IF(6,3,tinstrPop5_0_4)                 (*__csLOCAL_Pop_ti).op = 0;
                                                       ;
                                                       static _Bool __csLOCAL_Pop___if_cond_9;
                                                       static int __retval__TryPerformStackOp_11;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop5_0_4: IF(6,4,tinstrPop5_0_5)                 __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Pop_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop5_0_5: IF(6,5,tinstrPop5_0_6)                 if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPop5_0_6: IF(6,6,tinstrPop5_0_7)                 __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop5_0_7: IF(6,7,tinstrPop5_0_8)                 (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop5_0_8: IF(6,8,tinstrPop5_0_9)                 __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_11 = 1;
                                                       goto __exit__TryPerformStackOp_11;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_11 = 0;
                                                       goto __exit__TryPerformStackOp_11;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(6,9);
tinstrPop5_0_9: IF(6,9,tinstrPop5_0_10)                if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPop5_0_10: IF(6,10,tinstrPop5_0_11)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop5_0_11: IF(6,11,tinstrPop5_0_12)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPop5_0_12: IF(6,12,tinstrPop5_0_13)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop5_0_13: IF(6,13,tinstrPop5_0_14)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_11 = 1;
                                                       goto __exit__TryPerformStackOp_11;
                                                       				}
                                                       				GUARD(6,14);
tinstrPop5_0_14: IF(6,14,tinstrPop5_0_15)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop5_0_15: IF(6,15,tinstrPop5_0_16)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPop5_0_16: IF(6,16,tinstrPop5_0_17)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop5_0_17: IF(6,17,tinstrPop5_0_18)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_11 = 1;
                                                       goto __exit__TryPerformStackOp_11;
                                                       				}
                                                       				else
                                                       				{ GUARD(6,18)
tinstrPop5_0_18: IF(6,18,tinstrPop5_0_19)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop5_0_19: IF(6,19,tinstrPop5_0_20)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_11 = 0;
                                                       goto __exit__TryPerformStackOp_11;
                                                       				}
                                                       				GUARD(6,20);
                                                       			}
                                                       			GUARD(6,20);
                                                       			__exit__TryPerformStackOp_11: GUARD(6,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Pop___if_cond_9 = __retval__TryPerformStackOp_11 == 0;
                                                       if (__csLOCAL_Pop___if_cond_9)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPop5_0_20: IF(6,20,tinstrPop5_0_21)              __csLOCALPARAM_LesOP_p = __csLOCAL_Pop_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPop5_0_21: IF(6,21,tinstrPop5_0_22)              __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPop5_0_22: IF(6,22,tinstrPop5_0_23)              location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPop5_0_23: IF(6,23,tinstrPop5_0_24)              __csLOCAL_LesOP_him = collision;
tinstrPop5_0_24: IF(6,24,tinstrPop5_0_25)              __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPop5_0_25: IF(6,25,tinstrPop5_0_26)              __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPop5_0_26: IF(6,26,tinstrPop5_0_27)              if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPop5_0_27: IF(6,27,tinstrPop5_0_28)              __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPop5_0_28: IF(6,28,tinstrPop5_0_29)              __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       							{
                                                       goto __exit__LesOP_6;
                                                       							}
                                                       							else
                                                       							{ 
                                                       goto stack;
                                                       							}
                                                       							;
                                                       						}
                                                       						else
                                                       						{ GUARD(6,29)
tinstrPop5_0_29: IF(6,29,tinstrPop5_0_30)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_6;
                                                       						}
                                                       						GUARD(6,30);
                                                       					}
                                                       					GUARD(6,30);
                                                       				}
                                                       				GUARD(6,30);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPop5_0_30: IF(6,30,tinstrPop5_0_31)              __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       				{
tinstrPop5_0_31: IF(6,31,tinstrPop5_0_32)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_6;
                                                       				}
                                                       				GUARD(6,32);
                                                       				stack: GUARD(6,32)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_12;
                                                       				{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop5_0_32: IF(6,32,tinstrPop5_0_33)              __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop5_0_33: IF(6,33,tinstrPop5_0_34)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPop5_0_34: IF(6,34,tinstrPop5_0_35)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop5_0_35: IF(6,35,tinstrPop5_0_36)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop5_0_36: IF(6,36,tinstrPop5_0_37)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_12 = 1;
                                                       goto __exit__TryPerformStackOp_12;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_12 = 0;
                                                       goto __exit__TryPerformStackOp_12;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(6,37);
tinstrPop5_0_37: IF(6,37,tinstrPop5_0_38)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPop5_0_38: IF(6,38,tinstrPop5_0_39)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop5_0_39: IF(6,39,tinstrPop5_0_40)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPop5_0_40: IF(6,40,tinstrPop5_0_41)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop5_0_41: IF(6,41,tinstrPop5_0_42)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_12 = 1;
                                                       goto __exit__TryPerformStackOp_12;
                                                       						}
                                                       						GUARD(6,42);
tinstrPop5_0_42: IF(6,42,tinstrPop5_0_43)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop5_0_43: IF(6,43,tinstrPop5_0_44)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPop5_0_44: IF(6,44,tinstrPop5_0_45)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop5_0_45: IF(6,45,tinstrPop5_0_46)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_12 = 1;
                                                       goto __exit__TryPerformStackOp_12;
                                                       						}
                                                       						else
                                                       						{ GUARD(6,46)
tinstrPop5_0_46: IF(6,46,tinstrPop5_0_47)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop5_0_47: IF(6,47,tinstrPop5_0_48)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_12 = 0;
                                                       goto __exit__TryPerformStackOp_12;
                                                       						}
                                                       						GUARD(6,48);
                                                       					}
                                                       					GUARD(6,48);
                                                       					__exit__TryPerformStackOp_12: GUARD(6,48)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_12 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_6;
                                                       				}
                                                       				;
tinstrPop5_0_48: IF(6,48,tinstrPop5_0_49)              __VERIFIER_assume(0);
                                                       				__exit__LesOP_6: GUARD(6,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(6,49);
                                                       static int __csLOCAL_Pop_v;
tinstrPop5_0_49: IF(6,49,tinstrPop5_0_50)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_ti).cell.pdata;
tinstrPop5_0_50: IF(6,50,tinstrPop5_0_51)              __CS_atomic_free_ThreadInfo(__csLOCAL_Pop_ti);
                                                       __retval__Pop_3 = __csLOCAL_Pop_v;
                                                       goto __exit__Pop_3;
                                                       		__exit__Pop_3: GUARD(6,51)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop5_localPop_ret = __retval__Pop_3;
tinstrPop5_0_51: IF(6,51,tinstrPop5_0_52)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop5_localPop_ret);
                                                       goto __exit_instrPop5;
                                                       	__exit_instrPop5: GUARD(6,52)
                                                       ;
tinstrPop5_0_52:                                       STOP_NONVOID(52);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop6_0(void *__csLOCALPARAM_instrPop6_unused)
                                                       {
IF(7,0,tinstrPop6_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop6_localPop_ret;
                                                       static int __retval__Pop_4;
                                                       	{
                                                       static ThreadInfo *__csLOCAL_Pop_ti;
tinstrPop6_0_1: IF(7,1,tinstrPop6_0_2)                 __csLOCAL_Pop_ti = __CS_atomic_malloc_ThreadInfo();
tinstrPop6_0_2: IF(7,2,tinstrPop6_0_3)                 (*__csLOCAL_Pop_ti).id = ++unique_id;
tinstrPop6_0_3: IF(7,3,tinstrPop6_0_4)                 (*__csLOCAL_Pop_ti).op = 0;
                                                       ;
                                                       static _Bool __csLOCAL_Pop___if_cond_9;
                                                       static int __retval__TryPerformStackOp_13;
                                                       		{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop6_0_4: IF(7,4,tinstrPop6_0_5)                 __csLOCALPARAM_TryPerformStackOp_p = __csLOCAL_Pop_ti;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop6_0_5: IF(7,5,tinstrPop6_0_6)                 if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       			{
tinstrPop6_0_6: IF(7,6,tinstrPop6_0_7)                 __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop6_0_7: IF(7,7,tinstrPop6_0_8)                 (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop6_0_8: IF(7,8,tinstrPop6_0_9)                 __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       				{
                                                       __retval__TryPerformStackOp_13 = 1;
                                                       goto __exit__TryPerformStackOp_13;
                                                       				}
                                                       				else
                                                       				{ 
                                                       __retval__TryPerformStackOp_13 = 0;
                                                       goto __exit__TryPerformStackOp_13;
                                                       				}
                                                       				;
                                                       			}
                                                       			GUARD(7,9);
tinstrPop6_0_9: IF(7,9,tinstrPop6_0_10)                if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       			{
tinstrPop6_0_10: IF(7,10,tinstrPop6_0_11)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop6_0_11: IF(7,11,tinstrPop6_0_12)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       				{
tinstrPop6_0_12: IF(7,12,tinstrPop6_0_13)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop6_0_13: IF(7,13,tinstrPop6_0_14)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_13 = 1;
                                                       goto __exit__TryPerformStackOp_13;
                                                       				}
                                                       				GUARD(7,14);
tinstrPop6_0_14: IF(7,14,tinstrPop6_0_15)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop6_0_15: IF(7,15,tinstrPop6_0_16)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       				{
tinstrPop6_0_16: IF(7,16,tinstrPop6_0_17)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop6_0_17: IF(7,17,tinstrPop6_0_18)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_13 = 1;
                                                       goto __exit__TryPerformStackOp_13;
                                                       				}
                                                       				else
                                                       				{ GUARD(7,18)
tinstrPop6_0_18: IF(7,18,tinstrPop6_0_19)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop6_0_19: IF(7,19,tinstrPop6_0_20)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_13 = 0;
                                                       goto __exit__TryPerformStackOp_13;
                                                       				}
                                                       				GUARD(7,20);
                                                       			}
                                                       			GUARD(7,20);
                                                       			__exit__TryPerformStackOp_13: GUARD(7,20)
                                                       ;
                                                       		};
                                                       __csLOCAL_Pop___if_cond_9 = __retval__TryPerformStackOp_13 == 0;
                                                       if (__csLOCAL_Pop___if_cond_9)
                                                       		{
                                                       			{
                                                       static ThreadInfo *__csLOCALPARAM_LesOP_p;
tinstrPop6_0_20: IF(7,20,tinstrPop6_0_21)              __csLOCALPARAM_LesOP_p = __csLOCAL_Pop_ti;
                                                       static int __csLOCAL_LesOP_mypid;
tinstrPop6_0_21: IF(7,21,tinstrPop6_0_22)              __csLOCAL_LesOP_mypid = (*__csLOCALPARAM_LesOP_p).id;
tinstrPop6_0_22: IF(7,22,tinstrPop6_0_23)              location[__csLOCAL_LesOP_mypid] = __csLOCALPARAM_LesOP_p;
                                                       static int __csLOCAL_LesOP_him;
tinstrPop6_0_23: IF(7,23,tinstrPop6_0_24)              __csLOCAL_LesOP_him = collision;
tinstrPop6_0_24: IF(7,24,tinstrPop6_0_25)              __VERIFIER_assume(__CS_atomicint_cas(&collision, __csLOCAL_LesOP_him, __csLOCAL_LesOP_mypid));
                                                       if (__csLOCAL_LesOP_him > 0)
                                                       				{
                                                       static ThreadInfo *__csLOCAL_LesOP_q;
tinstrPop6_0_25: IF(7,25,tinstrPop6_0_26)              __csLOCAL_LesOP_q = location[__csLOCAL_LesOP_him];
tinstrPop6_0_26: IF(7,26,tinstrPop6_0_27)              if (((__csLOCAL_LesOP_q != 0) && ((*__csLOCAL_LesOP_q).id == __csLOCAL_LesOP_him)) && ((*__csLOCAL_LesOP_q).op != (*__csLOCALPARAM_LesOP_p).op))
                                                       					{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_0;
tinstrPop6_0_27: IF(7,27,tinstrPop6_0_28)              __csLOCAL_LesOP___if_cond_0 = __CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_0)
                                                       						{
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_1;
tinstrPop6_0_28: IF(7,28,tinstrPop6_0_29)              __csLOCAL_LesOP___if_cond_1 = __CS_atomic_TryCollision(__csLOCALPARAM_LesOP_p, __csLOCAL_LesOP_q, __csLOCAL_LesOP_him) == 1;
                                                       if (__csLOCAL_LesOP___if_cond_1)
                                                       							{
                                                       goto __exit__LesOP_7;
                                                       							}
                                                       							else
                                                       							{ 
                                                       goto stack;
                                                       							}
                                                       							;
                                                       						}
                                                       						else
                                                       						{ GUARD(7,29)
tinstrPop6_0_29: IF(7,29,tinstrPop6_0_30)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_7;
                                                       						}
                                                       						GUARD(7,30);
                                                       					}
                                                       					GUARD(7,30);
                                                       				}
                                                       				GUARD(7,30);
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_2;
tinstrPop6_0_30: IF(7,30,tinstrPop6_0_31)              __csLOCAL_LesOP___if_cond_2 = !__CS_atomicti_cas(&location[__csLOCAL_LesOP_mypid], __csLOCALPARAM_LesOP_p, 0);
                                                       if (__csLOCAL_LesOP___if_cond_2)
                                                       				{
tinstrPop6_0_31: IF(7,31,tinstrPop6_0_32)              __CS_atomic_FinishCollision(__csLOCALPARAM_LesOP_p);
                                                       goto __exit__LesOP_7;
                                                       				}
                                                       				GUARD(7,32);
                                                       				stack: GUARD(7,32)
                                                       ;
                                                       static _Bool __csLOCAL_LesOP___if_cond_3;
                                                       static int __retval__TryPerformStackOp_14;
                                                       				{
                                                       static ThreadInfo *__csLOCALPARAM_TryPerformStackOp_p;
tinstrPop6_0_32: IF(7,32,tinstrPop6_0_33)              __csLOCALPARAM_TryPerformStackOp_p = __csLOCALPARAM_LesOP_p;
                                                       static Cell *__csLOCAL_TryPerformStackOp_phead;
                                                       static Cell *__csLOCAL_TryPerformStackOp_pnext;
tinstrPop6_0_33: IF(7,33,tinstrPop6_0_34)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 1)
                                                       					{
tinstrPop6_0_34: IF(7,34,tinstrPop6_0_35)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop6_0_35: IF(7,35,tinstrPop6_0_36)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = __csLOCAL_TryPerformStackOp_phead;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_4;
tinstrPop6_0_36: IF(7,36,tinstrPop6_0_37)              __csLOCAL_TryPerformStackOp___if_cond_4 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, &(*__csLOCALPARAM_TryPerformStackOp_p).cell);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_4)
                                                       						{
                                                       __retval__TryPerformStackOp_14 = 1;
                                                       goto __exit__TryPerformStackOp_14;
                                                       						}
                                                       						else
                                                       						{ 
                                                       __retval__TryPerformStackOp_14 = 0;
                                                       goto __exit__TryPerformStackOp_14;
                                                       						}
                                                       						;
                                                       					}
                                                       					GUARD(7,37);
tinstrPop6_0_37: IF(7,37,tinstrPop6_0_38)              if ((*__csLOCALPARAM_TryPerformStackOp_p).op == 0)
                                                       					{
tinstrPop6_0_38: IF(7,38,tinstrPop6_0_39)              __csLOCAL_TryPerformStackOp_phead = S.ptop;
tinstrPop6_0_39: IF(7,39,tinstrPop6_0_40)              if (__csLOCAL_TryPerformStackOp_phead == 0)
                                                       						{
tinstrPop6_0_40: IF(7,40,tinstrPop6_0_41)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop6_0_41: IF(7,41,tinstrPop6_0_42)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_14 = 1;
                                                       goto __exit__TryPerformStackOp_14;
                                                       						}
                                                       						GUARD(7,42);
tinstrPop6_0_42: IF(7,42,tinstrPop6_0_43)              __csLOCAL_TryPerformStackOp_pnext = (*__csLOCAL_TryPerformStackOp_phead).pnext;
                                                       ;
                                                       static _Bool __csLOCAL_TryPerformStackOp___if_cond_5;
tinstrPop6_0_43: IF(7,43,tinstrPop6_0_44)              __csLOCAL_TryPerformStackOp___if_cond_5 = __CS_atomicc_cas(&S.ptop, __csLOCAL_TryPerformStackOp_phead, __csLOCAL_TryPerformStackOp_pnext);
                                                       if (__csLOCAL_TryPerformStackOp___if_cond_5)
                                                       						{
tinstrPop6_0_44: IF(7,44,tinstrPop6_0_45)              (*__csLOCALPARAM_TryPerformStackOp_p).cell = *__csLOCAL_TryPerformStackOp_phead;
tinstrPop6_0_45: IF(7,45,tinstrPop6_0_46)              //__CS_atomic_begin;
                                                       static int __csLOCAL_TryPerformStackOp_i;
                                                       __csLOCAL_TryPerformStackOp_i = __VERIFIER_nondet_int();
                                                       __VERIFIER_assume((0 <= __csLOCAL_TryPerformStackOp_i) && (__csLOCAL_TryPerformStackOp_i < 4));
                                                       __VERIFIER_assume((&threads[__csLOCAL_TryPerformStackOp_i].cell) == __csLOCAL_TryPerformStackOp_phead);
                                                       allocated[__csLOCAL_TryPerformStackOp_i] = 0;
                                                       //__CS_atomic_end;
                                                       __retval__TryPerformStackOp_14 = 1;
                                                       goto __exit__TryPerformStackOp_14;
                                                       						}
                                                       						else
                                                       						{ GUARD(7,46)
tinstrPop6_0_46: IF(7,46,tinstrPop6_0_47)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pnext = 0;
tinstrPop6_0_47: IF(7,47,tinstrPop6_0_48)              (*__csLOCALPARAM_TryPerformStackOp_p).cell.pdata = 2;
                                                       __retval__TryPerformStackOp_14 = 0;
                                                       goto __exit__TryPerformStackOp_14;
                                                       						}
                                                       						GUARD(7,48);
                                                       					}
                                                       					GUARD(7,48);
                                                       					__exit__TryPerformStackOp_14: GUARD(7,48)
                                                       ;
                                                       				};
                                                       __csLOCAL_LesOP___if_cond_3 = __retval__TryPerformStackOp_14 == 1;
                                                       if (__csLOCAL_LesOP___if_cond_3)
                                                       				{
                                                       goto __exit__LesOP_7;
                                                       				}
                                                       				;
tinstrPop6_0_48: IF(7,48,tinstrPop6_0_49)              __VERIFIER_assume(0);
                                                       				__exit__LesOP_7: GUARD(7,49)
                                                       ;
                                                       			};
                                                       0;
                                                       		}
                                                       		GUARD(7,49);
                                                       static int __csLOCAL_Pop_v;
tinstrPop6_0_49: IF(7,49,tinstrPop6_0_50)              __csLOCAL_Pop_v = (*__csLOCAL_Pop_ti).cell.pdata;
tinstrPop6_0_50: IF(7,50,tinstrPop6_0_51)              __CS_atomic_free_ThreadInfo(__csLOCAL_Pop_ti);
                                                       __retval__Pop_4 = __csLOCAL_Pop_v;
                                                       goto __exit__Pop_4;
                                                       		__exit__Pop_4: GUARD(7,51)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop6_localPop_ret = __retval__Pop_4;
tinstrPop6_0_51: IF(7,51,tinstrPop6_0_52)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop6_localPop_ret);
                                                       goto __exit_instrPop6;
                                                       	__exit_instrPop6: GUARD(7,52)
                                                       ;
tinstrPop6_0_52:                                       STOP_NONVOID(52);
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
                                                       static __cs_t __csLOCAL_main_tid5;
                                                       __cs_init_scalar(&__csLOCAL_main_tid5, sizeof(__cs_t));
                                                       static __cs_t __csLOCAL_main_tid6;
                                                       __cs_init_scalar(&__csLOCAL_main_tid6, sizeof(__cs_t));
                                                       static __cs_t __csLOCAL_main_tid7;
                                                       __cs_init_scalar(&__csLOCAL_main_tid7, sizeof(__cs_t));
                                                       __cs_create(&__csLOCAL_main_tid1, 0, &instrPush0_0, 0, 1);
tmain_1: IF(0,1,tmain_2)                               __cs_create(&__csLOCAL_main_tid2, 0, &instrPush1_0, 0, 2);
tmain_2: IF(0,2,tmain_3)                               __cs_create(&__csLOCAL_main_tid3, 0, &instrPush2_0, 0, 3);
tmain_3: IF(0,3,tmain_4)                               __cs_create(&__csLOCAL_main_tid4, 0, &instrPop3_0, 0, 4);
tmain_4: IF(0,4,tmain_5)                               __cs_create(&__csLOCAL_main_tid5, 0, &instrPop4_0, 0, 5);
tmain_5: IF(0,5,tmain_6)                               __cs_create(&__csLOCAL_main_tid6, 0, &instrPop5_0, 0, 6);
tmain_6: IF(0,6,tmain_7)                               __cs_create(&__csLOCAL_main_tid7, 0, &instrPop6_0, 0, 7);
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
                                                                    assume((pc_cs >= 0) && (pc_cs <= 51));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 51));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 51));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 52));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 5;
                                                                 if (active_thread[5] == 1) { /** instrPop4_0 **/
                                                                    pc_cs = pc[5] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 52));
                                                                    instrPop4_0(threadargs[5]);
                                                                    pc[5] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 6;
                                                                 if (active_thread[6] == 1) { /** instrPop5_0 **/
                                                                    pc_cs = pc[6] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 52));
                                                                    instrPop5_0(threadargs[6]);
                                                                    pc[6] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 7;
                                                                 if (active_thread[7] == 1) { /** instrPop6_0 **/
                                                                    pc_cs = pc[7] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 52));
                                                                    instrPop6_0(threadargs[7]);
                                                                    pc[7] = pc_cs;
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
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 51));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 51));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 51));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= pc[4]) && (pc_cs <= 52));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 5;
                                                                 if (active_thread[5] == 1) { /** instrPop4_0 **/
                                                                    pc_cs = pc[5] + guess_pc();
                                                                    assume((pc_cs >= pc[5]) && (pc_cs <= 52));
                                                                    instrPop4_0(threadargs[5]);
                                                                    pc[5] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 6;
                                                                 if (active_thread[6] == 1) { /** instrPop5_0 **/
                                                                    pc_cs = pc[6] + guess_pc();
                                                                    assume((pc_cs >= pc[6]) && (pc_cs <= 52));
                                                                    instrPop5_0(threadargs[6]);
                                                                    pc[6] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 7;
                                                                 if (active_thread[7] == 1) { /** instrPop6_0 **/
                                                                    pc_cs = pc[7] + guess_pc();
                                                                    assume((pc_cs >= pc[7]) && (pc_cs <= 52));
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
                                                       


