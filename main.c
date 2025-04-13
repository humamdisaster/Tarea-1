#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "estructuras.h" // Incluimos las definiciones y funciones para listas/colas/pilas

// --- Constantes ---
#define MAX_DESC 300        // Maximo largo para la descripcion de un ticket
#define MAX_PRIO 10         // Maximo largo para el string de prioridad ("Alta", "Media", "Bajo")
#define MAX_ID   50         // Maximo largo para el ID del ticket (como string)
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S" // Formato para mostrar la fecha y hora
#define TIME_BUFFER_SIZE 20 // Tamaño del buffer para guardar la fecha como texto

// --- Estructura de Datos ---

// Estructura para representar un ticket de soporte
typedef struct Ticket {
    char id[MAX_ID];            // Identificador unico del ticket (AHORA ES STRING)
    char descripcion[MAX_DESC]; // Descripcion del problema
    char prioridad[MAX_PRIO];   // Prioridad ("Alta", "Media", "Bajo")
    time_t horaRegistro;        // Momento en que se creo el ticket
} Ticket;

// --- Variables Globales ---

// Punteros a las colas para cada nivel de prioridad. Se inicializan en main()
Queue *colaAlto = NULL;
Queue *colaMedio = NULL;
Queue *colaBajo = NULL;
// Ya no usamos proxId porque pedimos el ID al usuario

// --- Funciones Auxiliares ---

// Convierte un timestamp (time_t) a un string con formato legible
void formatTime(time_t time, char *buffer, size_t bufferSize) {
    struct tm *tm_info; // Estructura para descomponer el tiempo
    tm_info = localtime(&time); // Convierte time_t a tiempo local
    // Formatea el tiempo en el buffer segun TIME_FORMAT
    strftime(buffer, bufferSize, TIME_FORMAT, tm_info);
}

// Muestra en pantalla los detalles de un ticket especifico
void mostrarDetallesTicket(Ticket *ticketProcesado) {
    // Si el puntero es nulo, no hacemos nada
    if (ticketProcesado == NULL) return;
    char buffer[TIME_BUFFER_SIZE]; // Buffer temporal para la fecha formateada
    // Formateamos la hora de registro para mostrarla
    formatTime(ticketProcesado->horaRegistro, buffer, sizeof(buffer));
    // Imprimimos los campos del ticket
    printf("  ID: %s\n", ticketProcesado->id);
    printf("  Descripcion: %s\n", ticketProcesado->descripcion);
    printf("  Prioridad: %s\n", ticketProcesado->prioridad);
    printf("  Hora de Registro: %s\n", buffer);
    printf("  ------------------------------------\n");
}
 // Verifica si el ID ya existe en alguna de las colas
bool idExiste(const char *id) {
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};
    // Recorremos las 3 colas
    for (int i = 0; i < 3; i++) {
        Queue *colaActual = colas[i];
        if (colaActual == NULL) continue; // Si la cola es nula, saltamos

        List* listaInter = colaActual->list; // Obtenemos la lista de la cola
        void* data = front(listaInter); // Obtenemos el primer elemento de la lista
        // Recorremos la lista de tickets actual
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            // Comparamos el ID del ticket con el ID ingresado
            if (strcmp(ticket->id, id) == 0) {
                return true; // ID ya existe
            }
            data = next(listaInter); // Avanzamos al siguiente ticket
        }
    }
    return false; // ID no existe
}

// --- Funciones Principales del Sistema ---

// 1. Registra un nuevo ticket en el sistema (pide ID y verifica unicidad)
void registrarTicket() {
    Ticket *nuevoTicket = (Ticket *)malloc(sizeof(Ticket));
    if (nuevoTicket == NULL) {
        printf("Error: No se pudo asignar memoria para el nuevo ticket.\n");
        return;
    }

    char bufferDesc[MAX_DESC];
    char bufferId[MAX_ID];

    // Pedir ID
    printf("Ingrese el ID del ticket (maximo %d caracteres): ", MAX_ID - 1);
    if (fgets(bufferId, sizeof(bufferId), stdin) != NULL) {
        bufferId[strcspn(bufferId, "\n")] = '\0';
        // Validar que no sea una cadena vacía
        if (strlen(bufferId) == 0) {
             printf("Error: El ID no puede estar vacío.\n");
             free(nuevoTicket);
             return;
        }
        strncpy(nuevoTicket->id, bufferId, MAX_ID - 1);
        nuevoTicket->id[MAX_ID - 1] = '\0';
    } else {
        printf("Error al leer el ID del ticket.\n");
        free(nuevoTicket);
        return;
    }

    // Verificación de ID
    if (idExiste(nuevoTicket->id)) {
        printf("\n*** Error: El ID '%s' ya existe en el sistema. ***\n", nuevoTicket->id);
        printf("Por favor, intente registrar el ticket con un ID diferente.\n");
        free(nuevoTicket); // Liberamos la memoria porque no lo vamos a usar
        return;            // Salimos de la función, no se registra nada
    }
    // Fin Verificación

    // Pedir Descripción
    printf("Ingrese la descripcion del ticket (maximo %d caracteres): ", MAX_DESC - 1);
    if (fgets(bufferDesc, sizeof(bufferDesc), stdin) != NULL) {
        bufferDesc[strcspn(bufferDesc, "\n")] = '\0';
        strncpy(nuevoTicket->descripcion, bufferDesc, MAX_DESC - 1);
        nuevoTicket->descripcion[MAX_DESC - 1] = '\0';
    } else {
        printf("Error al leer la descripcion del ticket.\n");
        free(nuevoTicket);
        return;
    }

    // Asignar Prioridad y Hora
    strncpy(nuevoTicket->prioridad, "Bajo", MAX_PRIO -1);
    nuevoTicket->prioridad[MAX_PRIO - 1] = '\0';
    nuevoTicket->horaRegistro = time(NULL);

    // Agregar a la Cola
    push_queue(colaBajo, nuevoTicket);

    printf("\nTicket registrado con exito!\n");
    mostrarDetallesTicket(nuevoTicket);
}

// Muestra todos los tickets dentro de una cola especifica
void mostrarTicketsEnCola(Queue *cola, const char *nombreCola) {
    List* listaInter = cola->list;
    printf("--- Tickets Prioridad %s ---\n", nombreCola);
    void* data = front(listaInter);

    if (data == NULL) {
        printf("   (No hay tickets en esta cola)\n");
    } else {
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            mostrarDetallesTicket(ticket);
            data = next(listaInter);
        }
    }
    printf("\n"); // Salto de línea después de mostrar cada cola
}

// 3. Muestra todos los tickets pendientes, ordenados por prioridad y llegada
void mostrarTicketsPendientes(){
    printf("\n===== LISTA DE TICKETS PENDIENTES =====\n\n");
    mostrarTicketsEnCola(colaAlto, "Alta");
    mostrarTicketsEnCola(colaMedio, "Media");
    mostrarTicketsEnCola(colaBajo, "Baja");

    if (top_queue(colaAlto) == NULL && top_queue(colaMedio) == NULL && top_queue(colaBajo) == NULL){
        printf("¡No hay tickets pendientes en el sistema!\n\n");
    }
}

// 4. Procesa el siguiente ticket segun prioridad
void procesarNextTicket(){
    Ticket *ticketAProcesar = NULL;
    Queue *colaPrioridad = NULL;

    printf("\n===== PROCESANDO SIGUIENTE TICKET =====\n");

    if (top_queue(colaAlto) != NULL) {
        colaPrioridad = colaAlto;
        printf("Procesando ticket de ALTA prioridad:\n");
    } else if (top_queue(colaMedio) != NULL) {
        colaPrioridad = colaMedio;
        printf("Procesando ticket de MEDIA prioridad:\n");
    } else if (top_queue(colaBajo) != NULL) {
        colaPrioridad = colaBajo;
        printf("Procesando ticket de BAJA prioridad:\n");
    }

    if (colaPrioridad != NULL){
        ticketAProcesar = (Ticket *)top_queue(colaPrioridad);
        mostrarDetallesTicket(ticketAProcesar);
        pop_queue(colaPrioridad);
        printf("Ticket ID %s procesado y eliminado de la cola.\n", ticketAProcesar->id);
        free(ticketAProcesar);
    } else {
        printf("No hay tickets pendientes para procesar.\n");
    }
    printf("=======================================\n");
}

// 5. Busca un ticket por su ID en todas las colas
void buscarYMostrarTicketPorId(){
    char idBuscar[MAX_ID]; // El ID que ingresara el usuario
    Ticket *ticketEncontrado = NULL;
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};

    printf("Ingrese el ID del ticket a buscar: ");
    // Leemos el ID como string
    if (fgets(idBuscar, sizeof(idBuscar), stdin) != NULL) {
        idBuscar[strcspn(idBuscar, "\n")] = '\0'; // Limpiar el salto de linea
    } else {
        printf("Error al leer el ID del ticket.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        Queue *colaActual = colas[i];
        List* listaInter = colaActual->list;
        void* data = front(listaInter);

        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            // Comparamos IDs
            if (strcmp(ticket->id, idBuscar) == 0) {
                ticketEncontrado = ticket;
                break;
            }
            data = next(listaInter);
        }
        if (ticketEncontrado != NULL) {
            break;
        }
    }

    printf("\n===== BUSCAR TICKET POR ID =====\n");
    if (ticketEncontrado != NULL) {
        printf("Ticket encontrado:\n");
        mostrarDetallesTicket(ticketEncontrado);
    } else {
        printf("No se encontro un ticket con ID %s.\n", idBuscar);
    }
    printf("================================\n");
}

// 2. Permite cambiar la prioridad de un ticket existente
void asignarPrioridad(){
    char idTicket[MAX_ID];       // ID del ticket a modificar
    char nuevaPrioridad[MAX_PRIO]; // String para la nueva prioridad
    Ticket *ticketEncontrado = NULL;
    Queue *colaOrigen = NULL;
    Queue *colaDestino = NULL;
    bool encontrado = false;

    printf("Ingrese el ID del ticket a modificar: ");
    if (fgets(idTicket, sizeof(idTicket), stdin) == NULL) {
        printf("Error al leer el ID del ticket.\n");
        return;
    }
    idTicket[strcspn(idTicket, "\n")] = '\0'; // Limpiar salto de linea

    printf("Ingrese la nueva prioridad (Alta, Media, Baja): ");
    if (fgets(nuevaPrioridad, sizeof(nuevaPrioridad), stdin) != NULL) {
        nuevaPrioridad[strcspn(nuevaPrioridad, "\n")] = '\0';
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
        void* data = front(listaInter);
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            if (strcmp(ticket->id, idTicket) == 0) {
                ticketEncontrado = ticket;
                erase_current(listaInter); // Eliminar de la lista origen
                encontrado = true;
                break;
            }
            data = next(listaInter);
        }
        if (encontrado) break;
    }

    if (encontrado && ticketEncontrado != NULL) {
        strncpy(ticketEncontrado->prioridad, nuevaPrioridad, MAX_PRIO - 1);
        ticketEncontrado->prioridad[MAX_PRIO - 1] = '\0';
        push_queue(colaDestino, ticketEncontrado); // Mover a la cola destino
        // Mostrar ID
        printf("\nPrioridad del ticket ID %s actualizada a '%s'.\n", idTicket, nuevaPrioridad);
        mostrarDetallesTicket(ticketEncontrado);
    } else {
        // Mostrar ID
        printf("No se encontro un ticket con ID %s.\n", idTicket);
    }
}

// Libera la memoria de todos los tickets que quedaron en las colas al salir
void liberarColas(){
    printf("Liberando memoria de tickets restantes...\n");
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};
    int ticketsLiberados = 0;
    for(int i = 0; i < 3; i++){
        Queue *colaActual = colas[i];
        while (top_queue(colaActual) != NULL){
            Ticket *ticketPtr = (Ticket *)top_queue(colaActual);
            pop_queue(colaActual);
            free(ticketPtr);
            ticketsLiberados++;
        }
    }
     if (ticketsLiberados > 0) {
        printf("Memoria de %d tickets liberada.\n", ticketsLiberados);
     } else {
        printf("No habia tickets pendientes para liberar.\n");
     }
}

// Lee la opcion del menu de forma mas segura usando fgets y sscanf
int leerOpcion(){
    char linea[10]; // Buffer suficiente para leer un numero y el enter
    int opcion = -1; // Valor por defecto para opcion invalida

    if (fgets(linea, sizeof(linea), stdin) != NULL) {
        // Intentamos convertir la linea leida a un entero
        // sscanf devuelve 1 si pudo convertir un entero exitosamente
        if (sscanf(linea, "%d", &opcion) == 1) {
            // Validamos que este en el rango permitido (0 a 5)
            if (opcion >= 0 && opcion <= 5) {
                return opcion; // Opcion valida
            }
        }
        // Si sscanf fallo o el numero esta fuera de rango, opcion sigue siendo -1
    }
    // Si fgets falla o la conversion/validacion falla, devolvemos -1
    return -1;
}

// --- Funcion Principal ---
int main(){
    int opcion;

    colaAlto = createQueue();
    colaMedio = createQueue();
    colaBajo = createQueue();

    if (colaAlto == NULL || colaMedio == NULL || colaBajo == NULL) {
        printf("Error al crear las colas. Saliendo.\n");
        return 1;
    }

    printf("=============================================\n");
    printf(" Sistema de Gestion de Tickets de Soporte\n");
    printf("=============================================\n");

    do{
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Registrar nuevo ticket\n");
        printf("2. Asignar prioridad a ticket\n");
        printf("3. Mostrar tickets pendientes\n");
        printf("4. Procesar siguiente ticket\n");
        printf("5. Buscar ticket por ID\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");

        opcion = leerOpcion(); // Usamos la nueva funcion para leer opcion

        if (opcion == -1) { // Si la opcion leida no es valida
            printf("Opcion invalida. Por favor, ingrese un numero entre 0 y 5.\n");
            continue; // Saltar al siguiente ciclo
        }
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
           // No necesitamos default porque leerOpcion ya valida el rango
        }
       // printf("\n"); // Espacio opcional despues de la accion

    } while (opcion != 0);

    liberarColas();
    printf("Programa terminado.\n");

    return 0;
}
