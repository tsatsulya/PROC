#include "stack.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>


#define min(x, y) ((x) < (y)) ? (x) : (y)

#define max(x, y) ((x) > (y)) ? (x) : (y)


static Canary_data CANARY = 0x000DEAD000;
Stack_data INVALID_VALUE = 0x00FE00;


static void stack_ptr_verify(Stack* ptr) {
    if (!ptr) {
        printf("NULL_PTR>:(");
        abort(); // TODO: better to disable in release
    }
} 

static size_t stack_get_buffer_size(size_t length) {

    size_t size = length*sizeof(Stack_data) + 2*sizeof(Canary_data);

    return size;
}

static Stack_data* stack_get_data_base_ptr(Stack *stack) {
    return (Stack_data*)(stack->data + sizeof(Canary_data)); 
}  

static Stack_data* get_ptr_to_element(Stack* stack, long long int position) {

    Stack_data* data = stack_get_data_base_ptr(stack);
    
    Stack_data* ptr_to_element = data + position;

    return ptr_to_element;
} 

static Stack_data get_element(Stack* stack, long long int position) { 
    
    return *get_ptr_to_element(stack, position);
} 



static unsigned stack_light_check_status(Stack* stack) { 
    unsigned status = SUCCESS;

    if (!stack) {
        status |= STACK_NULL; 
        return status;
    }
    if (stack->capacity < stack->element_count)
        status |= STACK_OVERFLOW;
    if (!stack->data)
        status |= DATA_NULL;

    return status;
}

static unsigned long long int stack_hash(Stack* stack);

static stack_status stack_check_hash(Stack* stack) {

    if (!stack)
        return STACK_NULL;

    unsigned long long int correct_stack_hash = stack->hash;
    unsigned long long int actual_stack_hash = stack_hash(stack);

    return (actual_stack_hash == correct_stack_hash) ? SUCCESS : INVALID_HASH;
}

static Canary_data* get_left_canary_position(Stack* stack) {
    return (Canary_data*)stack->data;
}

static Canary_data* get_right_canary_position(Stack* stack) {
    return (Canary_data*)(stack->data + sizeof(Stack_data) * stack->capacity + sizeof(Canary_data));
}


static unsigned stack_check_canaries(Stack* stack) {

    if (!stack) return STACK_NULL;
        unsigned status = SUCCESS;
    
    if (*get_left_canary_position(stack) != CANARY) status |= DAMAGED_LEFT_CANARY; 

    if (*get_right_canary_position(stack) != CANARY) status |= DAMAGED_RIGHT_CANARY;

    return status;
}

static unsigned stack_check_status(Stack *stack) {

    unsigned status = SUCCESS;

    status |= stack_light_check_status(stack);
    if (status) return status; // TODO: propagate?

    // propagate_error(status);


    status |= stack_check_canaries(stack);
    status |= stack_check_hash(stack);

    return status;
}

void stack_dump(Stack* stack, const char* file, const char* func, int line, stack_status status);
// TODO:                      ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ source_location?
//                            GET_SOURCE_LOCATION() would be useful macro

static unsigned stack_assert_invariants(Stack* stack, const char* file, const char* func, int line) {

    unsigned status = SUCCESS;

    if (status) {
        stack_dump(stack, file, func, line, status);
        return status;
    }

    status = stack_check_status(stack);

    if (status) {
        stack_dump(stack, file, func, line, status);
    }

    return status;
}

#define stack_assert_invariants(stack) stack_assert_invariants(stack, __FILE__, __func__, __LINE__)


static fullness get_stack_occupancy_status(Stack* stack) {

    if (stack->element_count == 0) return EMPTY;
    if (stack->element_count < stack->capacity / 2) return UNDER_HALF;
    if (stack->element_count < stack->capacity) return OVER_HALF;
    return FULL;
}

static stack_status stack_change_length(Stack* stack, double multiplier) {

    size_t new_capacity = max(stack->min_capacity, stack->capacity * multiplier);
    size_t new_size = stack_get_buffer_size(new_capacity); 

    stack->capacity = new_capacity;
    stack->data = (char*) realloc(stack->data, new_size);

    stack_assert_invariants(stack);

    return SUCCESS;
}

void print_size_info(Stack* stack, FILE* file) {

    fprintf(file, "capacity: %ld\n", stack->capacity);
    fprintf(file, "num of elements: %ld\n", stack->element_count);
}

static stack_status stack_length_up(Stack* stack) {

    stack_change_length(stack, stack->size_multiplier);
    return SUCCESS;
}

static stack_status stack_length_down(Stack* stack) {

    stack_change_length(stack, 1/stack->size_multiplier);
    return SUCCESS;
}




static void print_canaries(Stack* stack, FILE* file) {
    
    fprintf(file, "left_canary: %llx\n", *get_left_canary_position(stack));
    fprintf(file, "right_canary: %llx\n", *get_right_canary_position(stack));
}

static stack_status set_canaries(Stack* stack) {
    *get_left_canary_position(stack) = *get_right_canary_position(stack) = CANARY;
    return SUCCESS;
}

static unsigned long int gnu_hash(void* data, size_t size) {

    char* hash_data = (char*) data;
    unsigned long int hash = 0;
    
    for (unsigned long int byte = 0; byte < size; byte++) {    
        hash = hash * 33  + hash_data[byte]; 
    }

    return hash;
}

static unsigned long long int stack_hash(Stack* stack) { 

    unsigned long long int stack_hash = stack->hash;
    stack->hash = 0;

    int data_hash = gnu_hash((char*)stack_get_data_base_ptr(stack), stack->capacity * sizeof(Stack_data));
    int struct_hash = gnu_hash(stack, sizeof(Stack));

    stack->hash = stack_hash;
    return data_hash ^ struct_hash; 
}

static void stack_rehash(Stack* stack) {

    stack->hash = stack_hash(stack);
}


void stack_data_print(Stack* stack, FILE* file) {

    Stack_data* data = stack_get_data_base_ptr(stack);

    for (int i = 0; i < (int)stack->element_count; i++) {
        fprintf(file, "______elem #%d: %d\n", i+1, *(data + i));
    }
}


stack_status stack_init(Stack* stack, size_t length) {

    *stack = (Stack){
        .capacity = length,
        .min_capacity = length,
        .size_multiplier = 2,
        .element_count = 0,
        .hash = 0,
        .data = (char*)calloc(stack_get_buffer_size(length), sizeof(char)),
        .is_alive = true,
    };

   
    debug_msg(INFO_, printf("___init_stack: %p\n", stack));
    debug_msg(INFO_, printf("___init_data: %p\n", stack->data));
    
    set_canaries(stack);

    stack_rehash(stack);
    debug_msg(INFO_, print_canaries(stack, stdout));

    stack_assert_invariants(stack);
    return SUCCESS;
}


stack_status stack_push(Stack* stack, Stack_data element) {

    stack_assert_invariants(stack);
    stack_resize(stack); // TODO: add if_needed in name (and accept target_size)

    *get_ptr_to_element(stack, stack->element_count++) = element;

    stack_rehash(stack);

    return SUCCESS;
}

Stack_data stack_pop(Stack* stack) {

    stack_assert_invariants(stack);

    Stack_data last_element = get_element(stack, stack->element_count-1);
    stack->element_count--;
    
    stack_resize(stack);
    stack_rehash(stack);

    return last_element;
}

Stack_data stack_get_last_element(Stack* stack) {
    return get_element(stack, stack->element_count-1);
}
stack_status stack_resize(Stack* stack) {

    stack_ptr_verify(stack);

    stack_assert_invariants(stack);

    fullness status = get_stack_occupancy_status(stack);

    if (status == FULL) {

        debug_msg(INFO_, puts("length up!"));
        stack_length_up(stack);
    }
    if (status <= UNDER_HALF) {

        debug_msg(INFO_, puts("length down!"));
        stack_length_down(stack);
    }
    set_canaries(stack);
    stack_rehash(stack);

    return SUCCESS;
}

stack_status stack_destruct(Stack* stack) {

    stack_assert_invariants(stack); 
    free(stack->data);

    *stack = (Stack) {
        .capacity = 0,
        .min_capacity = 0,
        .size_multiplier = 0,
        .element_count = 0,
        .hash = 0, 
        .data = NULL,
        .is_alive = false
    };

    return SUCCESS;
}


void stack_dump(Stack* stack, const char* file, const char* func, int line, unsigned status) {
    // TODO:                                                                ^~~~~~~~ maybe it's better to move this in stack

    debug_msg(INFO_, printf("DUMP was called from %s :: %s :: %d\n", file, func, line));
    //                                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ print_source_location?

    FILE* dump_file = fopen("dump.txt", "w"); // TODO: maybe give dump file?
    // TODO:                             ^ maybe it's better to open it in "a"?

    setvbuf(dump_file, NULL, _IONBF, 0); // TODO: use

    assert(dump_file && "opening file error"); // TODO: ooooooooooossert?

    fprintf(dump_file, "DUMP was called from %s :: %s :: %d\n", file, func, line);

    fprintf(dump_file, "The stack address: [%p]\n", stack);

    if (status & STACK_NULL) { 

        fprintf(dump_file, "Dump aborted, stack has NULL pointer");
        fclose(dump_file);
        abort(); // TODO: podumoi, assert(false) would be better, but i would just return, maybe bool
    }

    fprintf(dump_file, "The data begining address: [%p]\n", stack->data);

    if (status & DATA_NULL) {

        fprintf(dump_file, "Dump aborted, data has NULL pointer\n");
        fclose(dump_file);
        abort();
    }

    
    print_size_info(stack, dump_file);
    stack_data_print(stack, dump_file);

    if (status & EMPTY_STACK) {

        fprintf(dump_file, "Removing element from empty stack!\n");
    }

    if (status & STACK_OVERFLOW) {

        fprintf(dump_file, "StackOverflow!\n");
    }

    if (status & INVALID_HASH) {
        
        fprintf(dump_file, "Hash was damaged!\n");
        fprintf(dump_file, "Stack hash: %llu\n", stack->hash);
        fprintf(dump_file, "Actual hash: %llu\n", stack_hash(stack));
    }
    fprintf(dump_file, "Stack hash: %llu\n", stack->hash);

    if (status & DAMAGED_LEFT_CANARY || status & DAMAGED_RIGHT_CANARY) {

        fprintf(dump_file, "Damaged canary(canaries)!\n");
    }
    print_canaries(stack, dump_file); 


    fclose(dump_file), dump_file = NULL;

    return;
}
