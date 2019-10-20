#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>



typedef struct Vector
{
    char  *data; // Char in this context means a byte.
    size_t count;
    size_t capacity;
    size_t element_size;

} Vector;



Vector * vector_create  (size_t element_size, size_t capacity);
void     vector_free    (Vector *Vector);
void     vector_push    (Vector *Vector, char *data);
void     vector_pop     (Vector *Vector);
void *   vector_get     (Vector *Vector, size_t index);
void     vector_insert  (Vector *Vector, void *data, size_t index);
void     vector_remove  (Vector *Vector, void *data);
void     vector_clear   (Vector *Vector);
size_t   vector_find    (Vector *Vector, void *data);

#elif
#error Please do not use the vector data structure at this time, thank you!
#endif
