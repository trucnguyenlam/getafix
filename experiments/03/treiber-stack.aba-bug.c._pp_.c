# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c"
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
# 2 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/stdio.h" 1
# 3 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/pthread.h" 1
# 4 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c" 2

int __VERIFIER_atomic_CAS(void **address, void *old_value, void *new_value)
{
    if (*address == old_value) {
        *address = new_value;
        return 1;
    }
    return 0;
}

int CAS(void **address, void *old_value, void *new_value)
{
    return __VERIFIER_atomic_CAS(address, old_value, new_value);
}




typedef struct Node Node;
typedef struct Node {
    Node *next;
    int val;
} Node;

typedef struct Stack {
    Node *top;
} Stack;


Stack gstack;



Node nodepointers[1];
int allocated[1];

Node* __VERIFIER_atomic_malloc_Node() {
    int i;
    __VERIFIER_assume(0 <= i && i < 1);
    __VERIFIER_assume(!allocated[i]);
    allocated[i] = 1;
    return &nodepointers[i];
}

Node* malloc_Node() {
    return __VERIFIER_atomic_malloc_Node();
}

void free_Node(Node* n) {
    int i;
    __VERIFIER_assume(&nodepointers[i] == n);
    allocated[i] = 0;
}


void push_aux(Node *n) {
    Node *oldtop;
    while (1) {
        oldtop = gstack.top;
        n->next = oldtop;
        if (CAS(&gstack.top, oldtop, n)){
            return;
        }
    }
}

Node *pop_aux() {
    Node *oldtop, *next;

    while (1) {
        oldtop = gstack.top;
        if (oldtop == 0)
            return 0;
        next = oldtop->next;
        if (CAS(&gstack.top, oldtop, next)) {
            return oldtop;
        }
    }
}

void Push(int i) {
    Node *n = malloc_Node();
    n->val = i;
    push_aux(n);
}

int Pop() {
    Node *n = pop_aux();
    if (n == 0) return 2;
    else {
        int v = n->val;
        free_Node (n);
        return v;
    }
}

void Init() {

}
# 111 "/home/trucnguyenlam/Development/getafix/getafix-concurrent/experiments/03/treiber-stack.aba-bug.c"
int PushOpen[2];
int PushDone[2];


int PopOpen;
int PopDone[3];


void checkInvariant()
{
    assert (
        PopDone[0] <= PushDone[0] + PushOpen[0] &&
        PopDone[1] <= PushDone[1] + PushOpen[1]
    );
}

void __VERIFIER_atomic_atomicIncr_Push(int localPush1) {
    PushOpen[localPush1]++;
}

void __VERIFIER_atomic_atomicDecrIncr_Push(int localPush1) {
    PushOpen[localPush1]--;
    PushDone[localPush1]++;
    checkInvariant();
}

void __VERIFIER_atomic_atomicIncr_Pop() {
    PopOpen++;
}

void __VERIFIER_atomic_atomicDecrIncr_Pop(int localPop_ret) {
    PopOpen--;
    PopDone[localPop_ret]++;
    checkInvariant();
}

void* instrPush0(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return 0;
}

void* instrPush1(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return 0;
}

void* instrPush2(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return 0;
}

void* instrPop3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    pthread_create(&tid1, 0, &instrPush0, 0);
    pthread_create(&tid2, 0, &instrPush1, 0);
    pthread_create(&tid3, 0, &instrPush2, 0);

    pthread_create(&tid4, 0, &instrPop3, 0);
    pthread_create(&tid5, 0, &instrPop4, 0);
    pthread_create(&tid6, 0, &instrPop5, 0);
    pthread_create(&tid7, 0, &instrPop6, 0);
}
