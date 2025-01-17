
#ifndef SAFE_MALLOC_H
#define	SAFE_MALLOC_H

#ifdef	__cplusplus
extern "C" {
#endif

    void free_remaining();
    
    void* safe_malloc(size_t size);

    void* safe_realloc(void* ptr, size_t size);
    
    void safe_free(void* ptr);
    
    /* funkce zavolaná při nedostatku paměti */
    extern void oom_exit();


#ifdef	__cplusplus
}
#endif

#endif	/* SAFE_MALLOC_H */

