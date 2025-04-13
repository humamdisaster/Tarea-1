# Tarea 1 - Sistema de Gestión de Tickets

Este programa simula un sistema de soporte técnico donde se pueden registrar, listar y procesar tickets con diferentes prioridades (Alta, Media, Baja). Utiliza estructuras de datos como listas y colas implementadas a través de TDA (Tipos de Datos Abstractos) proporcionados en archivos separados.

---

## Compilación y Ejecución

### 1. Requisitos
- Un compilador de C (como `gcc`).
- Un sistema operativo compatible con consola/terminal (Windows, Linux, macOS).

### 2. Archivos Necesarios
Asegúrate de tener todos estos archivos en la misma carpeta:

- `main.c`: Contiene la lógica principal de la aplicación y el menú de usuario.
- `estructuras.c`: Contiene la implementación de las estructuras de datos (Lista, Cola).
- `estructuras.h`: Contiene las definiciones de tipos y prototipos de funciones para las estructuras.

### 3. Compilar y Ejecutar (Ejemplo con GCC)

Abre una terminal o símbolo del sistema en la carpeta donde están los archivos y ejecuta:

```bash
# Compilar (genera un ejecutable llamado 'tarea1')
gcc main.c estructuras.c -o tarea1 -lm

# Ejecutar
./tarea1
```
(En Windows, el ejecutable podría ser tarea1.exe y se ejecutaría con tarea1 o .\tarea1)

Si estás usando VSCode, asegúrate de compilar *ambos* archivos (`main.c` y `estructuras.c`). Compilar solo main.c resultará en errores.

---

## Opciones disponibles en el menú

--- MENÚ PRINCIPAL ---
1. Registrar nuevo ticket
2. Asignar prioridad a ticket
3. Mostrar tickets pendientes
4. Procesar siguiente ticket
5. Buscar ticket por ID
0. Salir
```

### Funciones implementadas correctamente:
| Opción | Funciona | Detalles |
|--------|----------|----------|
| 1      | ✅        | 	Pide ID y descripción al usuario, crea el ticket y verifica que el ID sea único. |
| 2      | ✅        | Permite seleccionar prioridad (Alta, Media, Baja). |
| 3      | ✅        | Muestra todos los tickets en espera, ordenados por prioridad. |
| 4      | ✅        | Procesa y elimina el ticket con mayor prioridad. |
| 5      | ✅        | Busca ticket por ID y muestra sus detalles. |
| 0      | ✅        | Finaliza el programa y libera la memoria de los tickets pendientes. |

### Posibles errores conocidos:
- Si el programa se compila solo con `main.c`, mostrará errores de funciones no definidas (`undefined reference`).  
  **Solución:** compilar con `estructuras.c` también.
- Ingresar texto cuando se espera un número para la opción del menú mostrará un error (manejado por el programa).
---

### Limitaciones / A Considerar
IDs: El ID del ticket es un texto ingresado por el usuario. El sistema verifica que el ID no esté repetido al momento de registrar un nuevo ticket. Se recomienda usar IDs consistentes.
Persistencia: Los tickets registrados se pierden cuando el programa se cierra. No se guardan en archivos.
---

## Ejemplo de uso

=============================================
 Sistema de Gestion de Tickets de Soporte
=============================================

--- MENU PRINCIPAL ---
1. Registrar nuevo ticket
2. Asignar prioridad a ticket
3. Mostrar tickets pendientes
4. Procesar siguiente ticket
5. Buscar ticket por ID
0. Salir
Seleccione una opcion: 1

Ingrese el ID del ticket (maximo 49 caracteres): TICKET-001
Ingrese la descripcion del ticket (maximo 299 caracteres): El computador no enciende.

Ticket registrado con exito!
  ID: TICKET-001
  Descripcion: El computador no enciende.
  Prioridad: Bajo
  Hora de Registro: 2025-04-13 01:53:00  
  ------------------------------------

--- MENU PRINCIPAL ---
... (Opciones) ...
Seleccione una opcion: 2

Ingrese el ID del ticket a modificar: TICKET-001
Ingrese la nueva prioridad (Alta, Media, Baja): Alta

Prioridad del ticket ID TICKET-001 actualizada a 'Alta'.
  ID: TICKET-001
  Descripcion: El computador no enciende.
  Prioridad: Alta
  Hora de Registro: 2025-04-13 01:53:00 
  ------------------------------------

--- MENU PRINCIPAL ---
... (Opciones) ...
Seleccione una opcion: 3

===== LISTA DE TICKETS PENDIENTES =====

--- Tickets Prioridad Alta ---
  ID: TICKET-001
  Descripcion: El computador no enciende.
  Prioridad: Alta
  Hora de Registro: 2025-04-13 01:53:00
  ------------------------------------


--- Tickets Prioridad Media ---
   (No hay tickets en esta cola)


--- Tickets Prioridad Baja ---
   (No hay tickets en esta cola)


--- MENU PRINCIPAL ---
... (Opciones) ...
Seleccione una opcion: 4

===== PROCESANDO SIGUIENTE TICKET =====
Procesando ticket de ALTA prioridad:
  ID: TICKET-001
  Descripcion: El computador no enciende.
  Prioridad: Alta
  Hora de Registro: 2025-04-13 01:53:00
  ------------------------------------
Ticket ID TICKET-001 procesado y eliminado de la cola.
=======================================

--- MENU PRINCIPAL ---
... (Opciones) ...
Seleccione una opcion: 0

Saliendo del programa...
Liberando memoria de tickets restantes...
No habia tickets pendientes para liberar.
Programa terminado.