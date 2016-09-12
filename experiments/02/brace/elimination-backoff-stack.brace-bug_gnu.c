#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>






#define LOCATION_ARRAY_SIZE 2
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
    int          op;
    Cell          cell;
};

typedef struct Simple_Stack Simple_Stack;
struct Simple_Stack {
    Cell *ptop;
};


Simple_Stack S;
ThreadInfo *location[LOCATION_ARRAY_SIZE];
int collision;

// int unique_id = 0;
void StackOp(ThreadInfo *p);
int TryPerformStackOp(ThreadInfo *p);
void __VERIFIER_atomic_FinishCollision(ThreadInfo *p);
int __VERIFIER_atomic_TryCollision(ThreadInfo *, ThreadInfo *, int);


/**
 * THREADS and STUFF
 */

int taken_ids[LOCATION_ARRAY_SIZE];
pthread_mutex_t mutex;
int __VERIFIER_atomic_chooseUniqueId() {
    int i;
    pthread_mutex_lock(&mutex);
    while (1) {
        for (i = 1; i <= LOCATION_ARRAY_SIZE; ++i) {
            if (!taken_ids[i - 1]) {
                taken_ids[i - 1] = 1;
                pthread_mutex_unlock(&mutex);
                return i;
            }
        }
    }
}
















/**
 * ALGORITHM
 */

void LesOP(ThreadInfo *p) {
    p->id = __VERIFIER_atomic_chooseUniqueId();
    while (1) {
        int mypid = p->id;
        location[mypid - 1] = p;
        int him = collision;
        while (!__sync_bool_compare_and_swap(&collision, him, mypid)) {
            him = collision;
        }
        if (him > 0) {
            ThreadInfo* q = location[him - 1];

            if (q != NULL && q->id == him && q->op != p->op) {
                if (__sync_bool_compare_and_swap(&location[mypid - 1], p, NULL)) {
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

        if (!__sync_bool_compare_and_swap(&location[mypid - 1], p, NULL)) {
            __VERIFIER_atomic_FinishCollision(p);
            return;
        }
stack:
//                     if (TryPerformStackOp(p) == 1)
//                         return;
        TryPerformStackOp(p);
        return;
        // BUG: returns regardless (corresponds to brace bug)
    }
}

int TryPerformStackOp(ThreadInfo* p) {
    Cell *thead, *tnext;
    if (p->op == PUSH) {
        thead = S.ptop;
        p->cell.pnext = thead;
        return __sync_bool_compare_and_swap(&S.ptop, thead, &p->cell);
    }





    if (p->op == POP) {
        thead = S.ptop;
        if (thead == NULL) {
            // p->cell = EMPTY;  //Original code
            p->cell.pnext = 0; p->cell.pdata = 2;    // 2 means EMPTY
            return TRUE;
        }
        tnext = thead->pnext;
        if (__sync_bool_compare_and_swap(&S.ptop, thead, tnext)) {
            // p->cell.pdata = thead->pdata;  // Jad Injected code
            p->cell = *thead;

            return TRUE;
        }
        else {
            // p->cell = EMPTY;
            p->cell.pnext = 0; p->cell.pdata = 2;    // 2 means EMPTY
            return FALSE;
        }
    }
}


void __VERIFIER_atomic_FinishCollision(ThreadInfo *p) {
    pthread_mutex_lock(&mutex);
    int mypid = p->id;
    if (p->op == POP) {
        p->cell = location[mypid - 1]->cell;
        location[mypid - 1] = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

int __VERIFIER_atomic_TryCollision(ThreadInfo *p, ThreadInfo *q, int him) {
    pthread_mutex_lock(&mutex);
    int mypid = p->id;

    if (p->op == PUSH) {
        pthread_mutex_unlock(&mutex);
        return __sync_bool_compare_and_swap(&location[him - 1], q, p);
    }
    if (p->op == POP) {
        if (__sync_bool_compare_and_swap(&location[him - 1], q, NULL)) {
            p->cell = q->cell;
            location[mypid - 1] = NULL;
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


void Push(int x) {
    ThreadInfo *ti = malloc(sizeof(*ti));
    // __VERIFIER_assume(ti);

    ti->op = PUSH;
    ti->cell.pdata = x;

    if (!TryPerformStackOp(ti)) {
        LesOP(ti);
    }
}

int Pop() {
    ThreadInfo *ti = malloc(sizeof(*ti));
    // __VERIFIER_assume(ti);

    ti->op = POP;

    if (!TryPerformStackOp(ti)) {
        LesOP(ti);
    }

    int v = ti->cell.pdata;
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
    pthread_mutex_unlock(&mutex);
    checkInvariant();
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
    pthread_mutex_unlock(&mutex);
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

void* instrPush3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    int localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}



int main() {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_mutex_init(&mutex, 0);
    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);
    pthread_create(&tid4, NULL, &instrPush3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
}

