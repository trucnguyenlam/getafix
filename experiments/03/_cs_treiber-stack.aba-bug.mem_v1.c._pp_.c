/*  Generated 0.0.0.0.0.0 (-t7 -r3 -u1 -bcbmc) 2016-04-15 09:50:09  */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define THREADS 7
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
unsigned __CPROVER_bitvector[4] pc[THREADS+1];
unsigned __CPROVER_bitvector[5] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[4] thread_lines[] = {7, 10, 10, 10, 14, 14, 14, 14};

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
int __cs_sched_yield(unsigned __CPROVER_bitvector[5] val) {pc_cs = val + 1; return 0;}    // Version 1
//int __cs_sched_yield(unsigned __CPROVER_bitvector[5] val) {return 0;}     // Version 2


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

unsigned __CPROVER_bitvector[4] guess_pc() {unsigned __CPROVER_bitvector[4] i; return i;}

/* here */


typedef int val_t;
typedef int addr_t;
                                                       val_t MEM_VALUE[25];
                                                       void __CS_atomic_init_MEMORY(void)
                                                       {
                                                         MEM_VALUE[1] = 0;
                                                         MEM_VALUE[2] = 0;
                                                         MEM_VALUE[3] = 0;
                                                         MEM_VALUE[4] = 0;
                                                         MEM_VALUE[5] = 0;
                                                         MEM_VALUE[6] = 0;
                                                         MEM_VALUE[7] = 0;
                                                       }
                                                       
                                                       addr_t __CS_atomic_malloc_Node()
                                                       {
                                                         static int __csLOCAL___CS_atomic_malloc_Node_i;
                                                         __csLOCAL___CS_atomic_malloc_Node_i = __VERIFIER_nondet_int();
                                                         __VERIFIER_assume((0 <= __csLOCAL___CS_atomic_malloc_Node_i) && (__csLOCAL___CS_atomic_malloc_Node_i < 2));
                                                         __VERIFIER_assume(!MEM_VALUE[6 + __csLOCAL___CS_atomic_malloc_Node_i]);
                                                         MEM_VALUE[6 + __csLOCAL___CS_atomic_malloc_Node_i] = 1;
                                                         return 2 + (__csLOCAL___CS_atomic_malloc_Node_i * 2);
                                                       }
                                                       
                                                       int __cs_function_malloc_Node_inlined = 1;
                                                       int __cs_function_free_Node_inlined = 1;
                                                       _Bool __CS_atomic_CAS(addr_t __csLOCALPARAM___CS_atomic_CAS_address_ptr, addr_t __csLOCALPARAM___CS_atomic_CAS_old_address, addr_t __csLOCALPARAM___CS_atomic_CAS_new_address)
                                                       {
                                                         if (MEM_VALUE[__csLOCALPARAM___CS_atomic_CAS_address_ptr] == __csLOCALPARAM___CS_atomic_CAS_old_address)
                                                         {
                                                           MEM_VALUE[__csLOCALPARAM___CS_atomic_CAS_address_ptr] = __csLOCALPARAM___CS_atomic_CAS_new_address;
                                                           return 1;
                                                         }
                                                       
                                                         return 0;
                                                       }
                                                       
                                                       int __cs_function_push_aux_t0_inlined = 1;
                                                       addr_t n_push_t0;
                                                       int __cs_function_Push_t0_inlined = 1;
                                                       int __cs_function_push_aux_t1_inlined = 1;
                                                       addr_t n_push_t1;
                                                       int __cs_function_Push_t1_inlined = 1;
                                                       int __cs_function_push_aux_t2_inlined = 1;
                                                       addr_t n_push_t2;
                                                       int __cs_function_Push_t2_inlined = 1;
                                                       int __cs_function_pop_aux_t0_inlined = 1;
                                                       addr_t n_pop_t0;
                                                       int __cs_function_Pop_t0_inlined = 1;
                                                       int __cs_function_pop_aux_t1_inlined = 1;
                                                       addr_t n_pop_t1;
                                                       int __cs_function_Pop_t1_inlined = 1;
                                                       int __cs_function_pop_aux_t2_inlined = 1;
                                                       addr_t n_pop_t2;
                                                       int __cs_function_Pop_t2_inlined = 1;
                                                       int __cs_function_pop_aux_t3_inlined = 1;
                                                       addr_t n_pop_t3;
                                                       int __cs_function_Pop_t3_inlined = 1;
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
IF(1,0,tinstrPush0_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_t0_i;
                                                       __csLOCALPARAM_Push_t0_i = 1;
                                                       static addr_t __retval__malloc_Node_1;
                                                       		{
tinstrPush0_0_1: IF(1,1,tinstrPush0_0_2)               __retval__malloc_Node_1 = __CS_atomic_malloc_Node();
                                                       goto __exit__malloc_Node_1;
                                                       			__exit__malloc_Node_1: GUARD(1,2)
                                                       ;
                                                       		};
tinstrPush0_0_2: IF(1,2,tinstrPush0_0_3)               n_push_t0 = __retval__malloc_Node_1;
tinstrPush0_0_3: IF(1,3,tinstrPush0_0_4)               MEM_VALUE[n_push_t0 + 1] = __csLOCALPARAM_Push_t0_i;
                                                       		{
                                                       static addr_t __csLOCALPARAM_push_aux_t0_n;
tinstrPush0_0_4: IF(1,4,tinstrPush0_0_5)               __csLOCALPARAM_push_aux_t0_n = n_push_t0;
tinstrPush0_0_5: IF(1,5,tinstrPush0_0_6)               MEM_VALUE[8] = MEM_VALUE[1];
tinstrPush0_0_6: IF(1,6,tinstrPush0_0_7)               MEM_VALUE[__csLOCALPARAM_push_aux_t0_n + 0] = MEM_VALUE[8];
                                                       ;
                                                       static _Bool __csLOCAL_push_aux_t0___if_cond_0;
tinstrPush0_0_7: IF(1,7,tinstrPush0_0_8)               __csLOCAL_push_aux_t0___if_cond_0 = __CS_atomic_CAS(1, MEM_VALUE[8], __csLOCALPARAM_push_aux_t0_n);
                                                       if (__csLOCAL_push_aux_t0___if_cond_0)
                                                       			{
                                                       goto __exit__push_aux_t0_1;
                                                       			}
                                                       			;
tinstrPush0_0_8: IF(1,8,tinstrPush0_0_9)               __VERIFIER_assume(0);
                                                       			__exit__push_aux_t0_1: GUARD(1,9)
                                                       ;
                                                       		};
                                                       0;
                                                       		__exit__Push_t0_1: GUARD(1,9)
                                                       ;
                                                       	};
                                                       0;
tinstrPush0_0_9: IF(1,9,tinstrPush0_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush0;
                                                       	__exit_instrPush0: GUARD(1,10)
                                                       ;
tinstrPush0_0_10:                                      STOP_NONVOID(10);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush1_0(void *__csLOCALPARAM_instrPush1_unused)
                                                       {
IF(2,0,tinstrPush1_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_t1_i;
                                                       __csLOCALPARAM_Push_t1_i = 1;
                                                       static addr_t __retval__malloc_Node_2;
                                                       		{
tinstrPush1_0_1: IF(2,1,tinstrPush1_0_2)               __retval__malloc_Node_2 = __CS_atomic_malloc_Node();
                                                       goto __exit__malloc_Node_2;
                                                       			__exit__malloc_Node_2: GUARD(2,2)
                                                       ;
                                                       		};
tinstrPush1_0_2: IF(2,2,tinstrPush1_0_3)               n_push_t1 = __retval__malloc_Node_2;
tinstrPush1_0_3: IF(2,3,tinstrPush1_0_4)               MEM_VALUE[n_push_t1 + 1] = __csLOCALPARAM_Push_t1_i;
                                                       		{
                                                       static addr_t __csLOCALPARAM_push_aux_t1_n;
tinstrPush1_0_4: IF(2,4,tinstrPush1_0_5)               __csLOCALPARAM_push_aux_t1_n = n_push_t1;
tinstrPush1_0_5: IF(2,5,tinstrPush1_0_6)               MEM_VALUE[9] = MEM_VALUE[1];
tinstrPush1_0_6: IF(2,6,tinstrPush1_0_7)               MEM_VALUE[__csLOCALPARAM_push_aux_t1_n + 0] = MEM_VALUE[9];
                                                       ;
                                                       static _Bool __csLOCAL_push_aux_t1___if_cond_1;
tinstrPush1_0_7: IF(2,7,tinstrPush1_0_8)               __csLOCAL_push_aux_t1___if_cond_1 = __CS_atomic_CAS(1, MEM_VALUE[9], __csLOCALPARAM_push_aux_t1_n);
                                                       if (__csLOCAL_push_aux_t1___if_cond_1)
                                                       			{
                                                       goto __exit__push_aux_t1_1;
                                                       			}
                                                       			;
tinstrPush1_0_8: IF(2,8,tinstrPush1_0_9)               __VERIFIER_assume(0);
                                                       			__exit__push_aux_t1_1: GUARD(2,9)
                                                       ;
                                                       		};
                                                       0;
                                                       		__exit__Push_t1_1: GUARD(2,9)
                                                       ;
                                                       	};
                                                       0;
tinstrPush1_0_9: IF(2,9,tinstrPush1_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush1;
                                                       	__exit_instrPush1: GUARD(2,10)
                                                       ;
tinstrPush1_0_10:                                      STOP_NONVOID(10);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPush2_0(void *__csLOCALPARAM_instrPush2_unused)
                                                       {
IF(3,0,tinstrPush2_0_1)                                __CS_atomic_atomicIncr_Push(1);
                                                       	{
                                                       static int __csLOCALPARAM_Push_t2_i;
                                                       __csLOCALPARAM_Push_t2_i = 1;
                                                       static addr_t __retval__malloc_Node_3;
                                                       		{
tinstrPush2_0_1: IF(3,1,tinstrPush2_0_2)               __retval__malloc_Node_3 = __CS_atomic_malloc_Node();
                                                       goto __exit__malloc_Node_3;
                                                       			__exit__malloc_Node_3: GUARD(3,2)
                                                       ;
                                                       		};
tinstrPush2_0_2: IF(3,2,tinstrPush2_0_3)               n_push_t2 = __retval__malloc_Node_3;
tinstrPush2_0_3: IF(3,3,tinstrPush2_0_4)               MEM_VALUE[n_push_t2 + 1] = __csLOCALPARAM_Push_t2_i;
                                                       		{
                                                       static addr_t __csLOCALPARAM_push_aux_t2_n;
tinstrPush2_0_4: IF(3,4,tinstrPush2_0_5)               __csLOCALPARAM_push_aux_t2_n = n_push_t2;
tinstrPush2_0_5: IF(3,5,tinstrPush2_0_6)               MEM_VALUE[10] = MEM_VALUE[1];
tinstrPush2_0_6: IF(3,6,tinstrPush2_0_7)               MEM_VALUE[__csLOCALPARAM_push_aux_t2_n + 0] = MEM_VALUE[10];
                                                       ;
                                                       static _Bool __csLOCAL_push_aux_t2___if_cond_2;
tinstrPush2_0_7: IF(3,7,tinstrPush2_0_8)               __csLOCAL_push_aux_t2___if_cond_2 = __CS_atomic_CAS(1, MEM_VALUE[10], __csLOCALPARAM_push_aux_t2_n);
                                                       if (__csLOCAL_push_aux_t2___if_cond_2)
                                                       			{
                                                       goto __exit__push_aux_t2_1;
                                                       			}
                                                       			;
tinstrPush2_0_8: IF(3,8,tinstrPush2_0_9)               __VERIFIER_assume(0);
                                                       			__exit__push_aux_t2_1: GUARD(3,9)
                                                       ;
                                                       		};
                                                       0;
                                                       		__exit__Push_t2_1: GUARD(3,9)
                                                       ;
                                                       	};
                                                       0;
tinstrPush2_0_9: IF(3,9,tinstrPush2_0_10)              __CS_atomic_atomicDecrIncr_Push(1);
                                                       goto __exit_instrPush2;
                                                       	__exit_instrPush2: GUARD(3,10)
                                                       ;
tinstrPush2_0_10:                                      STOP_NONVOID(10);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop3_0(void *__csLOCALPARAM_instrPop3_unused)
                                                       {
IF(4,0,tinstrPop3_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop3_localPop_ret;
                                                       static int __retval__Pop_t0_1;
                                                       	{
                                                       static addr_t __retval__pop_aux_t0_1;
                                                       		{
tinstrPop3_0_1: IF(4,1,tinstrPop3_0_2)                 MEM_VALUE[11] = MEM_VALUE[1];
tinstrPop3_0_2: IF(4,2,tinstrPop3_0_3)                 if (MEM_VALUE[11] == 0)
                                                       			{
                                                       __retval__pop_aux_t0_1 = 0;
                                                       goto __exit__pop_aux_t0_1;
                                                       			}
                                                       			;
tinstrPop3_0_3: IF(4,3,tinstrPop3_0_4)                 MEM_VALUE[12] = MEM_VALUE[MEM_VALUE[11]];
                                                       ;
                                                       static _Bool __csLOCAL_pop_aux_t0___if_cond_3;
tinstrPop3_0_4: IF(4,4,tinstrPop3_0_5)                 __csLOCAL_pop_aux_t0___if_cond_3 = __CS_atomic_CAS(1, MEM_VALUE[11], MEM_VALUE[12]);
                                                       if (__csLOCAL_pop_aux_t0___if_cond_3)
                                                       			{
tinstrPop3_0_5: IF(4,5,tinstrPop3_0_6)                 __retval__pop_aux_t0_1 = MEM_VALUE[11];
                                                       goto __exit__pop_aux_t0_1;
                                                       			}
                                                       			GUARD(4,6);
tinstrPop3_0_6: IF(4,6,tinstrPop3_0_7)                 __VERIFIER_assume(0);
                                                       			__exit__pop_aux_t0_1: GUARD(4,7)
                                                       ;
                                                       		};
tinstrPop3_0_7: IF(4,7,tinstrPop3_0_8)                 n_pop_t0 = __retval__pop_aux_t0_1;
tinstrPop3_0_8: IF(4,8,tinstrPop3_0_9)                 if (n_pop_t0 == 0)
                                                       		{
                                                       __retval__Pop_t0_1 = 2;
                                                       goto __exit__Pop_t0_1;
                                                       		}
                                                       		else
                                                       		{ 
                                                       static int __csLOCAL_Pop_t0_v;
tinstrPop3_0_9: IF(4,9,tinstrPop3_0_10)                __csLOCAL_Pop_t0_v = MEM_VALUE[n_pop_t0 + 1];
                                                       			{
                                                       static addr_t __csLOCALPARAM_free_Node_n;
tinstrPop3_0_10: IF(4,10,tinstrPop3_0_11)              __csLOCALPARAM_free_Node_n = n_pop_t0;
                                                       static int __csLOCAL_free_Node_i;
                                                       __csLOCAL_free_Node_i = __VERIFIER_nondet_int();
tinstrPop3_0_11: IF(4,11,tinstrPop3_0_12)              __VERIFIER_assume((2 + (__csLOCAL_free_Node_i * 2)) == __csLOCALPARAM_free_Node_n);
tinstrPop3_0_12: IF(4,12,tinstrPop3_0_13)              MEM_VALUE[6 + __csLOCAL_free_Node_i] = 0;
                                                       				__exit__free_Node_1: GUARD(4,13)
                                                       ;
                                                       			};
                                                       0;
                                                       __retval__Pop_t0_1 = __csLOCAL_Pop_t0_v;
                                                       goto __exit__Pop_t0_1;
                                                       		}
                                                       		GUARD(4,13);
                                                       		__exit__Pop_t0_1: GUARD(4,13)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop3_localPop_ret = __retval__Pop_t0_1;
tinstrPop3_0_13: IF(4,13,tinstrPop3_0_14)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop3_localPop_ret);
                                                       goto __exit_instrPop3;
                                                       	__exit_instrPop3: GUARD(4,14)
                                                       ;
tinstrPop3_0_14:                                       STOP_NONVOID(14);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop4_0(void *__csLOCALPARAM_instrPop4_unused)
                                                       {
IF(5,0,tinstrPop4_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop4_localPop_ret;
                                                       static int __retval__Pop_t1_1;
                                                       	{
                                                       static addr_t __retval__pop_aux_t1_1;
                                                       		{
tinstrPop4_0_1: IF(5,1,tinstrPop4_0_2)                 MEM_VALUE[13] = MEM_VALUE[1];
tinstrPop4_0_2: IF(5,2,tinstrPop4_0_3)                 if (MEM_VALUE[13] == 0)
                                                       			{
                                                       __retval__pop_aux_t1_1 = 0;
                                                       goto __exit__pop_aux_t1_1;
                                                       			}
                                                       			;
tinstrPop4_0_3: IF(5,3,tinstrPop4_0_4)                 MEM_VALUE[14] = MEM_VALUE[MEM_VALUE[13]];
                                                       ;
                                                       static _Bool __csLOCAL_pop_aux_t1___if_cond_4;
tinstrPop4_0_4: IF(5,4,tinstrPop4_0_5)                 __csLOCAL_pop_aux_t1___if_cond_4 = __CS_atomic_CAS(1, MEM_VALUE[13], MEM_VALUE[14]);
                                                       if (__csLOCAL_pop_aux_t1___if_cond_4)
                                                       			{
tinstrPop4_0_5: IF(5,5,tinstrPop4_0_6)                 __retval__pop_aux_t1_1 = MEM_VALUE[13];
                                                       goto __exit__pop_aux_t1_1;
                                                       			}
                                                       			GUARD(5,6);
tinstrPop4_0_6: IF(5,6,tinstrPop4_0_7)                 __VERIFIER_assume(0);
                                                       			__exit__pop_aux_t1_1: GUARD(5,7)
                                                       ;
                                                       		};
tinstrPop4_0_7: IF(5,7,tinstrPop4_0_8)                 n_pop_t1 = __retval__pop_aux_t1_1;
tinstrPop4_0_8: IF(5,8,tinstrPop4_0_9)                 if (n_pop_t1 == 0)
                                                       		{
                                                       __retval__Pop_t1_1 = 2;
                                                       goto __exit__Pop_t1_1;
                                                       		}
                                                       		else
                                                       		{ 
                                                       static int __csLOCAL_Pop_t1_v;
tinstrPop4_0_9: IF(5,9,tinstrPop4_0_10)                __csLOCAL_Pop_t1_v = MEM_VALUE[n_pop_t1 + 1];
                                                       			{
                                                       static addr_t __csLOCALPARAM_free_Node_n;
tinstrPop4_0_10: IF(5,10,tinstrPop4_0_11)              __csLOCALPARAM_free_Node_n = n_pop_t1;
                                                       static int __csLOCAL_free_Node_i;
                                                       __csLOCAL_free_Node_i = __VERIFIER_nondet_int();
tinstrPop4_0_11: IF(5,11,tinstrPop4_0_12)              __VERIFIER_assume((2 + (__csLOCAL_free_Node_i * 2)) == __csLOCALPARAM_free_Node_n);
tinstrPop4_0_12: IF(5,12,tinstrPop4_0_13)              MEM_VALUE[6 + __csLOCAL_free_Node_i] = 0;
                                                       				__exit__free_Node_2: GUARD(5,13)
                                                       ;
                                                       			};
                                                       0;
                                                       __retval__Pop_t1_1 = __csLOCAL_Pop_t1_v;
                                                       goto __exit__Pop_t1_1;
                                                       		}
                                                       		GUARD(5,13);
                                                       		__exit__Pop_t1_1: GUARD(5,13)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop4_localPop_ret = __retval__Pop_t1_1;
tinstrPop4_0_13: IF(5,13,tinstrPop4_0_14)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop4_localPop_ret);
                                                       goto __exit_instrPop4;
                                                       	__exit_instrPop4: GUARD(5,14)
                                                       ;
tinstrPop4_0_14:                                       STOP_NONVOID(14);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop5_0(void *__csLOCALPARAM_instrPop5_unused)
                                                       {
IF(6,0,tinstrPop5_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop5_localPop_ret;
                                                       static int __retval__Pop_t2_1;
                                                       	{
                                                       static addr_t __retval__pop_aux_t2_1;
                                                       		{
tinstrPop5_0_1: IF(6,1,tinstrPop5_0_2)                 MEM_VALUE[15] = MEM_VALUE[1];
tinstrPop5_0_2: IF(6,2,tinstrPop5_0_3)                 if (MEM_VALUE[15] == 0)
                                                       			{
                                                       __retval__pop_aux_t2_1 = 0;
                                                       goto __exit__pop_aux_t2_1;
                                                       			}
                                                       			;
tinstrPop5_0_3: IF(6,3,tinstrPop5_0_4)                 MEM_VALUE[16] = MEM_VALUE[MEM_VALUE[15]];
                                                       ;
                                                       static _Bool __csLOCAL_pop_aux_t2___if_cond_5;
tinstrPop5_0_4: IF(6,4,tinstrPop5_0_5)                 __csLOCAL_pop_aux_t2___if_cond_5 = __CS_atomic_CAS(1, MEM_VALUE[15], MEM_VALUE[16]);
                                                       if (__csLOCAL_pop_aux_t2___if_cond_5)
                                                       			{
tinstrPop5_0_5: IF(6,5,tinstrPop5_0_6)                 __retval__pop_aux_t2_1 = MEM_VALUE[15];
                                                       goto __exit__pop_aux_t2_1;
                                                       			}
                                                       			GUARD(6,6);
tinstrPop5_0_6: IF(6,6,tinstrPop5_0_7)                 __VERIFIER_assume(0);
                                                       			__exit__pop_aux_t2_1: GUARD(6,7)
                                                       ;
                                                       		};
tinstrPop5_0_7: IF(6,7,tinstrPop5_0_8)                 n_pop_t2 = __retval__pop_aux_t2_1;
tinstrPop5_0_8: IF(6,8,tinstrPop5_0_9)                 if (n_pop_t2 == 0)
                                                       		{
                                                       __retval__Pop_t2_1 = 2;
                                                       goto __exit__Pop_t2_1;
                                                       		}
                                                       		else
                                                       		{ 
                                                       static int __csLOCAL_Pop_t2_v;
tinstrPop5_0_9: IF(6,9,tinstrPop5_0_10)                __csLOCAL_Pop_t2_v = MEM_VALUE[n_pop_t2 + 1];
                                                       			{
                                                       static addr_t __csLOCALPARAM_free_Node_n;
tinstrPop5_0_10: IF(6,10,tinstrPop5_0_11)              __csLOCALPARAM_free_Node_n = n_pop_t2;
                                                       static int __csLOCAL_free_Node_i;
                                                       __csLOCAL_free_Node_i = __VERIFIER_nondet_int();
tinstrPop5_0_11: IF(6,11,tinstrPop5_0_12)              __VERIFIER_assume((2 + (__csLOCAL_free_Node_i * 2)) == __csLOCALPARAM_free_Node_n);
tinstrPop5_0_12: IF(6,12,tinstrPop5_0_13)              MEM_VALUE[6 + __csLOCAL_free_Node_i] = 0;
                                                       				__exit__free_Node_3: GUARD(6,13)
                                                       ;
                                                       			};
                                                       0;
                                                       __retval__Pop_t2_1 = __csLOCAL_Pop_t2_v;
                                                       goto __exit__Pop_t2_1;
                                                       		}
                                                       		GUARD(6,13);
                                                       		__exit__Pop_t2_1: GUARD(6,13)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop5_localPop_ret = __retval__Pop_t2_1;
tinstrPop5_0_13: IF(6,13,tinstrPop5_0_14)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop5_localPop_ret);
                                                       goto __exit_instrPop5;
                                                       	__exit_instrPop5: GUARD(6,14)
                                                       ;
tinstrPop5_0_14:                                       STOP_NONVOID(14);
                                                       }
                                                       
                                                       
                                                       
                                                       void *instrPop6_0(void *__csLOCALPARAM_instrPop6_unused)
                                                       {
IF(7,0,tinstrPop6_0_1)                                 __CS_atomic_atomicIncr_Pop();
                                                       static int __csLOCAL_instrPop6_localPop_ret;
                                                       static int __retval__Pop_t3_1;
                                                       	{
                                                       static addr_t __retval__pop_aux_t3_1;
                                                       		{
tinstrPop6_0_1: IF(7,1,tinstrPop6_0_2)                 MEM_VALUE[17] = MEM_VALUE[1];
tinstrPop6_0_2: IF(7,2,tinstrPop6_0_3)                 if (MEM_VALUE[17] == 0)
                                                       			{
                                                       __retval__pop_aux_t3_1 = 0;
                                                       goto __exit__pop_aux_t3_1;
                                                       			}
                                                       			;
tinstrPop6_0_3: IF(7,3,tinstrPop6_0_4)                 MEM_VALUE[18] = MEM_VALUE[MEM_VALUE[17]];
                                                       ;
                                                       static _Bool __csLOCAL_pop_aux_t3___if_cond_6;
tinstrPop6_0_4: IF(7,4,tinstrPop6_0_5)                 __csLOCAL_pop_aux_t3___if_cond_6 = __CS_atomic_CAS(1, MEM_VALUE[17], MEM_VALUE[18]);
                                                       if (__csLOCAL_pop_aux_t3___if_cond_6)
                                                       			{
tinstrPop6_0_5: IF(7,5,tinstrPop6_0_6)                 __retval__pop_aux_t3_1 = MEM_VALUE[17];
                                                       goto __exit__pop_aux_t3_1;
                                                       			}
                                                       			GUARD(7,6);
tinstrPop6_0_6: IF(7,6,tinstrPop6_0_7)                 __VERIFIER_assume(0);
                                                       			__exit__pop_aux_t3_1: GUARD(7,7)
                                                       ;
                                                       		};
tinstrPop6_0_7: IF(7,7,tinstrPop6_0_8)                 n_pop_t3 = __retval__pop_aux_t3_1;
tinstrPop6_0_8: IF(7,8,tinstrPop6_0_9)                 if (n_pop_t3 == 0)
                                                       		{
                                                       __retval__Pop_t3_1 = 2;
                                                       goto __exit__Pop_t3_1;
                                                       		}
                                                       		else
                                                       		{ 
                                                       static int __csLOCAL_Pop_t3_v;
tinstrPop6_0_9: IF(7,9,tinstrPop6_0_10)                __csLOCAL_Pop_t3_v = MEM_VALUE[n_pop_t3 + 1];
                                                       			{
                                                       static addr_t __csLOCALPARAM_free_Node_n;
tinstrPop6_0_10: IF(7,10,tinstrPop6_0_11)              __csLOCALPARAM_free_Node_n = n_pop_t3;
                                                       static int __csLOCAL_free_Node_i;
                                                       __csLOCAL_free_Node_i = __VERIFIER_nondet_int();
tinstrPop6_0_11: IF(7,11,tinstrPop6_0_12)              __VERIFIER_assume((2 + (__csLOCAL_free_Node_i * 2)) == __csLOCALPARAM_free_Node_n);
tinstrPop6_0_12: IF(7,12,tinstrPop6_0_13)              MEM_VALUE[6 + __csLOCAL_free_Node_i] = 0;
                                                       				__exit__free_Node_4: GUARD(7,13)
                                                       ;
                                                       			};
                                                       0;
                                                       __retval__Pop_t3_1 = __csLOCAL_Pop_t3_v;
                                                       goto __exit__Pop_t3_1;
                                                       		}
                                                       		GUARD(7,13);
                                                       		__exit__Pop_t3_1: GUARD(7,13)
                                                       ;
                                                       	};
                                                       __csLOCAL_instrPop6_localPop_ret = __retval__Pop_t3_1;
tinstrPop6_0_13: IF(7,13,tinstrPop6_0_14)              __CS_atomic_atomicDecrIncr_Pop(__csLOCAL_instrPop6_localPop_ret);
                                                       goto __exit_instrPop6;
                                                       	__exit_instrPop6: GUARD(7,14)
                                                       ;
tinstrPop6_0_14:                                       STOP_NONVOID(14);
                                                       }
                                                       
                                                       
                                                       
                                                       int main_thread(void)
                                                       {
IF(0,0,tmain_1)                                        	{
                                                       __CS_atomic_init_MEMORY();
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
                                                                    assume((pc_cs >= 0) && (pc_cs <= 10));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 10));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 10));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 14));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 5;
                                                                 if (active_thread[5] == 1) { /** instrPop4_0 **/
                                                                    pc_cs = pc[5] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 14));
                                                                    instrPop4_0(threadargs[5]);
                                                                    pc[5] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 6;
                                                                 if (active_thread[6] == 1) { /** instrPop5_0 **/
                                                                    pc_cs = pc[6] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 14));
                                                                    instrPop5_0(threadargs[6]);
                                                                    pc[6] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 7;
                                                                 if (active_thread[7] == 1) { /** instrPop6_0 **/
                                                                    pc_cs = pc[7] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 14));
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
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 10));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 10));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 10));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= pc[4]) && (pc_cs <= 14));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 5;
                                                                 if (active_thread[5] == 1) { /** instrPop4_0 **/
                                                                    pc_cs = pc[5] + guess_pc();
                                                                    assume((pc_cs >= pc[5]) && (pc_cs <= 14));
                                                                    instrPop4_0(threadargs[5]);
                                                                    pc[5] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 6;
                                                                 if (active_thread[6] == 1) { /** instrPop5_0 **/
                                                                    pc_cs = pc[6] + guess_pc();
                                                                    assume((pc_cs >= pc[6]) && (pc_cs <= 14));
                                                                    instrPop5_0(threadargs[6]);
                                                                    pc[6] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 7;
                                                                 if (active_thread[7] == 1) { /** instrPop6_0 **/
                                                                    pc_cs = pc[7] + guess_pc();
                                                                    assume((pc_cs >= pc[7]) && (pc_cs <= 14));
                                                                    instrPop6_0(threadargs[7]);
                                                                    pc[7] = pc_cs;
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
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 10));
                                                                    instrPush0_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** instrPush1_0 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 10));
                                                                    instrPush1_0(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** instrPush2_0 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 10));
                                                                    instrPush2_0(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 4;
                                                                 if (active_thread[4] == 1) { /** instrPop3_0 **/
                                                                    pc_cs = pc[4] + guess_pc();
                                                                    assume((pc_cs >= pc[4]) && (pc_cs <= 14));
                                                                    instrPop3_0(threadargs[4]);
                                                                    pc[4] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 5;
                                                                 if (active_thread[5] == 1) { /** instrPop4_0 **/
                                                                    pc_cs = pc[5] + guess_pc();
                                                                    assume((pc_cs >= pc[5]) && (pc_cs <= 14));
                                                                    instrPop4_0(threadargs[5]);
                                                                    pc[5] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 6;
                                                                 if (active_thread[6] == 1) { /** instrPop5_0 **/
                                                                    pc_cs = pc[6] + guess_pc();
                                                                    assume((pc_cs >= pc[6]) && (pc_cs <= 14));
                                                                    instrPop5_0(threadargs[6]);
                                                                    pc[6] = pc_cs;
                                                                 }
                                                       
                                                                 thread_index = 7;
                                                                 if (active_thread[7] == 1) { /** instrPop6_0 **/
                                                                    pc_cs = pc[7] + guess_pc();
                                                                    assume((pc_cs >= pc[7]) && (pc_cs <= 14));
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
                                                       


