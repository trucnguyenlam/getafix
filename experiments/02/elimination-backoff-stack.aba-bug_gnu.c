#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>













#define LOCATION_ARRAY_SIZE 8
#define COLLISION_ARRAY_SIZE 1

#define POP 0
#define PUSH 1
#define TRUE 1
#define FALSE 0

typedef struct Cell Cell;
struct Cell {
    Cell        *pnext;
    int         pdata;
};

typedef struct ThreadInfo ThreadInfo;
struct ThreadInfo {
    unsigned int  id;
    int           op;
    Cell          cell;
};

typedef struct Simple_Stack Simple_Stack;
struct Simple_Stack {
    Cell *ptop;
};


Simple_Stack S;
ThreadInfo *location[LOCATION_ARRAY_SIZE];
int collision[COLLISION_ARRAY_SIZE];

int unique_id = 0;
void StackOp(ThreadInfo *p);
int TryPerformStackOp(ThreadInfo *p);
void __VERIFIER_atomic_FinishCollision(ThreadInfo *p);
int __VERIFIER_atomic_TryCollision(ThreadInfo *, ThreadInfo *, int);



/**
 * THREADS and STUFF
 */

#define NUM_THREADS 4      //
ThreadInfo threads[NUM_THREADS];
int allocated[NUM_THREADS];
pthread_mutex_t mutex;

ThreadInfo* __VERIFIER_atomic_malloc_ThreadInfo() {
    pthread_mutex_lock(&mutex);
    while (1)
    {
        for (int i = 0; i < NUM_THREADS; ++i) {
            if (!allocated[i]) {
                allocated[i] = 1;
                pthread_mutex_unlock(&mutex);
                return &threads[i];
            }
        }
    }
}
void __VERIFIER_atomic_free_ThreadInfo(ThreadInfo* ti) {
    pthread_mutex_lock(&mutex);
    while (1)
    {
        for (int i = 0; i < NUM_THREADS; ++i) {
            if (&threads[i] == ti) {
                allocated[i] = 0; // BUG Free the memory to witness the ABA bug
                pthread_mutex_unlock(&mutex);
                return;
            }
        }
    }
}

/**
 *  ALGORITHM
 */
void LesOP(ThreadInfo *p) {
    while (1) {
        int mypid = p->id;
        location[mypid] = p;
        int him = collision[0];   // GetPosition(p) = pos = 0;
        while (!__sync_bool_compare_and_swap(&collision[0], him, mypid)) {
            him = collision[0];
        }
        if (him > 0) {
            ThreadInfo* q = location[him];
            if (q != NULL && q->id == him && q->op != p->op) {
                if (__sync_bool_compare_and_swap(&location[mypid], p, NULL)) {
                    if (__VERIFIER_atomic_TryCollision(p, q, him) == TRUE) {
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
        // delay (p->spin);
        if (!__sync_bool_compare_and_swap(&location[mypid], p, NULL)) {
            __VERIFIER_atomic_FinishCollision(p);
            return;
        }
stack:
        if (TryPerformStackOp(p) == TRUE) {
            return;
        }
    }
}

int TryPerformStackOp(ThreadInfo * p) {
    Cell *phead, *pnext;
    if (p->op == PUSH) {
        phead = S.ptop;
        p->cell.pnext = phead;
        if (__sync_bool_compare_and_swap(&S.ptop, phead, &p->cell)) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    if (p->op == POP) {
        phead = S.ptop;
        if (phead == NULL) {
            // p->cell = EMPTY;  //Original code
            p->cell.pnext = 0; p->cell.pdata = 2;    // 2 means EMPTY
            return TRUE;
        }
        pnext = phead->pnext;
        if (__sync_bool_compare_and_swap(&S.ptop, phead, pnext)) {
            // p->cell.pdata = phead->pdata;  //Injected code
            p->cell = *phead;
            // Injected code
            while (1) {
                for (int i = 0; i < NUM_THREADS; ++i) {
                    ThreadInfo *oldti = &threads[i];
                    if (&oldti->cell == phead) {
                        __VERIFIER_atomic_free_ThreadInfo(oldti);
                        goto EXIT;
                    }
                }
            }
EXIT:
            // END: Injected code
            return TRUE;
        }
        else {
            // p->cell = EMPTY;
            p->cell.pnext = 0; p->cell.pdata = 2;    // 2 means EMPTY
            return FALSE;
        }
    }
}


void __VERIFIER_atomic_FinishCollision(ThreadInfo * p) {
    pthread_mutex_lock(&mutex);
    if (p->op == POP) {
        int mypid = p->id;
        p->cell = location[mypid]->cell;
        location[mypid] = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

int __VERIFIER_atomic_TryCollision(ThreadInfo * p, ThreadInfo * q, int him) {
    pthread_mutex_lock(&mutex);
    int mypid = p->id;
    if (p->op == PUSH) {
        if (__sync_bool_compare_and_swap(&location[him], q, p)) {
            pthread_mutex_unlock(&mutex);
            return TRUE;
        }
        else {
            pthread_mutex_unlock(&mutex);
            return FALSE;
        }
    }
    if (p->op == POP) {
        if (__sync_bool_compare_and_swap(&location[him], q, NULL)) {
            p->cell = q->cell;
            location[mypid] = NULL;
            pthread_mutex_unlock(&mutex);
            return TRUE;
        }
        else {
            pthread_mutex_unlock(&mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&mutex);
    return FALSE;
}


/**
 * Injected code
 */

void Init() {
    S.ptop = NULL;
}

// Derive from
// void StackOp (ThreadInfo * pInfo) {
//     if (TryPerformStackOp (p) == FALSE)
//         LesOP (p);
//     return;
// }

void Push(int x) {
    ThreadInfo *ti = __VERIFIER_atomic_malloc_ThreadInfo();

    // Initialize threads
    ti->id = ++unique_id;
    ti->op = PUSH;
    ti->cell.pdata = x;

    if (TryPerformStackOp(ti) == FALSE) {
        LesOP(ti);
    }
}

int Pop() {
    ThreadInfo *ti = __VERIFIER_atomic_malloc_ThreadInfo();
    ti->id = ++unique_id;
    ti->op = POP;

    if (TryPerformStackOp(ti) == FALSE) {
        LesOP(ti);
    }
    int v = ti->cell.pdata;
    __VERIFIER_atomic_free_ThreadInfo(ti);
    return v;
}

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
    pthread_mutex_lock(&mutex);
    PushOpen[localPush1]++;
    pthread_mutex_unlock(&mutex);
}

void __VERIFIER_atomic_atomicDecrIncr_Push(int localPush1) {
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

void __VERIFIER_atomic_atomicDecrIncr_Pop(int localPop_ret) {
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


int main(void) {
    Init();
    pthread_mutex_init(&mutex, 0);
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);

    pthread_create(&tid4, NULL, &instrPop3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
    pthread_create(&tid6, NULL, &instrPop5, NULL);
    pthread_create(&tid7, NULL, &instrPop6, NULL);
}
