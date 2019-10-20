#include "vector.h"
#include "utilities.h"

Vector * vector_create(size_t capacity, size_t element_size)
{
    Vector *vector       = allocate(Vector);
    vector->data         = (char*) calloc(capacity, element_size);
    vector->capacity     = capacity;
    vector->element_size = element_size;

    return vector;
}

void vector_free(Vector *vector)
{
    free(vector->data);
    free(vector);
}

void vector_resize(Vector *vector, size_t new_capacity)
{
    if (vector->capacity == new_capacity) return;

    void *new_data = calloc(new_capacity, vector->element_size);
    assert(new_data);

    memcpy(new_data, vector->data, vector->element_size * min(vector->capacity, new_capacity));

    free(vector->data);

    vector->data     = new_data;
    vector->capacity = new_capacity;
}

void vector_push(Vector *vector, char *data)
{
    if (vector->count == vector->capacity)
    {
        vector_resize(vector, vector->capacity * 2);
    }

    memcpy(vector->data + vector->element_size * vector->count, data, vector->element_size);

    vector->count += 1;
}

void vector_pop(Vector *vector)
{
    assert(vector->count);

    memset(vector->data + vector->element_size * vector->count, 0, vector->element_size);

    vector->count -= 1;
}

void * vector_get(Vector *vector, size_t index)
{
    assert(index < vector->count);

    return vector->data + index * vector->element_size;
}

void vector_insert(Vector *vector, void *data, size_t index)
{
    assert(index <= vector->count);

    if (index == vector->count)
    {
        vector_push(vector, data);
    }
    else
    {
        char  *destination = vector->data + vector->element_size * (index + 1);
        char  *source      = vector->data + vector->element_size * index;
        size_t size        = vector->element_size * (vector->count - index);

        memmove(destination, source, size);
        memcpy(source, data, vector->element_size);
    
        vector->count += 1;
    }
}

void vector_remove(Vector *vector, void *data)
{
    assert(vector->count);

    size_t position = vector_find(vector, data);
    assert(position != vector->count);

    // We don't need to move data if removing at the end.
    if (position == vector->count - 1)
    {
        memset(vector->data + vector->element_size * position, 0, vector->element_size);
    }
    else
    {
        char  *destination = vector->data + vector->element_size * position;
        char  *source      = vector->data + vector->element_size * (position + 1);
        size_t size        = vector->element_size * (vector->count - position - 1);

        memmove(destination, source, size);
    }

    vector->count -= 1;
}

void vector_clear(Vector *vector)
{
    memset(vector->data, 0, vector->element_size * vector->count);
    
    vector->count = 0;
}

size_t vector_find(Vector *vector, void *data)
{
    if (vector->count == 0)
    {
        return vector->count;
    }

    for (size_t i = 0; i < vector->count; i++)
    {
        int32_t comparison = memcmp(vector->data + vector->element_size * i, data, vector->element_size);

        if (comparison == 0)
        {
            return i;
        }
    }

    return vector->count;
}