int i, j;

proctype t1()
{
    byte k;
    for (k : 1 .. 11) {
        i = i + j;
    }
}

proctype t2()
{
    byte k;
    for (k : 1 .. 11) {
        j = j + i;
    }
}


init
{
    i = 1;
    j = 1;
    run t1();
    run t2();
    assert((i < 46368) && (j < 46368));
}