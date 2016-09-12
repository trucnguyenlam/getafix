# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
# 1 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 1
# 1 "./preprocessor/ccpp-0.1/fake_include/_fake_defines.h" 1
# 2 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/_fake_typedefs.h" 1



typedef int _____STARTSTRIPPINGFROMHERE_____;

typedef int __cs_t;
typedef int __cs_attr_t;
typedef int __cs_mutex_t;
typedef int __cs_mutexattr_t;
typedef int __cs_cond_t;
typedef int __cs_condattr_t;
typedef int __cs_key_t;
typedef int __cs_once_t;
typedef int __cs_rwlock_t;
typedef int __cs_rwlockattr_t;
typedef int __cs_spinlock_t;
typedef int __cs_barrier_t;
typedef int __cs_barrierattr_t;

typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;


typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;


typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;


typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;


typedef int intptr_t;
typedef int uintptr_t;


typedef int intmax_t;
typedef int uintmax_t;


typedef _Bool bool;

typedef int va_list;

typedef int fd_set;

typedef int _____STOPSTRIPPINGFROMHERE_____;
# 2 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 2
# 2 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/stdio.h" 1
# 3 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/pthread.h" 1
# 4 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/assert.h" 1
# 5 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c" 2
# 25 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
typedef struct Cell Cell;
struct Cell {
    Cell *pnext;
    int pdata;
};

typedef struct ThreadInfo ThreadInfo;
struct ThreadInfo {
    unsigned int id;
    int op;
    Cell cell;
};

typedef struct Simple_Stack Simple_Stack;
struct Simple_Stack {
    Cell *ptop;
};


Simple_Stack S;
ThreadInfo *location[2];
int collision;


void StackOp(ThreadInfo *p);
int TryPerformStackOp(ThreadInfo *p);

int __VERIFIER_atomicint_cas(int *p, int cmp, int new) { if (*p == cmp) { *p = new; return 1; } else return 0; }
int __VERIFIER_atomicti_cas(ThreadInfo* *p, ThreadInfo* cmp, ThreadInfo* new) { if (*p == cmp) { *p = new; return 1; } else return 0; }
int __VERIFIER_atomicc_cas(Cell* *p, Cell* cmp, Cell* new) { if (*p == cmp) { *p = new; return 1; } else return 0; }





int taken_ids[2];

int __VERIFIER_atomic_chooseUniqueId() {
    int i = nondet_int();
    __VERIFIER_assume(1 <= i && i <= 2);
    __VERIFIER_assume(!taken_ids[i - 1]);
    taken_ids[i - 1] = 1;
    return i;
}
# 88 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
void LesOP(ThreadInfo *p) {
    p->id = __VERIFIER_atomic_chooseUniqueId();

        int mypid = p->id;
        location[mypid - 1] = p;
        int him = collision;

        __VERIFIER_assume (__VERIFIER_atomicint_cas(&collision,him,mypid));

        if (him > 0) {
            ThreadInfo* q = location[him - 1];
            if (q != 0 && q->id == him && q->op != p->op) {
                if (__VERIFIER_atomicti_cas(&location[mypid - 1],p,0)) {
                    if (__VERIFIER_atomic_TryCollision(p, q, him)) {
                        return;
                    } else {
                        goto stack;
                    }
                }
                else {
                    __VERIFIER_atomic_FinishCollision(p);
                    return;
                }
            }
        }

        if (!__VERIFIER_atomicti_cas(&location[mypid - 1],p,0)) {
            __VERIFIER_atomic_FinishCollision(p);
            return;
        }
stack:
        if (TryPerformStackOp(p) == 1) {
            return;
        }

    __VERIFIER_assume(0);
}

int TryPerformStackOp(ThreadInfo* p) {
    Cell *thead, *tnext;
    if (p->op == 1) {
        thead = S.ptop;
        p->cell.pnext = thead;
        return __VERIFIER_atomicc_cas(&S.ptop,thead,&p->cell);
    }




    if (p->op == 0) {
        thead = S.ptop;
        if (thead == 0) {

            p->cell.pnext = 0; p->cell.pdata = 2;
            return 1;
        }
        tnext = thead->pnext;
        if (__VERIFIER_atomicc_cas(&S.ptop,thead,tnext)) {

            p->cell = *thead;
# 157 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
            return 1;
        }
        else {

            p->cell.pnext = 0; p->cell.pdata = 2;
            return 0;
        }
    }
}

void __VERIFIER_atomic_FinishCollision(ThreadInfo *p) {
    if (p->op == 0) {
    int mypid = p->id;


        location[mypid - 1] = 0;
    }
}

int __VERIFIER_atomic_TryCollision(ThreadInfo *p, ThreadInfo *q, int him) {
    int mypid = p->id;
    if (p->op == 1) {
        return __VERIFIER_atomicti_cas(&location[him - 1],q,p);
    }




    if (p->op == 0) {
        if (__VERIFIER_atomicti_cas(&location[him - 1],q,0)) {


            location[mypid - 1] = 0;
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}






void Init() {
    S.ptop = 0;
}
# 215 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/02/elimination-backoff-stack.empty-bug.c"
void Push(int x) {
    ThreadInfo *ti = malloc(sizeof(*ti));

    ti->op = 1;
    ti->cell.pdata = x;

    if (!TryPerformStackOp(ti)) {
        LesOP(ti);
    }
}



int Pop() {
    ThreadInfo *ti = malloc(sizeof(*ti));

    ti->op = 0;

    if (!TryPerformStackOp(ti)) {
        LesOP(ti);
    }

    int v = ti->cell.pdata;
    return v;
}



int tick = 0;
int tickBound = 1;

int PushOpen[2][2];
int PushDone[2][2][2];


int PopOpen[2];
int PopDone[2][2][3];

void checkInvariant() {
    assert
    (
        (PopDone[1][1][2] == 0 &&
         PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] +
         PopDone[0][1][0] >= 0 &&
         PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] >= 0 &&

         PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] >= 0 &&
         PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
         PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] >= 0 &&
         PopOpen[1] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
         PopOpen[1] >= 0 &&
         PopOpen[0] + PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] >= 0 &&
         PopOpen[0] >= 0 &&
         PopDone[0][1][2] >= 0 &&
         PopDone[0][0][2] >= 0 &&
         PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] - PopDone[0][1][1] >= PopDone[1][1][1] &&
         PopDone[1][1][1] >= 0 &&
         PopDone[0][1][1] >= 0 &&
         PushOpen[0][1] + PushDone[0][1][1] + PushDone[0][0][1] >= PopDone[0][0][1] &&
         PopDone[0][0][1] >= 0 &&
         PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] - PopDone[0][1][0] >= PopDone[1][1][0] &&
         PopDone[1][1][0] >= 0 &&
         PopDone[0][1][0] >= 0 &&
         PushOpen[0][0] + PushDone[0][1][0] + PushDone[0][0][0] >= PopDone[0][0][0] &&
         PopDone[0][0][0] >= 0 &&
         PushOpen[1][1] >= 0 &&
         PushOpen[0][1] >= 0 &&
         PushDone[1][1][1] >= 0 &&
         PushDone[0][1][1] >= 0 &&
         PushDone[0][0][1] >= 0 &&
         PushOpen[1][0] >= 0 &&
         PushOpen[0][0] >= 0 &&
         PushDone[1][1][0] >= 0 &&
         PushDone[0][1][0] >= 0 &&
         PushDone[0][0][0] >= 0) || (PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + -1 * PushDone[0][0][1] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] + PushDone[0][1][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + PushOpen[0][1] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + 2 * PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + -1 * PushDone[0][0][1] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] + PushDone[0][1][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] + PopDone[1][1][0] + PopDone[0][1][0] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[0][0][1] - PushDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + PushOpen[0][1] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PopDone[1][1][0] + PopDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] + PushOpen[0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][1] + PopDone[0][1][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PushDone[0][0][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PushDone[0][0][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] + PushOpen[0][0] + PushDone[0][1][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] + PushOpen[0][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] + PushOpen[0][0] + PushDone[0][1][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] + PushOpen[0][0] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PopDone[0][0][0] - PushDone[0][0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] + PushOpen[0][0] >= 0 &&
                                     PopOpen[1] + PopOpen[0] + PopDone[1][1][0] + PopDone[0][1][0] >= 0 &&
                                     PopOpen[1] + PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] >= 0 &&
                                     PopOpen[1] + PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] >= 0 &&
                                     PopOpen[1] >= 0 &&
                                     PopOpen[0] + PushOpen[1][0] + 2 * PushOpen[0][0] + PushDone[1][1][0] + 2 * PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] >= 0 &&
                                     PopOpen[0] + PushOpen[1][0] + 2 * PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] >= 0 &&
                                     PopOpen[0] + PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] >= 0 &&
                                     PopOpen[0] + PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] >= 0 &&
                                     PopOpen[0] >= 0 &&
                                     PopDone[1][1][2] >= 1 &&
                                     PopDone[0][1][2] >= 0 &&
                                     PopDone[0][0][2] >= 0 &&
                                     PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] - PopDone[0][0][1] - PopDone[0][1][1] >= PopDone[1][1][1] &&
                                     PopDone[1][1][1] >= 0 &&
                                     PopDone[0][1][1] >= 0 &&
                                     PushOpen[1][1] + 2 * PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] + PushDone[0][0][1] >= PopDone[0][0][1] &&
                                     PushOpen[0][1] + PushDone[0][1][1] + PushDone[0][0][1] >= PopDone[0][0][1] &&
                                     PopDone[0][0][1] >= 0 &&
                                     PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] - PopDone[0][0][0] - PopDone[0][1][0] >= PopDone[1][1][0] &&
                                     PopDone[1][1][0] >= 0 &&
                                     PopDone[0][1][0] >= 0 &&
                                     PushOpen[1][0] + 2 * PushOpen[0][0] + PushDone[1][1][0] + 2 * PushDone[0][1][0] + PushDone[0][0][0] >= PopDone[0][0][0] &&
                                     PushOpen[1][0] + 2 * PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] + PushDone[0][0][0] >= PopDone[0][0][0] &&
                                     PushOpen[0][0] + PushDone[0][1][0] + PushDone[0][0][0] >= PopDone[0][0][0] &&
                                     PopDone[0][0][0] >= 0 &&
                                     PushOpen[1][1] + PushOpen[0][1] + PushDone[1][1][1] + PushDone[0][1][1] >= 0 &&
                                     PushOpen[1][1] >= 0 &&
                                     PushOpen[0][1] >= 0 &&
                                     PushDone[1][1][1] >= 0 &&
                                     PushDone[0][1][1] >= 0 &&
                                     PushDone[0][0][1] >= 0 &&
                                     PushOpen[1][0] + PushOpen[0][0] + PushDone[1][1][0] + PushDone[0][1][0] >= 0 &&
                                     PushOpen[1][0] >= 0 &&
                                     PushOpen[0][0] >= 0 &&
                                     PushDone[1][1][0] >= 0 &&
                                     PushDone[0][1][0] >= 0 &&
                                     PushDone[0][0][0] >= 0)
    );
}

void __VERIFIER_atomic_atomicIncr_Push(int tickStart, int localPush1) {
    PushOpen[tickStart][localPush1]++;
}

void __VERIFIER_atomic_atomicDecrIncr_Push(int tickStart, int tick, int localPush1) {
    PushOpen[tickStart][localPush1]--;
    PushDone[tickStart][tick][localPush1]++;
    checkInvariant();
}

void __VERIFIER_atomic_atomicIncr_Pop(int tickStart) {
    PopOpen[tickStart]++;
}

void __VERIFIER_atomic_atomicDecrIncr_Pop(int tickStart, int tick, int localPop_ret) {
    PopOpen[tickStart]--;
    PopDone[tickStart][tick][localPop_ret]++;
    checkInvariant();
}

void* instrPush0(void* unused) {
    int tickStart = tick;
    __VERIFIER_atomic_atomicIncr_Push(tickStart, 1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(tickStart, tick, 1);
    return 0;
}

void* instrPush1(void* unused) {
    int tickStart = tick;
    __VERIFIER_atomic_atomicIncr_Push(tickStart, 1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(tickStart, tick, 1);
    return 0;
}

void* instrPush2(void* unused) {
    int tickStart = tick;
    __VERIFIER_atomic_atomicIncr_Push(tickStart, 1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(tickStart, tick, 1);
    return 0;
}

void* instrPop3(void* unused) {
    int tickStart = tick;
    __VERIFIER_atomic_atomicIncr_Pop(tickStart);
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(tickStart, tick, localPop_ret);
    return 0;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4;

    pthread_create(&tid1, 0, &instrPush0, 0);
    pthread_create(&tid2, 0, &instrPush1, 0);
    pthread_create(&tid3, 0, &instrPush2, 0);
    pthread_create(&tid4, 0, &instrPop3, 0);
    while (tick < tickBound) {
        tick++;
    }
}
