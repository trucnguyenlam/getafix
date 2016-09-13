#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define CASDEF(t,ty) \
    int __VERIFIER_atomic_ ## t ## _cas(ty *p, ty cmp, ty new) { \
        if (*p == cmp) { \
            *p = new; \
            return 1; \
        } \
        else \
            return 0; \
    }
#define CAS(t,x,y,z) __VERIFIER_atomic_ ## t ## _cas(x,y,z)

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

CASDEF(int, int)
CASDEF(ti, ThreadInfo*)
CASDEF(c, Cell*)

/**
 * THREADS and STUFF
 */

int taken_ids[LOCATION_ARRAY_SIZE];

int __VERIFIER_atomic_chooseUniqueId() {
    int i = nondet_int();
    __VERIFIER_assume(1 <= i && i <= LOCATION_ARRAY_SIZE);
    __VERIFIER_assume(!taken_ids[i - 1]);
    taken_ids[i - 1] = 1;
    return i;
}
















/**
 * ALGORITHM
 */

void LesOP(ThreadInfo *p) {
    p->id = __VERIFIER_atomic_chooseUniqueId();
    {
        int mypid = p->id;
        location[mypid - 1] = p;
        int him = collision;

        __VERIFIER_assume (CAS(int, &collision, him, mypid));

        if (him > 0) {
            ThreadInfo* q = location[him - 1];

            if (q != NULL && q->id == him && q->op != p->op) {
                if (CAS(ti, &location[mypid - 1], p, NULL)) {
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

        if (!CAS(ti, &location[mypid - 1], p, NULL)) {
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
    __VERIFIER_assume(0); // you can only return in the first loop iteration
}

int TryPerformStackOp(ThreadInfo* p) {
    Cell *thead, *tnext;
    if (p->op == PUSH) {
        thead = S.ptop;
        p->cell.pnext = thead;
        return CAS(c, &S.ptop, thead, &p->cell);
    }





    if (p->op == POP) {
        thead = S.ptop;
        if (thead == NULL) {
            // p->cell = EMPTY;  //Original code
            p->cell.pnext = 0; p->cell.pdata = 2;    // 2 means EMPTY
            return TRUE;
        }
        tnext = thead->pnext;
        if (CAS(c, &S.ptop, thead, tnext)) {
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
    int mypid = p->id;
    if (p->op == POP) {
        p->cell = location[mypid - 1]->cell;
        location[mypid - 1] = NULL;
    }
}

int __VERIFIER_atomic_TryCollision(ThreadInfo *p, ThreadInfo *q, int him) {
    int mypid = p->id;

    if (p->op == PUSH) {
        return CAS(ti, &location[him - 1], q, p);
    }
    if (p->op == POP) {
        if (CAS(ti, &location[him - 1], q, NULL)) {
            p->cell = q->cell;
            location[mypid - 1] = NULL;
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
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

    if (!TryPerformStackOp(ti))
        LesOP(ti);
}

int Pop() {
    ThreadInfo *ti = malloc(sizeof(*ti));
    // __VERIFIER_assume(ti);

    ti->op = POP;

    if (!TryPerformStackOp(ti))
        LesOP(ti);

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

    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);
    pthread_create(&tid4, NULL, &instrPush3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
}

