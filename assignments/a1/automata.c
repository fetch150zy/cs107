#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INVERTED_BOX "\e[7m \e[0m"

#define LIVE_STR "+"
#define EMPTY_STR " "

#define MSB sizeof(long)*CHAR_BIT-1
const int POW2[3] = {1, 2, 4};

void ruleset2array(unsigned char ruleset, int a[])
{
    for (int i = 0; i < CHAR_BIT; i++)
        a[i] = (('\1' << i) & ruleset) > '\0';
}

unsigned long acyclicShiftL(unsigned long num, int shift)
{
    if ((shift <= MSB) && shift >= 0)
        return num << shift;
    else
        return 0UL;
}

int findPatternCode(int i, unsigned long gen)
{
    int out = 0;
    for (int j = 1; j >= -1; j--)
        out += ((acyclicShiftL(1L, i+j) & gen) > 0) * POW2[j+1];
    return out;
}

unsigned long advance(unsigned long cur_gen, unsigned char ruleset)
{
    int ruleArray[CHAR_BIT];
    ruleset2array(ruleset, ruleArray);
    unsigned long out = 0;
    for (int i = MSB; i >= 0; i--) {
        int patternCode = findPatternCode(i, cur_gen);
        out += (long)ruleArray[patternCode] << i;
    }

    return out;
}

void draw_generation(unsigned long gen)
{
    const char *font[] = { EMPTY_STR, LIVE_STR };

    for (int i = MSB; i >= 0; i--) {
        long bit = 1L << i;
        printf("%s", font[(gen & bit) > 0]);
    }
    printf("\n");
}

unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname)
{
    char *end;
    unsigned long n = strtoul(str, &end, 0);
    if (*end != '\0') 
        error(1, 0, "Invalid number '%s' for %s", str, argname);
    if (n < low || n > high || errno == ERANGE) 
        error(1, 0, "%s %s is not in range [%#lx, %#lx]", argname, str, low, high);
    return n;
}

int main(int argc, char *argv[])
{
    unsigned long gen_0 = 1L << 32;
    unsigned char ruleset = 30;

    if (argc < 2)
        error(1, 0, "Missing argument. Please supply ruleset and optional initial generation.");

    ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2) gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");

    if (ruleset == 0) {
        draw_generation(gen_0);
    } else {
        unsigned long gen = gen_0;
        for (int i = 0; i < 32; i++) {
            draw_generation(gen);
            gen = advance(gen, ruleset);
        }
    }
    return 0;
}
