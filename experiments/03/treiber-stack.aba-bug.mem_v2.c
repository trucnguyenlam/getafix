#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NULL 0

typedef int val_t;
typedef int addr_t;


/**
 * Definitions:
 * address:  location on the memory  -  addr_t
 * pointer:  a pointer               -  ptr_t
 * value:    any value               -  val_t
 */



#define MEM_SIZE 25
// addr_t       MEM_ADDRESS[MEM_SIZE];
val_t        MEM_VALUE[MEM_SIZE];


/**
 * A structure can be represent as follows:
 *     (ignoring information about padding, we assume each field is given the same space)
 *     - Its address is the same as the first field
 *     - One field contribute to 1 (address shift) recursively
 */
#define  MEMORYSIZE      2            // 2 is enough here

#define  gstack          1
// #define  gstack_top      1
#define  nodepointers    2
#define  allocated       6

void __VERIFIER_atomic_init_MEMORY(void)
{
    MEM_VALUE[gstack]           = 0;
    MEM_VALUE[nodepointers + 0] = 0;
    MEM_VALUE[nodepointers + 1] = 0;
    MEM_VALUE[nodepointers + 2] = 0;
    MEM_VALUE[nodepointers + 3] = 0;
    MEM_VALUE[allocated]        = 0;
    MEM_VALUE[allocated + 1]    = 0;
}

addr_t __VERIFIER_atomic_malloc_Node() {
    int i = __VERIFIER_nondet_int();
    __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
    __VERIFIER_assume(!MEM_VALUE[allocated + i]);
    MEM_VALUE[allocated + i] = 1;
    return (nodepointers + i * 2);
}

addr_t malloc_Node() {
    return __VERIFIER_atomic_malloc_Node();
}

void free_Node(addr_t n) {
    int i = __VERIFIER_nondet_int();
    __VERIFIER_assume(nodepointers + i * 2 == n);
    MEM_VALUE[allocated + i] = 0; // Free the memory to witness the ABA bug
}

_Bool __VERIFIER_atomic_CAS(addr_t address_ptr, addr_t old_address, addr_t new_address)
{
    if (MEM_VALUE[address_ptr] == old_address)
    {
        MEM_VALUE[address_ptr] = new_address;
        return 1;
    }
    return 0;
}

// Push
#define oldtop_push_t0   8
void push_aux_t0(addr_t n) {
    // while (1) {
    MEM_VALUE[oldtop_push_t0] = MEM_VALUE[gstack];
    MEM_VALUE[n + 0] = MEM_VALUE[oldtop_push_t0];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_push_t0], n)) {
        return;
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_push_t0;
void Push_t0(int i) {
    n_push_t0 = malloc_Node();
    MEM_VALUE[n_push_t0 + 1] = i;
    push_aux_t0(n_push_t0);
}

#define oldtop_push_t1   9
void push_aux_t1(addr_t n) {
    // while (1) {
    MEM_VALUE[oldtop_push_t1] = MEM_VALUE[gstack];
    MEM_VALUE[n + 0] = MEM_VALUE[oldtop_push_t1];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_push_t1], n)) {
        return;
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_push_t1;
void Push_t1(int i) {
    n_push_t1 = malloc_Node();
    MEM_VALUE[n_push_t1 + 1] = i;
    push_aux_t1(n_push_t1);
}


#define oldtop_push_t2   10
// Push
void push_aux_t2(addr_t n) {
    // while (1) {
    MEM_VALUE[oldtop_push_t2] = MEM_VALUE[gstack];
    MEM_VALUE[n + 0] = MEM_VALUE[oldtop_push_t2];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_push_t2], n)) {
        return;
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_push_t2;
void Push_t2(int i) {
    n_push_t2 = malloc_Node();
    MEM_VALUE[n_push_t2 + 1] = i;
    push_aux_t2(n_push_t2);
}

// Pop

#define oldtop_pop_t0    11
#define next_pop_t0      12
addr_t pop_aux_t0() {
    // while (1) {
    MEM_VALUE[oldtop_pop_t0] = MEM_VALUE[gstack];
    if (MEM_VALUE[oldtop_pop_t0] == NULL) {
        return NULL;
    }
    MEM_VALUE[next_pop_t0] = MEM_VALUE[MEM_VALUE[oldtop_pop_t0]];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_pop_t0], MEM_VALUE[next_pop_t0])) {
        return MEM_VALUE[oldtop_pop_t0];
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_pop_t0;
int Pop_t0() {
    n_pop_t0 = pop_aux_t0();
    if (n_pop_t0 == NULL) return 2; // 2 means empty
    else {
        int v = MEM_VALUE[n_pop_t0 + 1];
        free_Node (n_pop_t0);
        return v;
    }
}

#define oldtop_pop_t1    13
#define next_pop_t1      14
addr_t pop_aux_t1() {
    // while (1) {
    MEM_VALUE[oldtop_pop_t1] = MEM_VALUE[gstack];
    if (MEM_VALUE[oldtop_pop_t1] == NULL) {
        return NULL;
    }
    MEM_VALUE[next_pop_t1] = MEM_VALUE[MEM_VALUE[oldtop_pop_t1]];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_pop_t1], MEM_VALUE[next_pop_t1])) {
        return MEM_VALUE[oldtop_pop_t1];
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_pop_t1;
int Pop_t1() {
    n_pop_t1 = pop_aux_t1();
    if (n_pop_t1 == NULL) return 2; // 2 means empty
    else {
        int v = MEM_VALUE[n_pop_t1 + 1];
        free_Node (n_pop_t1);
        return v;
    }
}

#define oldtop_pop_t2    15
#define next_pop_t2      16
addr_t pop_aux_t2() {
    // while (1) {
    MEM_VALUE[oldtop_pop_t2] = MEM_VALUE[gstack];
    if (MEM_VALUE[oldtop_pop_t2] == NULL) {
        return NULL;
    }
    MEM_VALUE[next_pop_t2] = MEM_VALUE[MEM_VALUE[oldtop_pop_t2]];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_pop_t2], MEM_VALUE[next_pop_t2])) {
        return MEM_VALUE[oldtop_pop_t2];
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_pop_t2;
int Pop_t2() {
    n_pop_t2 = pop_aux_t2();
    if (n_pop_t2 == NULL) return 2; // 2 means empty
    else {
        int v = MEM_VALUE[n_pop_t2 + 1];
        free_Node (n_pop_t2);
        return v;
    }
}

#define oldtop_pop_t3    17
#define next_pop_t3      18
addr_t pop_aux_t3() {
    // while (1) {
    MEM_VALUE[oldtop_pop_t3] = MEM_VALUE[gstack];
    if (MEM_VALUE[oldtop_pop_t3] == NULL) {
        return NULL;
    }
    MEM_VALUE[next_pop_t3] = MEM_VALUE[MEM_VALUE[oldtop_pop_t3]];
    if (__VERIFIER_atomic_CAS(gstack, MEM_VALUE[oldtop_pop_t3], MEM_VALUE[next_pop_t3])) {
        return MEM_VALUE[oldtop_pop_t3];
    }
    // }
    __VERIFIER_assume(0);
}

addr_t n_pop_t3;
int Pop_t3() {
    n_pop_t3 = pop_aux_t3();
    if (n_pop_t3 == NULL) return 2; // 2 means empty
    else {
        int v = MEM_VALUE[n_pop_t3 + 1];
        free_Node (n_pop_t3);
        return v;
    }
}


void Init() {
    __VERIFIER_atomic_init_MEMORY();
}

/**
 * Inject aba-bug
 */

#define PushArg0Max 2
int PushOpen[PushArg0Max];
int PushDone[PushArg0Max];
#define PopRetMax 3
int PopOpen;
int PopDone[PopRetMax];


void checkInvariant()
{
    assert (
        PopDone[0]  <= PushDone[0]  + PushOpen[0] &&
        PopDone[1]  <= PushDone[1]  + PushOpen[1]
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
    Push_t0(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush1(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push_t1(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush2(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push_t2(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPop3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop_t0();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop_t1();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop_t2();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop_t3();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    // 3 push
    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);

    //  4 pops
    pthread_create(&tid4, NULL, &instrPop3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
    pthread_create(&tid6, NULL, &instrPop5, NULL);
    pthread_create(&tid7, NULL, &instrPop6, NULL);
}
