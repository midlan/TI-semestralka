#ifndef PQUEUE_H
#define	PQUEUE_H

typedef binary_node pqdt;/* datový typ dat */

typedef struct {
  int top;
  int size;
  int (*comp)(const void*, const void *);
  pqdt *data;
} pqueue;

pqueue* pqueue_create(int size, int (*comp)(const void*, const void *));

void pqueue_free(pqueue **pq);

int pqueue_push(pqueue *pq, pqdt data);

int pqueue_pop(pqueue *pq, pqdt *item);

#endif