
#include <stdio.h>
#include <stdlib.h>

#include "safe_malloc.h"

void free_remaining() {
    //todo uvolnit všechny adresy ze seznamu alokovaných
}

void* safe_malloc(size_t size) {
    
    void* mem = malloc(size);
    
    if(mem == NULL) {
        free_remaining();
        oom_exit();
    }
    
    return mem;
}

void* safe_realloc(void* ptr, size_t size) {
    
    void* mem = realloc(ptr, size);
    
    if(mem == NULL) {
        free_remaining();
        oom_exit();
    }
    
    return mem;
}

void safe_free(void* ptr) {
    //todo odebrat adresu ze seznamu alokovaných
    free(ptr);
}