/*
 * Initial allocation size. It is used for reallocation data too,
 * because it will be expensive if we do reallocation every time we add new or
 * delete an existing element.
 */
#define SEPTARR_INIT_SIZE   2048

// To mark what data type is stored in each element.
enum septarr_type {
        SEPTARR_INT,
        SEPTARR_FLOAT,
        SEPTARR_DOUBLE,
        SEPTARR_STRING,
        SEPTARR_SEPTARR
};

/*
 * Where the data stored. The value and the data type, so we can keep track the
 * data type for every data stored. We store the data in heap, so it can handle
 * huge amount of data.
 */
struct septarr_data {
        enum septarr_type *type;
        void *value;
};

// Here is the data structure
struct septarr {
        unsigned int *size;
        unsigned int *allocated_size;
        struct septarr_data **data;
};

struct septarr *septarr_init();
unsigned int septarr_get_size(struct septarr *ptr);
unsigned int septarr_get_allocated_size(struct septarr *ptr);
unsigned int septarr_get_type(struct septarr *ptr, unsigned int index);
int septarr_push_int(struct septarr **ptr, int value);
int septarr_push_float(struct septarr **ptr, float value);
int septarr_push_double(struct septarr **ptr, double value);
int septarr_push_string(struct septarr **ptr, char *value);
int septarr_push_septarr(struct septarr **ptr, struct septarr *value);
int septarr_get_int(struct septarr *ptr, unsigned int index);
float septarr_get_float(struct septarr *ptr, unsigned int index);
double septarr_get_double(struct septarr *ptr, unsigned int index);
char *septarr_get_string(struct septarr *ptr, unsigned int index);
struct septarr *septarr_get_septarr(struct septarr *ptr, unsigned int index);
int septarr_delete_element(struct septarr **ptr, unsigned int index);
int septarr_destroy(struct septarr **ptr);