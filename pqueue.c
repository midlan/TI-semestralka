
#include <stdlib.h>

#include "pqueue.h"
#include "safe_malloc.h"

/* knihovna prioritní fronty */

/* vytvoří prioritní frontu */
pqueue* pqueue_create(int size, int (*comp)(const void*, const void *)) {

    pqueue *pq;
    
    if(comp == NULL) {
        return NULL;
    }
    
    pq = (pqueue*) safe_malloc(sizeof (pqueue));

    pq->comp = comp;
    pq->size = size;
    pq->data = (pqdt*) safe_malloc(pq->size * sizeof (pqdt));
    pq->top = -1;

    return pq;
}

/* zničí prioritní frontu */
void pqueue_free(pqueue **pq) {
    
    if(pq != NULL) {

        if (*pq != NULL) {

            if ((*pq)->data != NULL) {
                safe_free((*pq)->data);
            }

            safe_free(*pq);
            *pq = NULL;
        }
    }
}

/* přidá prvek do prioritní fronty */
int pqueue_push(pqueue *pq, pqdt *item) {

    if (pq != NULL && item != NULL) {

        /* zkontrolovat jestli nedošlo místo a případně zdvojnásobit úložiště */
        if (pq->top + 1 >= pq->size) {

            pq->size *= 2;
            pq->data = (pqdt*) safe_realloc(pq->data, pq->size * sizeof (pqdt));
        }

        pq->top++;

        /* vložení nového prvku na poslední pozici úložiště */
        pq->data[pq->top] = *item;

        return 1;
    }

    return 0;
}

/* odebere prvek s nejvyšší prioritou, případně překopíruje prvek na pointer */
int pqueue_pop(pqueue *pq, pqdt *item) {

    if (pq != NULL && pq->top >= 0) {
        
        pqdt *max = pq->data;
        int i;
        
        /* nalezení prvku s nejvyšší prioritou */
        for(i = 1; i <= pq->top; i++) {
            if(pq->comp(pq->data + i, max) < 0) {
                max = pq->data + i;
            }
        }

        if(item != NULL) {
            *item = *max;
        }

        /* pokud prvek nebyl na poslední pozici, nakopírovat na jeho místo poslední prvek */
        if(max != pq->data + pq->top) {
            *max = pq->data[pq->top];
        }

        /* označit paměť zabranou posledním prvkem jako volnou */
        pq->top--;

        return 1;
    }

    return 0;
}