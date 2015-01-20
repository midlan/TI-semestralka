#include "pqueue.h"
#include <stdlib.h>

/* knihovna prioritní fronty */

/* vytvoří prioritní frontu */
pqueue* pqueue_create(int size, int (*comp)(const void*, const void *)) {

    if(comp == NULL) {
        return NULL;
    }
    
    pqueue *pq = (pqueue*) malloc(sizeof (pqueue));

    if (pq != NULL) {

        pq->comp = comp;
        pq->size = size;
        pq->data = (pqdt*) malloc(pq->size * sizeof (pqdt));

        if (pq->data == NULL) {
            pqueue_free(&pq);
            return NULL;
        }

        pq->top = -1;
    }

    return pq;
}

/* zničí prioritní frontu */
void pqueue_free(pqueue **pq) {
    
    if(pq != NULL) {

        if (*pq != NULL) {

            if ((*pq)->data != NULL) {
                free((*pq)->data);
            }

            free(*pq);
            *pq = NULL;
        }
    }
}

/* přidá prvek do prioritní fronty */
int pqueue_push(pqueue *pq, pqdt *item) {

    if (pq != NULL && item != NULL) {

        /* zkontrolovat jestli nedošlo místo a případně zdvojnásobit úložiště */
        if (pq->top + 1 >= pq->size) {

            int new_size = pq->size * 2;
            char* more_data = (pqdt*) realloc(pq->data, new_size * sizeof (pqdt));

            if (more_data != NULL) {
                pq->size = new_size;
                pq->data = more_data;
            } else
                return 0;
        }

        pq->top++;

        /* vložení nového prvku na poslední pozici úložiště */
        pq->data[pq->top] = item;

        return 1;
    }

    return 0;
}

/* odebere prvek s nejvyšší prioritou, případně překopíruje prvek na pointer */
int pqueue_pop(pqueue *pq, pqdt *item) {

    if (pq != NULL && pq->top >= 0) {
        
        pqdt *max;

        /* nalezení prvku s nejvyšší prioritou */
        for(i = 1; i <= pq->top; i++) {
            if(pq->comp(max, pq->data + i) < 0) {
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