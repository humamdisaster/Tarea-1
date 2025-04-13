#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "estructuras.h" // Incluimos las definiciones y funciones para listas/colas/pilas

// --- Constantes ---
#define MAX_DESC 300       // Maximo largo para la descripcion de un ticket
#define MAX_PRIO 10        // Maximo largo para el string de prioridad ("Alta", "Media", "Bajo")
#define MAX_ID 50       // Maximo largo para el ID del ticket (en caso de que se use un string)
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S" // Formato para mostrar la fecha y hora
#define TIME_BUFFER_SIZE 20 // Tamaño del buffer para guardar la fecha como texto

// --- Estructura de Datos ---

// Estructura para representar un ticket de soporte
typedef struct Ticket {
    char id[MAX_ID];                     // Identificador unico del ticket
    char descripcion[MAX_DESC]; // Descripcion del problema
    char prioridad[MAX_PRIO];   // Prioridad ("Alto", "Media", "Bajo")
    time_t horaRegistro;        // Momento en que se creo el ticket
} Ticket;

// --- Variables Globales ---

// Punteros a las colas para cada nivel de prioridad. Se inicializan en main()
Queue *colaAlto = NULL;
Queue *colaMedio = NULL;
Queue *colaBajo = NULL;

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
    printf("  ID: %d\n", ticketProcesado->id);
    printf("  Descripcion: %s\n", ticketProcesado->descripcion);
    printf("  Prioridad: %s\n", ticketProcesado->prioridad);
    printf("  Hora de Registro: %s\n", buffer);
    printf("  ------------------------------------\n");
}

// --- Funciones Principales del Sistema ---

// 1. Registra un nuevo ticket en el sistema
void registrarTicket(){
    // Pedimos memoria para guardar los datos del nuevo ticket
    // Es necesario porque la cola guarda solo el puntero
    Ticket *nuevoTicket = (Ticket *)malloc(sizeof(Ticket));
    if (nuevoTicket == NULL) {
        // Si no hay memoria, avisamos y salimos
        printf("Error: No se pudo asignar memoria para el nuevo ticket.\n");
        return;
    }

    char bufferDesc[MAX_DESC]; // Buffer temporal para leer la descripcion
    char bufferId[MAX_ID]; // Buffer temporal para leer el ID
 
    printf ("Ingrese el ID del ticket (maximo %d caracteres): ", MAX_ID);
    if (fgets(bufferId, sizeof(bufferId), stdin) != NULL) {
        // Quitamos el salto de linea ('\n') que deja fgets al final
        bufferId[strcspn(bufferId, "\n")] = '\0';
        // Copiamos el ID al ticket (con cuidado de no pasarnos del limite)
        strncpy(nuevoTicket->id, bufferId, MAX_ID);
        nuevoTicket->id[MAX_ID - 1] = '\0'; // Importante: asegurar que termine en nulo
    } else {
        // Si hubo un error leyendo, avisamos y liberamos la memoria
        printf("Error al leer el ID del ticket.\n");
        free(nuevoTicket); // Liberar memoria reservada si no se pudo completar
        return;
    }

    // Pedimos la descripcion al usuario
    printf("Ingrese la descripcion del ticket (maximo %d caracteres): ", MAX_DESC);
    if (fgets(bufferDesc, sizeof(bufferDesc), stdin) != NULL) {
        // Quitamos el salto de linea ('\n') que deja fgets al final
        bufferDesc[strcspn(bufferDesc, "\n")] = '\0';
        // Copiamos la descripcion al ticket (con cuidado de no pasarnos del limite)
        strncpy(nuevoTicket->descripcion, bufferDesc, MAX_DESC - 1);
        nuevoTicket->descripcion[MAX_DESC - 1] = '\0'; // Importante: asegurar que termine en nulo
    }
 else{
        // Si hubo un error leyendo, avisamos y liberamos la memoria
        printf("Error al leer la descripcion del ticket.\n");
        free(nuevoTicket); // Liberar memoria reservada si no se pudo completar
        return;
    }
    // Asignamos prioridad "Bajo" por defecto y registramos la hora actual
    strncpy(nuevoTicket->prioridad, "Bajo", MAX_PRIO -1);
    nuevoTicket->prioridad[MAX_PRIO - 1] = '\0';
    nuevoTicket->horaRegistro = time(NULL); // time(NULL) nos da la hora actual
    
    // Agregamos el puntero al nuevo ticket a la cola de prioridad baja
    push_queue(colaBajo, nuevoTicket);

    printf("\nTicket registrado con exito!\n");
    mostrarDetallesTicket(nuevoTicket); // Mostramos el ticket que acabamos de crear
}

// Muestra todos los tickets dentro de una cola especifica
// Necesita acceder a la lista interna de la cola porque la interfaz de Queue es limitada
void mostrarTicketsEnCola(Queue *cola, const char *nombreCola) {
    // Obtenemos la lista que esta "dentro" de la cola (esto depende de estructuras.c)
    List* listaInter = cola->list;
    printf("--- Tickets Prioridad %s ---\n", nombreCola);

    // Empezamos a recorrer la lista desde el principio
    void* data = front(listaInter);

    if (data == NULL) {
        // Si front() devuelve NULL, la lista esta vacia
        printf("No hay tickets en la cola %s.\n", nombreCola);
        return;
    }
    // Mientras haya elementos en la lista...
    while (data != NULL) {
        // Convertimos el puntero generico (void*) al tipo que sabemos que es (Ticket*)
        Ticket *ticket = (Ticket *)data;
        // Mostramos los detalles de este ticket
        mostrarDetallesTicket(ticket);
        // Avanzamos al siguiente elemento de la lista
        data = next(listaInter);
    }
    printf("\n");
}

// 3. Muestra todos los tickets pendientes, ordenados por prioridad y llegada
void mostrarTicketsPendientes(){
    printf("\n===== LISTA DE TICKETS PENDIENTES =====\n\n");
    // Llamamos a la funcion auxiliar para cada cola de prioridad
    mostrarTicketsEnCola(colaAlto, "Alta");
    mostrarTicketsEnCola(colaMedio, "Media");
    mostrarTicketsEnCola(colaBajo, "Baja");

    // Verificamos si todas las colas estan vacias mirando el elemento 'top'
    // top_queue devuelve NULL si la cola esta vacia
    if (top_queue(colaAlto) == NULL && top_queue(colaMedio) == NULL && top_queue(colaBajo) == NULL){
        printf("No hay tickets pendientes.\n");
    }
}

void procesarNextTicket(){
    Ticket *ticketAProcesar = NULL;
    Queue *colaPrioridad = NULL;

     printf("\n===== PROCESANDO SIGUIENTE TICKET =====\n");

    // Buscamos la cola no vacia con mayor prioridad (Alto > Medio > Bajo)
    if (top_queue(colaAlto) != NULL) { // Si hay tickets en Alta
        colaPrioridad = colaAlto;
        printf("Ticket prioridad Alta\n");
    } else if (top_queue(colaMedio) != NULL) { // Si no, si hay en Media
        colaPrioridad = colaMedio;
        printf("Ticket prioridad Media\n");
    } else if (top_queue(colaBajo) != NULL) { // Si no, si hay en Baja
        colaPrioridad = colaBajo;
        printf("Ticket prioridad Baja\n");
    }
    // Si encontramos una cola con tickets...
    if (colaPrioridad != NULL){
        // 1. Obtenemos el puntero al primer ticket (sin sacarlo aun)
        ticketAProcesar = (Ticket *)top_queue(colaPrioridad);
        // 2. Mostramos sus detalles
        mostrarDetallesTicket(ticketAProcesar);
        // 3. Lo sacamos de la cola (ahora si se elimina)
        pop_queue(colaPrioridad);
        printf("Ticket ID %d procesado y eliminado de la cola.\n", ticketAProcesar->id);
        // 4. ¡Importante! Liberamos la memoria que pedimos con malloc para este ticket
        free(ticketAProcesar);
    } else {
        // Si todas las colas estaban vacias
        printf("No hay tickets pendientes para procesar.\n");
    }
    printf("=======================================\n");
}

// 5. Busca un ticket por su ID en todas las colas y muestra sus detalles si lo encuentra
void buscarYMostrarTicketPorId(){
    char idBuscar[MAX_ID]; // El ID que ingresara el usuario
    Ticket *ticketEncontrado = NULL; // Para guardar el puntero si lo encontramos
    // Un array con punteros a nuestras colas para poder recorrerlas facilmente
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};

    printf("Ingrese el ID del ticket a buscar: ");
    // Validacion basica de entrada
    if (fghets(idBuscar, sizeof(idBuscar), stdin) != NULL) {
        idBuscar[strcspn(idBuscar, "\n")] = '\0'; // Limpiar el salto de linea
    } else {
        printf("Error al leer el ID del ticket.\n");
        return;
    }
    // Recorremos cada una de las colas (Alta, Media, Baja)
    for (int i = 0; i < 3; i++) {
        Queue *colaActual = colas[i];
        // Accedemos a la lista interna para poder recorrerla
        List* listaInter = colaActual->list;
        void* data = front(listaInter); // Empezamos por el primer elemento

        // Recorremos la lista interna de la cola actual
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data; // Convertimos a Ticket*
            // Comparamos el ID del ticket actual con el buscado
            if (strcmp(ticket->id, idBuscar) == 0) {
                ticketEncontrado = ticket; // Guardamos el puntero
                break; // Salimos del while, ya no necesitamos buscar en esta cola
            }
            data = next(listaInter); // Pasamos al siguiente elemento de la lista
        }
        if (ticketEncontrado != NULL) {
            break; // Salimos del for, ya lo encontramos en alguna cola
        }
    }
    // Mostramos el resultado de la busqueda
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
    int idTicket; // ID del ticket a modificar
    char nuevaPrioridad[MAX_PRIO]; // String para la nueva prioridad
    Ticket *ticketEncontrado = NULL; // Puntero al ticket si lo encontramos
    Queue *colaOrigen = NULL; // De donde sacamos el ticket
    Queue *colaDestino = NULL; // A donde moveremos el ticket
    bool encontrado = false; // Bandera para saber si lo encontramos

    // Pedimos datos al usuario
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

    // Determinamos cual sera la cola de destino segun el string ingresado
    if (strcmp(nuevaPrioridad, "Alta") == 0) {
        colaDestino = colaAlto;
    } else if (strcmp(nuevaPrioridad, "Media") == 0) {
        colaDestino = colaMedio;
    } else if (strcmp(nuevaPrioridad, "Baja") == 0) {
        colaDestino = colaBajo;
    } else {
        // Si no es ninguna de las validas, mostramos error
        printf("Prioridad no valida. Debe ser Alta, Media o Baja.\n");
        return;
    }

    // Ahora, buscamos el ticket en todas las colas
    Queue *colas[] = {colaAlto, colaMedio, colaBajo};
    for (int i = 0; i < 3; i++){
        colaOrigen = colas[i]; // Guardamos la cola que estamos revisando ahora
        List* listaInter = colaOrigen->list; // Accedemos a su lista interna
        void* data = front(listaInter); //Ponemos el current de la lista al inicio
        // Recorremos la lista interna
        while (data != NULL) {
            Ticket *ticket = (Ticket *)data;
            if (ticket->id == idTicket) {
                ticketEncontrado = ticket; // Guardamos el puntero al ticket encontrado
                // Usamos erase_current de la LIBRERIA DE LISTAS para sacar el nodo
                // Esto es necesario porque pop_queue solo saca del frente
                erase_current(listaInter);
                encontrado = true; // Marcamos que lo encontramos
                break; // Salimos del while
            }
            data = next(listaInter); // Pasamos al siguiente
        }
        if (encontrado) break; // Si ya lo encontramos, salimos del for tambien
    }
    // Si encontramos el ticket, lo movemos a la cola de destino
    if (encontrado && ticketEncontrado != NULL) {
        // Asignar la nueva prioridad
        strncpy(ticketEncontrado->prioridad, nuevaPrioridad, MAX_PRIO - 1);
        ticketEncontrado->prioridad[MAX_PRIO - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
        // Agregar el ticket a la cola de destino
        push_queue(colaDestino, ticketEncontrado);
        printf("Ticket con ID %d modificado a prioridad %s.\n", idTicket, nuevaPrioridad);
        mostrarDetallesTicket(ticketEncontrado);
    } else {
        // Si el bucle termino y no lo encontramos
        printf("No se encontro un ticket con ID %d.\n", idTicket);
    }
}

// Libera la memoria de todos los tickets que quedaron en las colas al salir
void liberarColas(){
    printf("Liberando memoria de tickets restantes.\n");
    Queue *colas[] = {colaAlto, colaMedio, colaBajo}; // Array de las colas

    // Recorremos cada cola
    for(int i = 0; i < 3; i++){
        Queue *colaActual = colas[i];
        // Mientras la cola no este vacia.
        while (top_queue(colaActual) != NULL){
            // 1. Obtenemos el puntero al ticket del frente
            Ticket *ticketPtr = (Ticket *)top_queue(colaActual);
            // 2. Lo sacamos de la cola
            pop_queue(colaActual);
            // 3. Liberamos la memoria asociada a ese ticket
            free(ticketPtr);
        }
        
    }
}

int leerOpcion(){
    char linea[100]; // Buffer para leer la entrada
    int opcion; // Variable para guardar la opcion leida

    if (fgets(linea, sizeof(linea), stdin) != NULL) {
        // Convertimos la cadena leida a un entero
        if (sscanf(linea, "%d", &opcion) == 1) {
            if (opcion >= 0 && opcion <= 5) {
                return opcion; // Retornamos la opcion si es valida
            }
        }
    }
    return -1; // Opcion invalida
}

// --- Funcion Principal ---
int main(){
    int opcion; // Para guardar la opcion del menu elegida por el usuario


    // Creamos las tres colas usando la funcion de la libreria
    colaAlto = createQueue();
    colaMedio = createQueue();
    colaBajo = createQueue();

    // Verificacion basica por si falla la creacion de colas (poca memoria, etc.)
    if (colaAlto == NULL || colaMedio == NULL || colaBajo == NULL) {
        printf("Error al crear las colas.\n");
        return 1;
    }
    // Mensaje de bienvenida
    printf("=============================================\n");
    printf(" Sistema de Gestion de Tickets de Soporte\n");
    printf("=============================================\n");
    // Bucle principal del menu
    do{
        printf("--- MENU PRINCIPAL ---\n");
        printf("1. Registrar nuevo ticket\n");
        printf("2. Asignar prioridad a ticket\n");
        printf("3. Mostrar tickets pendientes\n");
        printf("4. Procesar siguiente ticket\n");
        printf("5. Buscar ticket por ID\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");

        // Leemos la opcion del usuario y validamos que sea un numero
        opcion = leerOpcion();
        if (opcion == -1) {
            printf("Opcion invalida. Por favor, ingrese un numero entre 0 y 5.\n");
            continue;
        }
        printf("\n");

        // Ejecutamos la accion correspondiente a la opcion elegida
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
    } while (opcion != 0); // El bucle continua mientras la opcion no sea 0

    // Antes de terminar, liberamos la memoria de los tickets que quedaron
    liberarColas();
    printf("Fin del programa!\n");

    return 0; // Salida exitosa
}