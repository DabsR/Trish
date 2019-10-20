#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define initialise(OBJECT_PTR) memset(OBJECT_PTR, 0, sizeof(*OBJECT_PTR));
#define allocate(TYPE)         (TYPE*) calloc(1, sizeof(TYPE));
#define array(TYPE, LENGTH)    calloc(LENGTH, sizeof(TYPE));

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))