#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_DESC 300


typedef struct {
    int id;
    char descripcion[MAX_DESC];
    char prioridad[3];
    time_t horaRegistro;
} Ticket;

typedef struct Nodo {
    Ticket ticket;
    struct Nodo* siguiente;
} Nodo;

typedef struct{
    Nodo* head;
    Nodo* tail;
} Cola;

