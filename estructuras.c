#include <stdlib.h>
#include "estructuras.h"

// --------------------- NODO ---------------------

typedef struct Node {
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

// --------------------- LISTA ---------------------

struct List {
    Node* head;
    Node* tail;
    Node* current;
};

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->head = list->tail = list->current = NULL;
    return list;
}

void push_back(List* list, void* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) list->tail->next = node;
    else list->head = node;

    list->tail = node;
}

void push_front(List* list, void* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->prev = NULL;
    node->next = list->head;

    if (list->head) list->head->prev = node;
    else list->tail = node;

    list->head = node;
}

void insert_current(List* list, void* data) {
    if (!list->current) return;

    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;

    node->prev = list->current;
    node->next = list->current->next;

    if (list->current->next) list->current->next->prev = node;
    else list->tail = node;

    list->current->next = node;
}

void erase_current(List* list) {
    if (!list->current) return;

    Node* to_delete = list->current;

    if (to_delete->prev) to_delete->prev->next = to_delete->next;
    else list->head = to_delete->next;

    if (to_delete->next) to_delete->next->prev = to_delete->prev;
    else list->tail = to_delete->prev;

    list->current = to_delete->next;
    free(to_delete);
}

void pop_back(List* list) {
    if (!list->tail) return;
    list->current = list->tail;
    erase_current(list);
}

void pop_front(List* list) {
    if (!list->head) return;
    list->current = list->head;
    erase_current(list);
}

void* front(List* list) {
    if (!list->head) return NULL;
    list->current = list->head;
    return list->head->data;
}

void* back(List* list) {
    if (!list->tail) return NULL;
    list->current = list->tail;
    return list->tail->data;
}

void* next(List* list) {
    if (!list->current || !list->current->next) return NULL;
    list->current = list->current->next;
    return list->current->data;
}

void* previous(List* list) {
    if (!list->current || !list->current->prev) return NULL;
    list->current = list->current->prev;
    return list->current->data;
}

// --------------------- COLA ---------------------

struct Queue {
    List* list;
};

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->list = createList();
    return q;
}

void push_queue(Queue* queue, void* data) {
    push_back(queue->list, data);
}

void pop_queue(Queue* queue) {
    pop_front(queue->list);
}

void* top_queue(Queue* queue) {
    return front(queue->list);
}

// --------------------- PILA ---------------------

struct Stack {
    List* list;
};

Stack* createStack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->list = createList();
    return s;
}

void push_stack(Stack* stack, void* data) {
    push_back(stack->list, data);
}

void pop_stack(Stack* stack) {
    pop_back(stack->list);
}

void* top_stack(Stack* stack) {
    return back(stack->list);
}