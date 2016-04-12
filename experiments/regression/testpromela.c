int a;
int b;

int main(int argc, char const *argv[])
{
    while (1)
    {
        a += 1;
        if (a > 10)
        {
            b += 2;
        }
        else
        {
            a += 1;
        }
        if (b > 0)
        {
            break;
        }
    }
    return 0;
}