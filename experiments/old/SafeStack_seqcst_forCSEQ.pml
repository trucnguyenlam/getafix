// #define NUM_THREADS 3





// typedef struct SafeStackItem
// {
//     volatile int Value;
//     int Next;
// } SafeStackItem;

// typedef struct SafeStack
// {
//     SafeStackItem* array;
//     int head;
//     int count;
// } SafeStack;

// pthread_t threads[NUM_THREADS];

// /// Global
// SafeStack stack;

typedef SafeStack {
    int arrayValue[3];
    int arrayNext[3];
    int head;
    int count;
}

SafeStack stack;


/*void __VERIFIER_atomic_store(int *obj, int v)
{
    *obj = v;
}

int __VERIFIER_atomic_load(int *obj)
{
    return *obj;
}

int __VERIFIER_atomic_exchange(int *obj, int v)
{
    int t = *obj;
    *obj = v;
    return t;
}*/

/*_Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
{
    if(*obj == *expected)
    {
        *obj = desired;
        return 1;
    }
    else
    {
        *expected = *obj;
        return 0;
    }
}*/

/*int __VERIFIER_atomic_fetch_add(int * obj, int v)
{
    int old = *obj;
    *obj = old + v;
    return old;
}

int __VERIFIER_atomic_fetch_sub(int * obj, int v)
{
    int old = *obj;
    *obj = old - v;
    return old;
}
*/
/*void Init(int pushCount)
{
    int i;
    stack.array = malloc(3 * sizeof(SafeStackItem));
    __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(&stack.head, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        __VERIFIER_atomic_store(&stack.array[i].Next, i + 1);
    }
    __VERIFIER_atomic_store(&stack.array[pushCount - 1].Next, -1);
}
*/
/*void Destroy(void)
{
    free(stack.array);
}

int Pop(void)
{
    while (__VERIFIER_atomic_load(&stack.count) > 1)
    {
        int head1 = __VERIFIER_atomic_load(&stack.head);
        int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
            {
                __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                return head1;
            }
            else
            {
                __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
            }
        }
    }

    return -1;
}*/

/*void Push(int index)
{
    int head1 = __VERIFIER_atomic_load(&stack.head);
    do
    {
        __VERIFIER_atomic_store(&stack.array[index].Next, head1);

    } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
    __VERIFIER_atomic_fetch_add(&stack.count, 1);
}*/


proctype thread0()
{
    byte i;

    for (i : 0 .. 2) {
        int elem;
        do
        ::  {{
                // elem = Pop();
                // int Pop(void)
                // {
                do
                :: (stack.count > 1) ->
                    {
                        int head1 = stack.head;
                        int next1;
                        atomic {
                            next1 = stack.arrayNext[head1];
                            stack.arrayNext[head1] = -1;
                        };

                        if
                        :: (next1 >= 0) ->
                            {
                                int head2 = head1;
                                bool ifCond1;
                                atomic {
                                    // _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
                                    if
                                    ::  (stack.head == head2) ->
                                        {
                                            stack.head = next1;
                                            ifCond1 =  true;
                                        }
                                    ::  else ->
                                        {
                                            head2 = stack.head;
                                            ifCond1 = false;
                                        }
                                    fi;
                                };
                                if
                                :: ifCond1 ->
                                    {
                                        stack.count--;
                                        elem = head1;
                                        goto exitPop;
                                    }
                                :: else ->
                                    {
                                        atomic {
                                            stack.arrayNext[head1] = next1;
                                        };
                                    }
                                fi;
                            }
                        fi;
                    }
                od
                elem = -1;
                goto exitPop;
                // }
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        stack.arrayValue[elem] = 0;
        assert((stack.arrayValue[elem] == 0));

        /*Push(elem);*/
        // void Push(int index)
        {
            int head1 = stack.head;
            do
            :: true -> {
                stack.arrayNext[elem] = head1;

                bool ifCond2;
                atomic {
                    if
                    :: (stack.head == head1) -> {
                        stack.head = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = stack.head;
                        ifCond2 = false;
                    }
                    fi;
                }

                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            stack.count = stack.count + 1;
        };
    }
    // return NULL;
}

proctype thread1()
{
    byte i;

    for (i : 0 .. 2) {
        int elem;
        do
        ::  {{
                // elem = Pop();
                // int Pop(void)
                // {
                do
                :: (stack.count > 1) ->
                    {
                        int head1 = stack.head;
                        int next1;
                        atomic {
                            next1 = stack.arrayNext[head1];
                            stack.arrayNext[head1] = -1;
                        };

                        if
                        :: (next1 >= 0) ->
                            {
                                int head2 = head1;
                                bool ifCond1;
                                atomic {
                                    // _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
                                    if
                                    ::  (stack.head == head2) ->
                                        {
                                            stack.head = next1;
                                            ifCond1 =  true;
                                        }
                                    ::  else ->
                                        {
                                            head2 = stack.head;
                                            ifCond1 = false;
                                        }
                                    fi;
                                };
                                if
                                :: ifCond1 ->
                                    {
                                        stack.count--;
                                        elem = head1;
                                        goto exitPop;
                                    }
                                :: else ->
                                    {
                                        atomic {
                                            stack.arrayNext[head1] = next1;
                                        };
                                    }
                                fi;
                            }
                        fi;
                    }
                od
                elem = -1;
                goto exitPop;
                // }
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        stack.arrayValue[elem] = 1;
        assert((stack.arrayValue[elem] == 1));

        /*Push(elem);*/
        // void Push(int index)
        {
            int head1 = stack.head;
            do
            :: true -> {
                stack.arrayNext[elem] = head1;

                bool ifCond2;
                atomic {
                    if
                    :: (stack.head == head1) -> {
                        stack.head = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = stack.head;
                        ifCond2 = false;
                    }
                    fi;
                }

                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            stack.count = stack.count + 1;
        };
    }
    // return NULL;
}

proctype thread2()
{
    byte i;

    for (i : 0 .. 2) {
        int elem;
        do
        ::  {{
                // elem = Pop();
                // int Pop(void)
                // {
                do
                :: (stack.count > 1) ->
                    {
                        int head1 = stack.head;
                        int next1;
                        atomic {
                            next1 = stack.arrayNext[head1];
                            stack.arrayNext[head1] = -1;
                        };

                        if
                        :: (next1 >= 0) ->
                            {
                                int head2 = head1;
                                bool ifCond1;
                                atomic {
                                    // _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
                                    if
                                    ::  (stack.head == head2) ->
                                        {
                                            stack.head = next1;
                                            ifCond1 =  true;
                                        }
                                    ::  else ->
                                        {
                                            head2 = stack.head;
                                            ifCond1 = false;
                                        }
                                    fi;
                                };
                                if
                                :: ifCond1 ->
                                    {
                                        stack.count--;
                                        elem = head1;
                                        goto exitPop;
                                    }
                                :: else ->
                                    {
                                        atomic {
                                            stack.arrayNext[head1] = next1;
                                        };
                                    }
                                fi;
                            }
                        fi;
                    }
                od
                elem = -1;
                goto exitPop;
                // }
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        stack.arrayValue[elem] = 2;
        assert((stack.arrayValue[elem] == 2));

        /*Push(elem);*/
        // void Push(int index)
        {
            int head1 = stack.head;
            do
            :: true -> {
                stack.arrayNext[elem] = head1;

                bool ifCond2;
                atomic {
                    if
                    :: (stack.head == head1) -> {
                        stack.head = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = stack.head;
                        ifCond2 = false;
                    }
                    fi;
                }

                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            stack.count = stack.count + 1;
        };
    }
    // return NULL;
}

init
{
    // int i;
/*    Init(NUM_THREADS);
*/
/*    stack.array = malloc(3 * sizeof(SafeStackItem));
*/
    stack.count = 3;
    stack.head = 0;
    stack.arrayNext[0] = 1;
    stack.arrayNext[1] = 2;
    stack.arrayNext[2] = -1;

    run thread0();
    run thread1();
    run thread2();

}

