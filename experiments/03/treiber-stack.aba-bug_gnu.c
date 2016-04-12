#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>














/**
 * Data structure
 */
typedef struct Node Node;
struct Node {
    Node *next;
    int val;
};

typedef struct Stack {
    Node *top;
} Stack;

// Global stack
Stack gstack;
pthread_mutex_t mutex;
#define MEMORYSIZE 10

Node nodepointers[MEMORYSIZE];
char allocated[MEMORYSIZE];

Node* __VERIFIER_atomic_malloc_Node() {
    pthread_mutex_lock(&mutex);
    while (1)
    {
        for (int i = 0; i < MEMORYSIZE; ++i)
        {
            if (!allocated[i])
            {
                allocated[i] = 1;
                pthread_mutex_unlock(&mutex);
                return &nodepointers[i];
            }
        }
    }
}

Node* malloc_Node() {
    return __VERIFIER_atomic_malloc_Node();
}

void free_Node(Node* n) {
    pthread_mutex_lock(&mutex);
    while (1)
    {
        for (int i = 0; i < MEMORYSIZE; ++i)
        {
            if (&nodepointers[i] == n)
            {
                allocated[i] = 0; // Free the memory to witness the ABA bug
                pthread_mutex_unlock(&mutex);
                return;
            }
        }
    }
}


void push_aux(Node *n) {
    Node *oldtop;
    while (1) {
        oldtop = gstack.top;
        n->next = oldtop;
        if (__sync_bool_compare_and_swap(&gstack.top, oldtop, n)){
            return;
        }
    }
}

Node *pop_aux() {
    Node *oldtop, *next;

    while (1) {
        oldtop = gstack.top;
        if (oldtop == NULL)
            return NULL;
        next = oldtop->next;
        if (__sync_bool_compare_and_swap(&gstack.top, oldtop, next)) {
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
    if (n == NULL) return 2; // 2 mean empty
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

char PushOpen[PushArg0Max];
char PushDone[PushArg0Max];

#define PopRetMax 3
char PopOpen;
char PopDone[PopRetMax];


void checkInvariant()
{
    assert (
        PopDone[0]  <= PushDone[0]  + PushOpen[0] &&
        PopDone[1]  <= PushDone[1]  + PushOpen[1]
    );
}

void __VERIFIER_atomic_atomicIncr_Push(char localPush1) {
    pthread_mutex_lock(&mutex);
    PushOpen[localPush1]++;
    pthread_mutex_unlock(&mutex);
}

void __VERIFIER_atomic_atomicDecrIncr_Push(char localPush1) {
    pthread_mutex_lock(&mutex);
    PushOpen[localPush1]--;
    PushDone[localPush1]++;
    checkInvariant();
    pthread_mutex_unlock(&mutex);
}

void __VERIFIER_atomic_atomicIncr_Pop() {
    pthread_mutex_lock(&mutex);
    PopOpen++;
    pthread_mutex_unlock(&mutex);
}

void __VERIFIER_atomic_atomicDecrIncr_Pop(char localPop_ret) {
    pthread_mutex_lock(&mutex);
    PopOpen--;
    PopDone[localPop_ret]++;
    checkInvariant();
    pthread_mutex_unlock(&mutex);
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
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;
    pthread_mutex_init(&mutex, 0);
    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);
    pthread_create(&tid4, NULL, &instrPop3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
    pthread_create(&tid6, NULL, &instrPop5, NULL);
    pthread_create(&tid7, NULL, &instrPop6, NULL);
}
