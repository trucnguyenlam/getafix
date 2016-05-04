#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int __VERIFIER_atomic_CAS(void **address, void *old_value, void *new_value)
{
    if (*address == old_value) {
        *address = new_value;
        return 1;
    }
    return 0;
}

// int CAS(void **address, void *old_value, void *new_value)
// {
//     return __VERIFIER_atomic_CAS(address, old_value, new_value);
// }

/**
 * Data structure
 */
typedef struct Node Node;
typedef struct Node {
    Node *next;
    int val;
} Node;

typedef struct Stack {
    Node *top;
} Stack;

// Global stack
Stack gstack;

#define MEMORYSIZE 2  // Original 10 but 2 is already enough

Node nodepointers[MEMORYSIZE];
int allocated[MEMORYSIZE];

Node* __VERIFIER_atomic_malloc_Node() {
    int i;
    __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
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
    allocated[i] = 0; // Free the memory to witness the ABA bug
}


void push_aux(Node *n) {
    Node *oldtop;
    // while (1) {
        oldtop = gstack.top;
        n->next = oldtop;
        if (__VERIFIER_atomic_CAS(&gstack.top, oldtop, n)){
            return;
        }
    // }
    __VERIFIER_assume(0);
}

Node *pop_aux() {
    Node *oldtop, *next;

    // while (1) {
        oldtop = gstack.top;
        if (oldtop == NULL){
            return NULL;
        }
        next = oldtop->next;
        if (__VERIFIER_atomic_CAS(&gstack.top, oldtop, next)) {
            return oldtop;
        }
    // }
    __VERIFIER_assume(0);
}

void Push(int i) {
    Node *n = malloc_Node();
    n->val = i;
    push_aux(n);
}

int Pop() {
    Node *n = pop_aux();
    if (n == NULL) {
        return 2; // 2 mean empty
    }
    else {
        int v = n->val;
        free_Node (n);
        return v;
    }
}

void Init() {
    // Do something
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
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush1(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush2(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPop3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);

    pthread_create(&tid4, NULL, &instrPop3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
    pthread_create(&tid6, NULL, &instrPop5, NULL);
    pthread_create(&tid7, NULL, &instrPop6, NULL);
}
