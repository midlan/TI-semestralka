#ifndef PQUEUE_H
#define	PQUEUE_H

typedef binary_node pqdt;/* datový typ dat */

typedef struct {
  int top;
  int size;
  int (*comp)(const void*, const void *);
  pqdt *data;
} pqueue;

pqueue* pqueue_create(int size);

void pqueue_free(pqueue **s);

int pqueue_push(pqueue *pq, pqpt *priority, pqdt *data);

int pqueue_pop(pqueue *pq, pqpt *priority, pqdt *data);

#endif