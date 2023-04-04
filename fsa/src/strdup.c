#include <stdlib.h>
#include "strdup.h"

char*
strdup(const char* s) {
    if (s == NULL) {
	return NULL;
    }
    char* t = malloc(strlen(s)+1);
    strcpy(t, s);
    return t;
}

