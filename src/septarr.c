#include "septarr.h"
#include <stdlib.h>
#include <string.h>

/*
 * Initial function to construct the data. We have to call septarr_destroy() to
 * destroy (free()) the data after we done with the data. This function will
 * return NULL when it fail, and then set errno.
 */
struct septarr *septarr_init()
{
        struct septarr *ret;

        ret = malloc(sizeof(*ret));
        if (ret == NULL)
                return NULL;

        ret->size = malloc(sizeof(int));
        if (ret->size == NULL) {
                free(ret);
                return NULL;
        }

        ret->allocated_size = malloc(sizeof(int));
        if (ret->allocated_size == NULL) {
                free(ret->size);
                free(ret);
                return NULL;
        }

        ret->data = malloc(sizeof(*ret->data)*SEPTARR_INIT_SIZE);
        if (ret->data == NULL) {
                free(ret->size);
                free(ret->allocated_size);
                free(ret);
                return NULL;
        }

        *ret->size = 0;
        *ret->allocated_size = SEPTARR_INIT_SIZE;
        ret->data[0] = NULL;

        return ret;
}

/*
 * This function have responsbility for reallocation of the array inside the
 * data structure. This function will return 0 if success and return a number
 * below zero if fail and then set errno.
 */
int septarr_realloc(struct septarr **ptr, unsigned int new_allocated_size)
{
        struct septarr_data **new_ptr;
        
        new_ptr = reallocarray(
                (*ptr)->data, new_allocated_size, sizeof(*(*ptr)->data)
        );
        if (new_ptr == NULL)
                return -1;
        
        (*ptr)->data = new_ptr;
        *(*ptr)->allocated_size = new_allocated_size;

        return 0;
}

/*
 * Get the size of the array inside the data structure. This is the easiest way
 * to get the size.
 */
unsigned int septarr_get_size(struct septarr *ptr)
{
        return *ptr->size;
}

/*
 * Get the allocated size of the array inside the data structure. This is the
 * easiest way to get the allocated size.
 */
unsigned int septarr_get_allocated_size(struct septarr *ptr)
{
        return *ptr->allocated_size;
}

/*
 * Get the data type stored in the element of array inside the data structure.
 * Make sure that you are not accessing an out-of-bounds value or you will get
 * undefined behavior (UB). The reason why I did not make this function safe is
 * because I need speed, so we have to use it carefully.
 */
unsigned int septarr_get_type(struct septarr *ptr, unsigned int index)
{
        return *ptr->data[index]->type;
}

/*
 * The function that will grow the array inside the data structure if we need
 * to grow it. This function will return 0 if success and return a number
 * below zero if fail and then set errno.
 */
int septarr_grow_if_needed(struct septarr **ptr)
{
        if (*(*ptr)->size >= *(*ptr)->allocated_size-1)
                return septarr_realloc(
                        &(*ptr), *(*ptr)->allocated_size+SEPTARR_INIT_SIZE
                );

        return 0;
}

/*
 * The function that will shrink the array inside the data structure if we need
 * to shrink it. This function will return 0 if success and return a number
 * below zero if fail and then set errno.
 */
int septarr_shrink_if_needed(struct septarr **ptr)
{
        if((*(*ptr)->allocated_size-SEPTARR_INIT_SIZE) >= *(*ptr)->size)
                return septarr_realloc(
                        &(*ptr), *(*ptr)->allocated_size-SEPTARR_INIT_SIZE
                );

        return 0;
}

/*
 * Push an int value to the data structure. This function will return 0 if
 * success and return a number below zero if fail and then set errno.
 */
int septarr_push_int(struct septarr **ptr, int value)
{
        int gin = septarr_grow_if_needed(&(*ptr));
        if (gin < 0)
                return gin;

        (*ptr)->data[*(*ptr)->size] = malloc(sizeof(**(*ptr)->data));
        if ((*ptr)->data[*(*ptr)->size] == NULL)
                return -2;

        (*ptr)->data[*(*ptr)->size]->type = malloc(
                sizeof(*(*ptr)->data[*(*ptr)->size]->type)
        );
        if ((*ptr)->data[*(*ptr)->size]->type == NULL) {
                free((*ptr)->data[*(*ptr)->size]);
                return -3;
        }

        (*ptr)->data[*(*ptr)->size]->value = malloc(sizeof(int));
        if ((*ptr)->data[*(*ptr)->size]->value == NULL) {
                free((*ptr)->data[*(*ptr)->size]->type);
                free((*ptr)->data[*(*ptr)->size]);
                return -4;

        }

        *(*ptr)->data[*(*ptr)->size]->type = SEPTARR_INT;
        *((int *)(*ptr)->data[*(*ptr)->size]->value) = value;
        *(*ptr)->size += 1;
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * Push a float value to the data structure. This function will return 0 if
 * success and return a number below zero if fail and then set errno.
 */
int septarr_push_float(struct septarr **ptr, float value)
{
        int gin = septarr_grow_if_needed(&(*ptr));
        if (gin < 0)
            return gin;

        (*ptr)->data[*(*ptr)->size] = malloc(sizeof(**(*ptr)->data));
        if ((*ptr)->data[*(*ptr)->size] == NULL)
                return -2;

        (*ptr)->data[*(*ptr)->size]->type = malloc(
                sizeof(*(*ptr)->data[*(*ptr)->size]->type)
        );
        if ((*ptr)->data[*(*ptr)->size]->type == NULL) {
                free((*ptr)->data[*(*ptr)->size]);
                return -3;
        }

        (*ptr)->data[*(*ptr)->size]->value = malloc(sizeof(float));
        if ((*ptr)->data[*(*ptr)->size]->value == NULL) {
                free((*ptr)->data[*(*ptr)->size]->type);
                free((*ptr)->data[*(*ptr)->size]);
                return -4;
        }

        *(*ptr)->data[*(*ptr)->size]->type = SEPTARR_FLOAT;
        *(float *)(*ptr)->data[*(*ptr)->size]->value = value;
        *(*ptr)->size += 1;
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * Push a double value to the data structure. This function will return 0 if
 * success and return a number below zero if fail and then set errno.
 */
int septarr_push_double(struct septarr **ptr, double value)
{
        int gin = septarr_grow_if_needed(&(*ptr));
        if (gin < 0)
                return gin;

        (*ptr)->data[*(*ptr)->size] = malloc(sizeof(**(*ptr)->data));
        if ((*ptr)->data[*(*ptr)->size] == NULL)
                return -2;

        (*ptr)->data[*(*ptr)->size]->type = malloc(
                sizeof(*(*ptr)->data[*(*ptr)->size]->type)
        );
        if ((*ptr)->data[*(*ptr)->size]->type == NULL) {
                free((*ptr)->data[*(*ptr)->size]);
                return -3;
        }

        (*ptr)->data[*(*ptr)->size]->value = malloc(sizeof(double));
        if ((*ptr)->data[*(*ptr)->size]->value == NULL) {
                free((*ptr)->data[*(*ptr)->size]->type);
                free((*ptr)->data[*(*ptr)->size]);
                return -4;
        }

        *(*ptr)->data[*(*ptr)->size]->type = SEPTARR_DOUBLE;
        *(double *)(*ptr)->data[*(*ptr)->size]->value = value;
        *(*ptr)->size += 1;
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * Push a string value to the data structure. This function will return 0 if
 * success and return a number below zero if fail and then set errno.
 */
int septarr_push_string(struct septarr **ptr, char *value)
{
        int gin = septarr_grow_if_needed(&(*ptr));
        if (gin < 0)
                return gin;

        (*ptr)->data[*(*ptr)->size] = malloc(sizeof(**(*ptr)->data));
        if ((*ptr)->data[*(*ptr)->size] == NULL)
                return -2;

        (*ptr)->data[*(*ptr)->size]->type = malloc(
                sizeof(*(*ptr)->data[*(*ptr)->size]->type)
        );
        if ((*ptr)->data[*(*ptr)->size]->type == NULL) {
                free((*ptr)->data[*(*ptr)->size]);
                return -3;
        }

        (*ptr)->data[*(*ptr)->size]->value = malloc(
                sizeof(char)*strlen(value) + 1
        );
        if ((*ptr)->data[*(*ptr)->size]->value == NULL) {
                free((*ptr)->data[*(*ptr)->size]->type);
                free((*ptr)->data[*(*ptr)->size]);
                return -4;
        }

        *(*ptr)->data[*(*ptr)->size]->type = SEPTARR_STRING;
        strcpy((char *)(*ptr)->data[*(*ptr)->size]->value, value);
        *(*ptr)->size += 1;
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * Push a septarr to the data structure. This function will return 0 if success
 * and return a number below zero if fail and then set errno.
 */
int septarr_push_septarr(struct septarr **ptr, struct septarr *value)
{
        if (value == NULL)
                return -2;

        int gin = septarr_grow_if_needed(&(*ptr));
        if (gin < 0)
                return gin;

        (*ptr)->data[*(*ptr)->size] = malloc(sizeof(**(*ptr)->data));
        if ((*ptr)->data[*(*ptr)->size] == NULL)
                return -3;

        (*ptr)->data[*(*ptr)->size]->type = malloc(
                sizeof(*(*ptr)->data[*(*ptr)->size]->type)
        );
        if ((*ptr)->data[*(*ptr)->size]->type == NULL) {
                free((*ptr)->data[*(*ptr)->size]);
                return -4;
        }

        *(*ptr)->data[*(*ptr)->size]->type = SEPTARR_SEPTARR;
        (*ptr)->data[*(*ptr)->size]->value = value;
        *(*ptr)->size += 1;
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * The easiest way to get the data that stored in data structure. Make sure
 * that you are not accessing an out-of-bounds value or you will get
 * undefined behavior (UB). Make sure that you use the right function 
 * for the right data type.
 *
 * The reason why I did not make this function safe is
 * because I need speed, so we have to use it carefully.
 * 
 * This comment applied to all septarr_get_*() functions below.
 */
int septarr_get_int(struct septarr *ptr, unsigned int index)
{
        return *(int *)ptr->data[index]->value;
}

float septarr_get_float(struct septarr *ptr, unsigned int index)
{
        return *(float *)ptr->data[index]->value;
}

double septarr_get_double(struct septarr *ptr, unsigned int index)
{
        return *(double *)ptr->data[index]->value;
}

char *septarr_get_string(struct septarr *ptr, unsigned int index)
{
        return (char *)ptr->data[index]->value;
}

struct septarr *septarr_get_septarr(struct septarr *ptr, unsigned int index)
{
        return (struct septarr *)ptr->data[index]->value;
}

/*
 * The function that have a responsbility to delete an element of the array
 * inside the data structure. This function will return 0 if success and return
 * a number below zero if fail.
 */
int septarr_delete_element(struct septarr **ptr, unsigned int index)
{
        if (index >= *(*ptr)->size)
                return -2;

        int sin = septarr_shrink_if_needed(&(*ptr));
        if (sin < 0)
                return sin;

        if (*(*ptr)->data[index]->type == SEPTARR_SEPTARR) {
                struct septarr *septarr = (struct septarr *)
                                                (*ptr)->data[index]->value;
                septarr_destroy(&septarr);
        } else {
                free((*ptr)->data[index]->value);
        }
        
        free((*ptr)->data[index]->type);

        for (int i = index; i  < *(*ptr)->size - 1; i++)
                *(*ptr)->data[i] = *(*ptr)->data[i + 1];

        *(*ptr)->size -= 1;

        free((*ptr)->data[*(*ptr)->size]);
        
        (*ptr)->data[*(*ptr)->size] = NULL;

        return 0;
}

/*
 * Destoy the data structure. It will frees the memmory that was allocated
 * during initialization and insertions.
 */
int septarr_destroy(struct septarr **ptr)
{
        for (int i = *(*ptr)->size  - 1; i >= 0; i--) {
                if (*(*ptr)->data[i]->type == SEPTARR_SEPTARR) {
                        struct septarr *septarr = (struct septarr *)
                                                        (*ptr)->data[i]->value;
                        septarr_destroy(&septarr);
                } else {
                        free((*ptr)->data[i]->value);
                }
                
                free((*ptr)->data[i]->type);
                free((*ptr)->data[i]);
        }

        free((*ptr)->data);
        free((*ptr)->size);
        free((*ptr)->allocated_size);
        free(*ptr);

        return 0;
}

