#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estructuras.c"
#define MAX_DESC 300

typedef struct {
    int id;
    char descripcion[MAX_DESC];
    char prioridad[3];
    time_t horaRegistro;
} Ticket;

typedef struct Nodo {
    Ticket ticket;
    struct Nodo* next;
    struct Nodo* prev:
} Nodo;

typedef struct{
    Nodo* head;
    Nodo* tail;
} Cola;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de gestión de tickets de soporte técnico");
  puts("========================================");

  puts("1) Registrar ticket");
  puts("2) Asignar prioridad a su ticket");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente ticket");
  puts("5) Mostrar tickets por prioridad");
  puts("6) Salir");
}

void registrar_ticket(List *tickets) {
  printf("Registrar nuevo paciente\n");
  // Aquí implementarías la lógica para registrar un nuevo paciente
}

void mostrar_lista_tickets(List *tickets) {
  printf("tickets en espera: \n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
}

int main(){
    Queue* colaPriori1 = createQueue();
    Queue* colaPriori2 = createQueue();
    Queue* colaPriori3 = createQueue();
    char opcion;
    List *tickets = list_create();

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
        registrar_tickets(tickets);
        break;
        case '2':
        // Lógica para asignar prioridad
        break;
        case '3':
        mostrar_lista_tickets(tickets);
        break;
        case '4':
        // Lógica para pasar al sig
        break;
        case '5':
        // Lógica para mostrar tickets por prioridad
        break;
        case '6':
        puts("Saliendo del sistema de gestión hospitalaria...");
        break;
        default:
        puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();

    } while (opcion != '6');

    // Liberar recursos, si es necesario
    list_clean(tickets);

    return 0;
}
