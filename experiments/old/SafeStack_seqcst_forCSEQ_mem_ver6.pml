
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

/// Global
// SafeStack stack;

#define MEM_SIZE 8

int MEM_VALUE[MEM_SIZE];
// array allocated 6 placed
// head is 6
// count is 7

// void __VERIFIER_atomic_store(int *obj, int v)
// {
//     *obj = v;
// }

// int __VERIFIER_atomic_load(int *obj)
// {
//     return *obj;
// }

// int __VERIFIER_atomic_exchange(int *obj, int v)
// {
//     int t = *obj;
//     *obj = v;
//     return t;
// }

// _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
// {
//     if(*obj == *expected)
//     {
//         *obj = desired;
//         return 1;
//     }
//     else
//     {
//         *expected = *obj;
//         return 0;
//     }
// }

// int __VERIFIER_atomic_fetch_add(int * obj, int v)
// {
//     int old = *obj;
//     *obj = old + v;
//     return old;
// }

// int __VERIFIER_atomic_fetch_sub(int * obj, int v)
// {
//     int old = *obj;
//     *obj = old - v;
//     return old;
// }

// void Init(int pushCount)
// {
//     // int i;
//     // stack.array = malloc(3 * sizeof(SafeStackItem));
//     // __VERIFIER_atomic_store(&stack.count, pushCount);
//     MEM_VALUE[7] = 3;
//     // __VERIFIER_atomic_store(&stack.head, 0);
//     MEM_VALUE[6] = 0;
//     // for (i = 0; i < pushCount - 1; i++)
//     // {
//         // __VERIFIER_atomic_store(&stack.array[0].Next, 1);
//         MEM_VALUE[0] = 1;
//         // __VERIFIER_atomic_store(&stack.array[1].Next, 2);
//         MEM_VALUE[2] = 2;
//     // }
//     // __VERIFIER_atomic_store(&stack.array[2].Next, -1);
//         MEM_VALUE[4] = -1;
// }

// void Destroy(void)
// {
//     free(stack.array);
// }

// int Pop(void)
// {
//     // while (__VERIFIER_atomic_load(&stack.count) > 1)
//     while (MEM_VALUE[7] > 1)
//     {
//         // int head1 = __VERIFIER_atomic_load(&stack.head);
//         int head1 = MEM_VALUE[6];
//         // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);

//         if (next1 >= 0)
//         {
//             int head2 = head1;
//             if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
//             {
//                 __VERIFIER_atomic_fetch_sub(&stack.count, 1);
//                 return head1;
//             }
//             else
//             {
//                 __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
//             }
//         }
//     }

//     return -1;
// }

// void Push(int index)
// {
//     // int head1 = __VERIFIER_atomic_load(&stack.head);
//     int head1 = MEM_VALUE[6];
//     do
//     {
//         // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
//         MEM_VALUE[index * 2 + 1] = head1;

//     } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
//     __VERIFIER_atomic_fetch_add(&stack.count, 1);
// }


proctype thread0()
{
    byte i;
    for (i : 0 .. 1) {
        int elem;
        do
        :: {
            // elem = Pop();
            // int Pop(void)
            {
                // while (__VERIFIER_atomic_load(&stack.count) > 1)
                do
                :: (MEM_VALUE[7] > 1) -> {
                    // int head1 = __VERIFIER_atomic_load(&stack.head);
                    int head1 = MEM_VALUE[6];
                    // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
                    int next1;
                    atomic {
                        next1 = MEM_VALUE[2*head1 + 1];
                        MEM_VALUE[2*head1 + 1] = -1;
                    };
                    if
                    :: (next1 >= 0) -> {
                        int head2 = head1;
                        bool ifCond1;
                        atomic {
                            if
                            :: (MEM_VALUE[6] == head2) -> {
                                MEM_VALUE[6] = next1;
                                ifCond1 = true;
                                }
                            :: else -> {
                                head2 = MEM_VALUE[6];
                                ifCond1 = false;
                            }
                            fi;
                        }
                           /*(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))*/
                        if
                        :: ifCond1 -> {
                            // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                            MEM_VALUE[7]--;
                            elem = head1;
                            goto exitPop;
                        }
                        :: else -> {
                            // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                            atomic {
                                MEM_VALUE[2*head1 + 1] = next1;
                            };
                        }
                        fi;
                    }
                    fi;
                }
                od;
                elem = -1;
                goto exitPop;
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        // stack.array[elem].Value = 0;
        MEM_VALUE[2*elem] = 0;
        assert((MEM_VALUE[2*elem] == 0));

        // Push(elem);

        // void Push(int index)
        {
            // int head1 = __VERIFIER_atomic_load(&stack.head);
            int head1 = MEM_VALUE[6];
            do
            :: {
                // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
                MEM_VALUE[elem * 2 + 1] = head1;
                bool ifCond2;
                atomic {
                    if
                    :: (MEM_VALUE[6] ==  head1) -> {
                        MEM_VALUE[6] = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = MEM_VALUE[6];
                        ifCond2 = false;
                    }
                    fi;
                };
              // while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            // __VERIFIER_atomic_fetch_add(&stack.count, 1);
            MEM_VALUE[7]++;
        };
    };
    // return NULL;
}
proctype thread1()
{
    byte i;
    for (i : 0 .. 1) {
        int elem;
        do
        :: {
            // elem = Pop();
            // int Pop(void)
            {
                // while (__VERIFIER_atomic_load(&stack.count) > 1)
                do
                :: (MEM_VALUE[7] > 1) -> {
                    // int head1 = __VERIFIER_atomic_load(&stack.head);
                    int head1 = MEM_VALUE[6];
                    // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
                    int next1;
                    atomic {
                        next1 = MEM_VALUE[2*head1 + 1];
                        MEM_VALUE[2*head1 + 1] = -1;
                    };
                    if
                    :: (next1 >= 0) -> {
                        int head2 = head1;
                        bool ifCond1;
                        atomic {
                            if
                            :: (MEM_VALUE[6] == head2) -> {
                                MEM_VALUE[6] = next1;
                                ifCond1 = true;
                                }
                            :: else -> {
                                head2 = MEM_VALUE[6];
                                ifCond1 = false;
                            }
                            fi;
                        }
                           /*(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))*/
                        if
                        :: ifCond1 -> {
                            // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                            MEM_VALUE[7]--;
                            elem = head1;
                            goto exitPop;
                        }
                        :: else -> {
                            // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                            atomic {
                                MEM_VALUE[2*head1 + 1] = next1;
                            };
                        }
                        fi;
                    }
                    fi;
                }
                od;
                elem = -1;
                goto exitPop;
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        // stack.array[elem].Value = 1;
        MEM_VALUE[2*elem] = 1;
        assert((MEM_VALUE[2*elem] == 1));

        // Push(elem);

        // void Push(int index)
        {
            // int head1 = __VERIFIER_atomic_load(&stack.head);
            int head1 = MEM_VALUE[6];
            do
            :: {
                // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
                MEM_VALUE[elem * 2 + 1] = head1;
                bool ifCond2;
                atomic {
                    if
                    :: (MEM_VALUE[6] ==  head1) -> {
                        MEM_VALUE[6] = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = MEM_VALUE[6];
                        ifCond2 = false;
                    }
                    fi;
                };
              // while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            // __VERIFIER_atomic_fetch_add(&stack.count, 1);
            MEM_VALUE[7]++;
        };
    };
    // return NULL;
}
proctype thread2()
{
    byte i;
    for (i : 0 .. 1) {
        int elem;
        do
        :: {
            // elem = Pop();
            // int Pop(void)
            {
                // while (__VERIFIER_atomic_load(&stack.count) > 1)
                do
                :: (MEM_VALUE[7] > 1) -> {
                    // int head1 = __VERIFIER_atomic_load(&stack.head);
                    int head1 = MEM_VALUE[6];
                    // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
                    int next1;
                    atomic {
                        next1 = MEM_VALUE[2*head1 + 1];
                        MEM_VALUE[2*head1 + 1] = -1;
                    };
                    if
                    :: (next1 >= 0) -> {
                        int head2 = head1;
                        bool ifCond1;
                        atomic {
                            if
                            :: (MEM_VALUE[6] == head2) -> {
                                MEM_VALUE[6] = next1;
                                ifCond1 = true;
                                }
                            :: else -> {
                                head2 = MEM_VALUE[6];
                                ifCond1 = false;
                            }
                            fi;
                        }
                           /*(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))*/
                        if
                        :: ifCond1 -> {
                            // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                            MEM_VALUE[7]--;
                            elem = head1;
                            goto exitPop;
                        }
                        :: else -> {
                            // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                            atomic {
                                MEM_VALUE[2*head1 + 1] = next1;
                            };
                        }
                        fi;
                    }
                    fi;
                }
                od;
                elem = -1;
                goto exitPop;
            };
            exitPop: skip;
            if
            :: (elem >= 0) -> break;
            fi;
        }
        od;
        // stack.array[elem].Value = 2;
        MEM_VALUE[2*elem] = 2;
        assert((MEM_VALUE[2*elem] == 2));

        // Push(elem);

        // void Push(int index)
        {
            // int head1 = __VERIFIER_atomic_load(&stack.head);
            int head1 = MEM_VALUE[6];
            do
            :: {
                // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
                MEM_VALUE[elem * 2 + 1] = head1;
                bool ifCond2;
                atomic {
                    if
                    :: (MEM_VALUE[6] ==  head1) -> {
                        MEM_VALUE[6] = elem;
                        ifCond2 = true;
                    }
                    :: else -> {
                        head1 = MEM_VALUE[6];
                        ifCond2 = false;
                    }
                    fi;
                };
              // while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
                if
                :: ifCond2 -> break;
                fi;
            }
            od;
            // __VERIFIER_atomic_fetch_add(&stack.count, 1);
            MEM_VALUE[7]++;
        };
    };
    // return NULL;
}

init
{
    // Init(NUM_THREADS);

    // void Init(int pushCount)
    {
        // int i;
        // stack.array = malloc(3 * sizeof(SafeStackItem));
        // __VERIFIER_atomic_store(&stack.count, pushCount);
        MEM_VALUE[7] = 3;
        // __VERIFIER_atomic_store(&stack.head, 0);
        MEM_VALUE[6] = 0;
        // for (i = 0; i < pushCount - 1; i++)
        // {
            // __VERIFIER_atomic_store(&stack.array[0].Next, 1);
            MEM_VALUE[0] = 1;
            // __VERIFIER_atomic_store(&stack.array[1].Next, 2);
            MEM_VALUE[2] = 2;
        // }
        // __VERIFIER_atomic_store(&stack.array[2].Next, -1);
            MEM_VALUE[4] = -1;
    };



    run thread0();
    run thread1();
    run thread2();


    // for (i = 0; i < NUM_THREADS; ++i) {
    //     pthread_create(&threads[i], NULL, thread, (void *)i);
    // }

    // for (i = 0; i < NUM_THREADS; ++i) {
    //     pthread_join(threads[i], NULL);
    // }

    // return 0;
}

