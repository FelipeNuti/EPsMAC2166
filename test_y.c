#include <stdio.h>

int acha_y(int x, int z, int b)
{
    return (int) (z-x+256+256) % (1<<b);
}

int main()
{
    while (1)
    {
        int z, x, b;
        scanf("%d %d %d", &x, &z, &b);
        printf("%d\n\n", acha_y(x, z,b));

    }
    return 0;
}