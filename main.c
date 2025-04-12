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

void mostrarDetallesTicket(Ticket *ticketProcesado) {
    if (ticketProcesado == NULL) return;
    char buffer[TIME_BUFFER_SIZE];
    formatTime(ticketProcesado->horaRegistro, buffer, sizeof(buffer));
    printf("ID: %d\n", ticketProcesado->id);
    printf("Descripcion: %s\n", ticketProcesado->descripcion);
    printf("Prioridad: %s\n", ticketProcesado->prioridad);
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
    nuevoTicket->id = proxId++;

    printf("Ingrese la descripcion del ticket (maximo 300 caracteres): ");
    if (fgets(bufferDesc, sizeof(bufferDesc), stdin) != NULL) {
        bufferDesc[strcspn(bufferDesc, "\n")] = '\0'; // Eliminar el salto de línea
        strncpy(nuevoTicket->descripcion, bufferDesc, MAX_DESC);
        nuevoTicket->descripcion[MAX_DESC - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo

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

    printf("\nTicket registrado con exito!\n");
    mostrarDetallesTicket(nuevoTicket);
}

void mostrarTicketsEnCola(Queue *cola, const char *nombreCola) {
    List* listaInter = cola->list;
    printf("--- Tickets Prioridad %s ---\n", nombreCola);

    void* data = front(listaInter);

    if (data == NULL) {
        printf("No hay tickets en la cola %s.\n", nombreCola);
        return;
    }
    while (data != NULL) {
        Ticket *ticket = (Ticket *)data;
        mostrarDetallesTicket(ticket);
        data = next(listaInter);
    }
    printf("\n");
}

void mostrarTicketsPendientes(){
    printf("\nTickets pendientes:\n");
    mostrarTicketsEnCola(colaAlto, "Alta");
    mostrarTicketsEnCola(colaMedio, "Media");
    mostrarTicketsEnCola(colaBajo, "Baja");
    printf("\n");
    if (top_queue(colaAlto) == NULL && top_queue(colaMedio) == NULL && top_queue(colaBajo) == NULL){
        printf("No hay tickets pendientes.\n");
    }
}

void procesarNextTicket(){
    Ticket *ticketAProcesar = NULL;
    Queue *colaPrioridad = NULL;

    printf("\nProcesando siguiente ticket:\n");
    if (top_queue(colaAlto) != NULL) {
        colaPrioridad = colaAlto;
        printf("Ticket prioridad Alta\n");
    } else if (top_queue(colaMedio) != NULL) {
        colaPrioridad = colaMedio;
        printf("Ticket prioridad Media\n");
    } else if (top_queue(colaBajo) != NULL) {
        colaPrioridad = colaBajo;
        printf("Ticket prioridad Baja\n");
    }
    if (colaPrioridad != NULL){
        ticketAProcesar = (Ticket *)top_queue(colaPrioridad);
        mostrarDetallesTicket(ticketAProcesar);
        pop_queue(colaPrioridad);
        printf("Ticket procesado y eliminado de la cola.\n");
        free(ticketAProcesar);
    } else {
        printf("No hay tickets pendientes.\n");
    }
    printf("\n");
}

void buscarYMostrarTicketPorId(){
    int idBuscar;
    Ticket *ticketEncontrado = NULL;
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};

    printf("Ingrese el ID del ticket a buscar: ");
    if (scanf("%d", &idBuscar) != 1) {
        printf("Error al leer el ID del ticket.\n");
        return;
    }
    getchar(); // Limpiar el buffer de entrada

    for (int i = 0; i < 3; i++) {
        Queue *colaActual = colas[i];
        List* listaInter = colaActual->list;
        void* data = front(listaInter);

        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            if (ticket->id == idBuscar) {
                ticketEncontrado = ticket;
                break;
            }
            data = next(listaInter);
        }
        if (ticketEncontrado != NULL) {
            break;
        }
    }
    printf("\nBuscar Ticket por ID:\n");
    if (ticketEncontrado != NULL) {
        printf("Ticket encontrado:\n");
        mostrarDetallesTicket(ticketEncontrado);
    } else {
        printf("Error: No se encontro un ticket con ID %d.\n", idBuscar);
    }
    printf("\n");
}

void asignarPrioridad(){
    int idTicket;
    char nuevaPrioridad[MAX_PRIO];
    Ticket *ticketEncontrado = NULL;
    Queue *colaOrigen = NULL;
    Queue *colaDestino = NULL;
    bool encontrado = false;

    printf("Ingrese el ID del ticket a modificar: ");
    if (scanf("%d", &idTicket) != 1) {
        printf("Error al leer el ID del ticket.\n");
        getchar();
        return;
    }
    getchar();

    printf("Ingrese la nueva prioridad (Alta, Media, Baja): ");
    if (fgets(nuevaPrioridad, sizeof(nuevaPrioridad), stdin) != NULL) {
        nuevaPrioridad[strcspn(nuevaPrioridad, "\n")] = '\0'; // Eliminar el salto de línea
    } else {
        printf("Error al leer la nueva prioridad.\n");
        return;
    }

    if (strcmp(nuevaPrioridad, "Alta") == 0) {
        colaDestino = colaAlto;
    } else if (strcmp(nuevaPrioridad, "Media") == 0) {
        colaDestino = colaMedio;
    } else if (strcmp(nuevaPrioridad, "Baja") == 0) {
        colaDestino = colaBajo;
    } else {
        printf("Prioridad no valida. Debe ser Alta, Media o Baja.\n");
        return;
    }

    Queue *colas[] = {colaAlto, colaMedio, colaBajo};
    for (int i = 0; i < 3; i++){
        colaOrigen = colas[i];
        List* listaInter = colaOrigen->list;
        void* data = front(listaInter); //pone el current de la lista al inicio
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            if (ticket->id == idTicket) {
                ticketEncontrado = ticket;
                // Eliminar el ticket de la cola de origen
                erase_current(listaInter);
                encontrado = true;
                break;
            }
            data = next(listaInter);
        }
        if (encontrado) break;
    }
    if (encontrado && ticketEncontrado != NULL) {
        // Asignar la nueva prioridad
        strncpy(ticketEncontrado->prioridad, nuevaPrioridad, MAX_PRIO - 1);
        ticketEncontrado->prioridad[MAX_PRIO - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
        // Agregar el ticket a la cola de destino
        push_queue(colaDestino, ticketEncontrado);
        printf("Ticket con ID %d modificado a prioridad %s.\n", idTicket, nuevaPrioridad);
        mostrarDetallesTicket(ticketEncontrado);
    } else {
        printf("No se encontro un ticket con ID %d.\n", idTicket);
    }
}

void liberarColas(){
    printf("Liberando memoria de tickets restantes.\n");
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};
    for(int i = 0; i < 3; i++){
        Queue *colaActual = colas[i];
        while (top_queue(colaActual) != NULL){
            Ticket *ticketPtr = (Ticket *)top_queue(colaActual);
            pop_queue(colaActual);
            free(ticketPtr);
        }
        
    }
}

int main(){  //menú principal
    int opcion;

    colaAlto = createQueue();
    colaMedio = createQueue();
    colaBajo = createQueue();

    if (colaAlto == NULL || colaMedio == NULL || colaBajo == NULL) {
        printf("Error al crear las colas.\n");
        return 1;
    }
    printf("=============================================\n");
    printf(" Sistema de Gestion de Tickets de Soporte\n");
    printf("=============================================\n");

    do{
        printf("--- MENU PRINCIPAL ---\n");
        printf("1. Registrar nuevo ticket\n");
        printf("2. Asignar prioridad a ticket\n");
        printf("3. Mostrar tickets pendientes\n");
        printf("4. Procesar siguiente ticket\n");
        printf("5. Buscar ticket por ID\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Opcion invalida. Por favor, ingrese un numero.\n");
            getchar();
            continue;
        }
        getchar();

        printf("\n");
        switch(opcion){
            case 1:
                registrarTicket();
                break;
            case 2:
                asignarPrioridad();
                break;
            case 3:
                mostrarTicketsPendientes();
                break;
            case 4:
                procesarNextTicket();
                break;
            case 5:
                buscarYMostrarTicketPorId();
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Por favor, intente de nuevo.\n");
        }
        printf("\n");
    } while (opcion != 0);

    liberarColas();

    return 0;
}