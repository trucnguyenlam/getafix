#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

/**
 * Information about MEM version
 * Like the os simulating what it's called HEAP
 *
 */

/**
 * In every C program, variables will be on two parts of memory (AFAIK)
 * - STACK: storing global variable, pointer variables, ...
 * - HEAP: the area that is pointed to by pointer, and created by malloc
 *
 */


// int is general for value type
typedef val_t int;

#define MEM_SIZE 10

unsigned int MEM_ADDRESS[MEM_SIZE];
val_t        MEM_VALUE[MEM_SIZE];

/**
 * Initialize of HEAP
 */
void __VERIFIER_atomic_init_MEMORY(void)
{
}



/**
 * Presuming all the value in this program is int
 * @param  address [an address (a pointer)]
 * @return         [value at the address]
 */
int __VERIFIER_atomic_READ_PTR(unsigned int address)
{


}

/**
 * Write a value to pointer (points to)
 * @param address [a pointer]
 * @param value   []
 */
void __VERIFIER_atomic_WRITE_PTR(unsigned int address, val_t value)
{

}

/**
 * Return an address which can possibly hold enough space for malloc
 * @param  size [description]
 * @return      [description]
 */
unsigned int __VERIFIER_atomic_malloc(size_t size)
{




}

/**
 * Get address of a variable
 * @param  variable [can be a variable on STACK or variable on HEAP]
 * @return          [address of that variable]
 * Algorithm:
 *  - Determine the type of variable (syntatically)
 *  - If this
 */
unsigned int __VERIFIER_atomic_GET_ADDR(unsigned int variable)
{

}



// int __VERIFIER_atomic_CAS(void **address, void *old_value, void *new_value)
// {
//     if (*address == old_value) {
//         *address = new_value;
//         return 1;
//     }
//     return 0;
// }

// int CAS(void **address, void *old_value, void *new_value)
// {
//     return __VERIFIER_atomic_CAS(address, old_value, new_value);
// }

_Bool __VERIFIER_atomic_CAS(unsigned int address_ptr, unsigned int old_address, unsigned int new_address)
{
    if (MEM_VALUE[address_ptr] == old_address)
    {
        MEM_VALUE[address_ptr] = new_address;
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
 * A structure can be represent as follows:
 *     (ignoring information about padding, we assume each field is given the same space)
 *     - Its address is the same as the first field
 *     - One field contribute to 1 (address shift) recursively
 */

/**
 * Data structure
 */
// typedef struct Node Node;
// typedef struct Node {
//     Node *next;
//     int val;
// } Node;
// typedef struct Node
// {
//     unsigned int next;
//     int val;
// };
// sizeof(Node) = 2

// typedef struct Stack {
//     Node *top;
// } Stack;

// typedef struct Stack {
//     unsigned int top;
// } Stack;
// sizeof(Stack) = 1


// Global stack
// Stack gstack;
#define  gstack          1            // MEM_ADDRESS[gstack] = 1

#define  MEMORYSIZE 10            // Not quite relevant

// Node nodepointers[MEMORYSIZE];
#define nodepointers  2  //  MEM_ADDRESS[nodespointers] = 2
// nodepointers[0]  2    //  MEM_ADDRESS[nodespointers + 0] = 2
// nodepointers[1]  2 + sizeof(Node)    //  MEM_ADDRESS[nodespointers[1]] = 4
// nodepointers[2]  2 + 2*sizeof(Node)    //  MEM_ADDRESS[nodespointers[2]] = 6
//..
// nodepointers[9]  2 + 9*sizeof(Node)    //  MEM_ADDRESS[nodespointers[9]] = 20


// int allocated[MEMORYSIZE];    //
#define allocated  21    //  MEM_ADDRESS[node]
// allocated[0]  21
// ..
// allocated[9]  30


/**
 * Syntactically, we know how much we use for a stack? Is it true
 * So we statically assign address of the stack
 *
 *
 */
void __VERIFIER_atomic_init_STACK(void)
{
    /// What is it here
    MEM_VALUE[gstack + 0] = 0;   // 0 means NULL
}


// Node* __VERIFIER_atomic_malloc_Node() {
//     int i;
//     __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
//     __VERIFIER_assume(!allocated[i]);
//     allocated[i] = 1;
//     return &nodepointers[i];
// }
unsigned int __VERIFIER_atomic_malloc_Node() {
    int i;
    __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
    __VERIFIER_assume(!MEM_VALUE[allocated + i]);
    MEM_VALUE[allocated + i] = 1;
    return (nodepointers + i*2); // MEM_ADDRESS[nodepointers[i]]
}

// Node* malloc_Node() {
//     return __VERIFIER_atomic_malloc_Node();
// }

unsigned int malloc_Node() {
    return __VERIFIER_atomic_malloc_Node();
}

// void free_Node(Node* n) {
//     int i;
//     __VERIFIER_assume(&nodepointers[i] == n);
//     allocated[i] = 0; // Free the memory to witness the ABA bug
// }

void free_Node(unsigned int n) {
    int i;
    __VERIFIER_assume(nodepointers + i*2 == n);
    MEM_VALUE[allocated + i] = 0; // Free the memory to witness the ABA bug
}


// void push_aux(Node *n) {
//     Node *oldtop;
//     while (1) {
//         oldtop = gstack.top;
//         n->next = oldtop;
//         if (CAS(&gstack.top, oldtop, n)){
//             return;
//         }
//     }
// }

void push_aux(unsigned int n) {
    unsigned int oldtop = HEAP_OFFSET + 1;        // value(oldtop) = address
    while (1) {
        MEM_VALUE[oldtop] = MEM_VALUE[gstack + 0];   // MEM_ADDRESS[gstack + 0]
        MEM_VALUE[n + 0] = MEM_VALUE[oldtop];
        if (__VERIFIER_atomic_CAS(gstack + 0, MEM_VALUE[oldtop], MEM_VALUE[n])) {
            return;
        }
    }
}



// Node *pop_aux() {
//     Node *oldtop, *next;

//     while (1) {
//         oldtop = gstack.top;
//         if (oldtop == NULL) {
//             return NULL;
//         }
//         next = oldtop->next;
//         if (__VERIFIER_atomic_CAS(&gstack.top, oldtop, next)) {
//             return oldtop;
//         }
//     }
// }

unsigned int pop_aux() {
    unsigned int oldtop = HEAP_OFFSET + 2;
    unsigned int next = HEAP_OFFSET + 3;
    while (1) {
        MEM_VALUE[oldtop] = MEM_VALUE[gstack + 0];
        if (MEM_VALUE[oldtop] == NULL) {
            return NULL;
        }
        MEM_VALUE[next] = MEM_VALUE[oldtop + 0];
        if (__VERIFIER_atomic_CAS(gstack + 0, MEM_VALUE[oldtop], MEM_VALUE[next])) {
            return oldtop;
        }
    }
}

// void Push(int i) {
//     Node *n = malloc_Node();
//     n->val = i;
//     push_aux(n);
// }

void Push(int i) {
    unsigned int n = malloc_Node();
    MEM_VALUE[n + 1] = i;
    push_aux(n);
}

// int Pop() {
//     Node *n = pop_aux();
//     if (n == NULL) return 2; // 2 mean empty
//     else {
//         int v = n->val;
//         free_Node (n);
//         return v;
//     }
// }

int Pop() {
    unsigned int n = pop_aux();
    if (n == NULL) return 2; // 2 mean empty
    else {
        int v = MEM_VALUE[n + 1];
        free_Node (n);
        return v;
    }
}

void Init() {
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
