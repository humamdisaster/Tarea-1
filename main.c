#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "estructuras.h"
#define MAX_DESC 300
#define MAX_PRIO 10
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define TIME_BUFFER_SIZE 20


typedef struct Ticket {
    int id;
    char descripcion[MAX_DESC];
    char prioridad[MAX_PRIO];
    time_t horaRegistro;
} Ticket;

Queue *colaAlto;
Queue *colaMedio;
Queue *colaBajo;
int proxId = 1;

void formatTime(time_t time, char *buffer, size_t bufferSize) {
    struct tm *tm_info;
    tm_info = localtime(&time);
    strftime(buffer, bufferSize, TIME_FORMAT, tm_info);
}

void mostrarDetallesTicket(Ticket *t){
    if (t == NULL) return;
    char buffer[TIME_BUFFER_SIZE];
    formatTime(t->horaRegistro, buffer, sizeof(buffer));
    printf("ID: %d\n", t->id);
    printf("Descripcion: %s\n", t->descripcion);
    printf("Prioridad: %s\n", t->prioridad);
    printf("Hora de Registro: %s\n", buffer);
    printf("------------------------------------\n");
}

void registrarTicket(){
    Ticket *nuevoTicket = (Ticket *)malloc(sizeof(Ticket));
    if (nuevoTicket == NULL) {
        printf("Error al asignar memoria para el nuevo ticket.\n");
        return;
    }

    char bufferDesc[MAX_DESC];

    printf("Ingrese la descripcion del ticket (máximo 300 carácteres): ");
    if (fgets(buffer_desc, sizeof(bufferDesc), stdin) != NULL) {
        bufferDesc[strcspn(bufferDesc, "\n")] = '\0'; // Eliminar el salto de línea
    }
    else{
        printf("Error al leer la descripcion del ticket.\n");
        free(nuevoTicket);
        return;
    }
    strncpy(nuevoTicket->prioridad, "Bajo", MAX_PRIO -1);
    nuevoTicket->prioridad[MAX_PRIO - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
    nuevoTicket->horaRegistro = time(NULL);

    push_queue(colaBajo, nuevoTicket);

    printf("\nTicket registrado con éxito!\n");
    mostrarDetallesTicket(nuevoTicket);
}

void mostrarTicketsEnCola(Queue *cola, const char *nombreCola) {
    List *lista = cola->lista;

    printf("--- Tickets Prioridad %s ---\n", nombre_prioridad);
}