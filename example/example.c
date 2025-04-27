#include "../src/septarr.h"
#include <stdio.h>

void print_data(struct septarr *ptr)
{
    for(int i=0; i<septarr_get_size(ptr); i++) {
        switch (septarr_get_type(ptr, i))
        {
        case SEPTARR_INT:
            printf("%d : %d : int\n", i, septarr_get_int(ptr, i));
            break;
        case SEPTARR_FLOAT:
            printf("%d : %f : float\n", i, septarr_get_float(ptr, i));
            break;
        case SEPTARR_DOUBLE:
            printf("%d : %f : double\n", i, septarr_get_double(ptr, i));
            break;
        case SEPTARR_STRING:
            printf("%d : %s : string\n", i, septarr_get_string(ptr, i));
            break;
        case SEPTARR_SEPTARR:
            printf("%d : septarr\n", i);
            printf("==============================\n");
            struct septarr *ret = septarr_get_septarr(ptr, i);
            print_data(ret);
            printf("==============================\n");
            break;
        default:
            break;
        }
    }
}

int main(void)
{
    struct septarr *ptr;

    ptr = septarr_init();

    septarr_push_int(&ptr, 1);
    septarr_push_int(&ptr, 2);
    septarr_push_int(&ptr, 3);
    septarr_push_float(&ptr, 5.6);
    septarr_push_float(&ptr, 5.7);
    septarr_push_float(&ptr, 5.8);
    septarr_push_double(&ptr, 87656787643212345);
    septarr_push_double(&ptr, 98670910297930492);
    septarr_push_double(&ptr, 10290203937268277);
    septarr_push_string(&ptr, "Hello");
    septarr_push_string(&ptr, "world");

    printf("ptr size : %d\n", septarr_get_size(ptr));
    printf("ptr allocatd size : %d\n", septarr_get_allocated_size(ptr));

    septarr_delete_element(&ptr, 8);
    septarr_delete_element(&ptr, 7);
    septarr_delete_element(&ptr, 6);

    printf("ptr size : %d\n", septarr_get_size(ptr));
    printf("ptr allocatd size : %d\n", septarr_get_allocated_size(ptr));

    septarr_push_string(&ptr, "Lorem");
    septarr_push_string(&ptr, "ipsum");
    septarr_push_string(&ptr, "dolor");

    printf("ptr size : %d\n", septarr_get_size(ptr));
    printf("ptr allocatd size : %d\n", septarr_get_allocated_size(ptr));

    struct septarr *ptr1 = septarr_init();
    septarr_push_string(&ptr1, "Rest the in loud");
    septarr_push_string(&ptr1, "Trees snow leopard");
    septarr_push_septarr(&ptr, ptr1);

    struct septarr *ptr2 = septarr_init();
    septarr_push_string(&ptr2, "Mountain heroes water");
    septarr_push_string(&ptr2, "Flare the soup");
    septarr_push_septarr(&ptr, ptr2);

    septarr_delete_element(&ptr, 11);

    print_data(ptr);

    septarr_destroy(&ptr);
    
    return 0;
}
