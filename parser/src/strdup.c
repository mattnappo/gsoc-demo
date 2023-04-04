#include <stdlib.h>
#include <string.h>
#include "strdup.h"

/**
 * From strdup(3): allocates sufficient memory for a copy of the
 * string s, does the copy, and returns a pointer to it.
 */
char *strdup(const char *s) {
    if (s == NULL) { return NULL; }
    char *t = calloc(strlen(s)+1, 1);
    strcpy(t, s);
    return t;
}
