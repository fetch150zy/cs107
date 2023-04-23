#include <error.h>
#include <stdio.h>
#include <stdlib.h>

void print_triangle(int nlevels)
{
    int size = 1 << nlevels;

    for (int row = size - 1; row >= 0; row--) {
        for (int col = 0; col < row; col++) {
            printf(" ");
        }
        for (int col = 0; col + row < size; col++) {
            printf("%c ", (col & row) ? ' ' : '*');
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int nlevels = 3;
    if (argc > 1)
        nlevels = atoi(argv[1]);
    if (nlevels > 8 || nlevels < 0){
        printf("nlevels must be >= 1 and <= 8\n");
        exit(1);
    }
    print_triangle(nlevels);
    return 0;
}
