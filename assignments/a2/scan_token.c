#include <stdbool.h>
#include <string.h>

bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen)
{
    size_t dellen = strspn(*p_input, delimiters);
    *p_input += dellen;
    size_t toklen = strcspn(*p_input, delimiters);
    if (toklen > 0) {
        if (toklen >= buflen)
        toklen = buflen-1;
        strncpy(buf, *p_input, toklen);
        buf[toklen] = '\0';
        *p_input += toklen;
        return true;
    } else
        return false;
}
