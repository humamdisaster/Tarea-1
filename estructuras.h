// estructuras.h
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// --------------------- LISTA ---------------------

typedef struct List List;

List* createList();
void push_back(List* list, void* data);
void push_front(List* list, void* data);
void insert_current(List* list, void* data);
void erase_current(List* list);
void pop_back(List* list);
void pop_front(List* list);
void* front(List* list);
void* back(List* list);
void* next(List* list);
void* previous(List* list);

// --------------------- COLA ---------------------

typedef struct Queue Queue;

Queue* createQueue();
void push_queue(Queue* queue, void* data);
void pop_queue(Queue* queue);
void* top_queue(Queue* queue);

// --------------------- PILA ---------------------

typedef struct Stack Stack;

Stack* createStack();
void push_stack(Stack* stack, void* data);
void pop_stack(Stack* stack);
void* top_stack(Stack* stack);

#endif // ESTRUCTURAS_H



