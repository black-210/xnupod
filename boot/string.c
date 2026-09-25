#include <stddef.h>

size_t
strlen(const char *s)
{
    size_t n = 0;

    while (s[n])
        n++;

    return n;
}

int
memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *x = a;
    const unsigned char *y = b;

    for (size_t i = 0; i < n; i++) {
        if (x[i] != y[i])
            return x[i] < y[i] ? -1 : 1;
    }

    return 0;
}

void *
memset(void *ptr, int value, size_t n)
{
    unsigned char *p = ptr;

    for (size_t i = 0; i < n; i++)
        p[i] = (unsigned char)value;

    return ptr;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; i++)
        d[i] = s[i];

    return dest;
}
