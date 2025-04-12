# Tarea 1 - Sistema de Gestión de Tickets

Este programa simula un sistema de soporte técnico donde se pueden registrar, listar y procesar tickets con diferentes prioridades. Utiliza estructuras de datos como listas, colas y pilas implementadas manualmente a través de TDA (Tipos de Datos Abstractos).

---

## Compilación y ejecución

### 1. Requisitos
- Compilador C (como `gcc`)
- Sistema operativo compatible con consola/terminal (Windows, Linux, macOS)

### 2. Archivos necesarios
Asegúrate de tener todos estos archivos en la misma carpeta:

- `main.c` — Lógica principal del programa  
- `estructuras.c` — Implementación de TDA Lista, Cola y Pila  
- `estructuras.h` — Prototipos y definiciones de las estructuras  

### 3. Compilar y ejecutar (Ejemplo con GCC)

**En terminal bash:**
```bash
gcc main.c estructuras.c -o tarea1
./tarea1
```

Si estás usando VSCode, asegúrate de compilar *ambos* archivos (`main.c` y `estructuras.c`), no solo uno.

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
| 1      | ✅        | Crea un nuevo ticket con descripción e ID automático. |
| 2      | ✅        | Permite seleccionar prioridad (Alta, Media, Baja). |
| 3      | ✅        | Muestra todos los tickets en espera, ordenados por prioridad. |
| 4      | ✅        | Procesa y elimina el ticket con mayor prioridad. |
| 5      | ✅        | Busca ticket por ID y muestra sus detalles. |
| 0      | ✅        | Finaliza el programa correctamente. |

### Posibles errores conocidos:
- Si el programa se compila solo con `main.c`, mostrará errores de funciones no definidas (`undefined reference`).  
  **Solución:** compilar con `estructuras.c` también.

---

## Ejemplo de uso

--- MENÚ PRINCIPAL ---
1. Registrar nuevo ticket
Seleccione una opción: 1
Ingrese la descripcion del ticket (maximo 300 caracteres): El computador no enciende.

--- MENÚ PRINCIPAL ---
2. Asignar prioridad a ticket
Seleccione una opción: 2
Ingrese ID del ticket: 1
Seleccione prioridad (Alta, Media, Baja): Alta

--- MENÚ PRINCIPAL ---
3. Mostrar tickets pendientes
Seleccione una opción: 3
Ticket prioridad Alta
ID: 1
Descripcion: El computador no enciende.
Prioridad: Alta
Hora de Registro: 2025-04-11 23:45:23

--- MENÚ PRINCIPAL ---
4. Procesar siguiente ticket
Ticket procesado y eliminado de la cola.

--- MENÚ PRINCIPAL ---
0. Salir
Saliendo del programa...
Liberando memoria de tickets restantes.
Fin del programa!
```
