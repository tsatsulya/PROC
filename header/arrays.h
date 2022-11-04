#define max(x, y) ((x) > (y)) ? (x) : (y)

#define array(type) array_##type

#define define_array(type)              \
    typedef struct array(type) {        \
        type* buffer;                   \
        size_t size;                    \
        size_t capacity;                 \
    } array(type);



#define create_array(type, length)                          \
    (array(type)) {                                         \
        .buffer = (type*) calloc(sizeof(type), length),     \
        .size = 0,                                          \
        .capacity = length                                  \
    }


#define recalloc_array(type, array) do {                                                            \
    array.capacity = max(1, array.capacity * 2);                                                    \
    type* implemented_##array = (type*) realloc(array.buffer, sizeof(type) * (array.capacity));     \
    array.buffer = implemented_##array;                                                             \
} while(0)


#define push_element(type, array, element) do {             \
    if (array.size == array.capacity)                       \
        recalloc_array(type, array);                        \
    array.buffer[array.size] = element;                     \
    array.size++;                                           \
} while(0)



