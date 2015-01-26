#ifndef PQUEUE_H
#define	PQUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "binary_tree.h"
    
typedef binary_node* pqdt;/* datový typ dat */

typedef struct {
  int top;
  int size;
  int (*comp)(pqdt*, pqdt*);
  pqdt* data;
} pqueue;

pqueue* pqueue_create(int size, int (*comp)(pqdt*, pqdt*));

void pqueue_free(pqueue** pq);

int pqueue_push(pqueue* pq, pqdt* item);

int pqueue_pop(pqueue* pq, pqdt* item);


#ifdef	__cplusplus
}
#endif

#endif	/* PQUEUE_H */

