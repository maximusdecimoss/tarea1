#include "tdas/extra.h"
#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

// Estructura para almacenar los datos del usuario
typedef struct {
    char nombre[50];
    int id;
    int prioridad;
    char problema[500];
    int hora;
    int minuto;
} Usuario;

// Declaración de funciones
void generarNombreAleatorio(char *nombre);
void generarProblemaAleatorio(char *problema);
void generarHoraAleatoria(int *hora, int *minuto);
int comparar_id(void *elemento, void *id);
int esNombreValido(char *nombre);
int esHoraValida(int hora, int minuto);
void mostrarMenuPrincipal();
void registrar_Usuario(List *Usuarios, List *baja);
void registrar_Usuario_aleatorio(List *Usuarios, List *baja);
void imprimir_lista(List *Usuarios);
void asignar_prioridad(List *alta, List *media, List *baja, Usuario *UsuarioNuevo);
void procesar_siguiente_ticket(List *alta, List *media, List *baja, List *Usuarios);
int buscar_en_lista_por_id(List *lista, int id, const char *prioridad);
void imprimir_ticket(Usuario *usuario, const char *prioridad);
int buscar_ticket_por_id(int id, List *alta, List *media, List *baja, List *Usuarios);
Usuario *crear_usuario_aleatorio();
int eliminar_usuario_por_id(List *lista, int id);

// Función para buscar y eliminar un usuario de una lista por ID
int eliminar_usuario_por_id(List *lista, int id) {
    if (lista == NULL) return 0;
    
    Usuario *usuarioActual = list_first(lista);
    Usuario *usuarioAnterior = NULL;
    
    while (usuarioActual != NULL) {
        if (usuarioActual->id == id) {
            // Si es el primer elemento
            if (usuarioAnterior == NULL) {
                list_popFront(lista);
            } else {
                // Si está en medio o al final, primero volvemos al inicio
                list_first(lista);
                // Y avanzamos hasta encontrar de nuevo el elemento
                while (list_next(lista) != usuarioActual);
                list_popCurrent(lista);
            }
            return 1;
        }
        usuarioAnterior = usuarioActual;
        usuarioActual = list_next(lista);
    }
    return 0;
}

void inicializarGeneradorAleatorio() {
    srand(time(NULL));
}

Usuario *crear_usuario_aleatorio() {
    Usuario *nuevo = malloc(sizeof(Usuario));
    if (nuevo == NULL) return NULL;

    const char *nombres[] = {"Juan", "Maria", "Pedro", "Ana", "Luis", "Carlos", "Sofia"};
    const char *problemas[] = {
        "El sistema se congela al intentar iniciar sesión",
        "No puedo acceder a mis archivos después de la actualización",
        "La aplicación se cierra inesperadamente al abrir un archivo",
        "Mi contraseña no es aceptada, aunque la introduzco correctamente",
        "La conexión a internet sigue fallando sin razón aparente",
        "No puedo encontrar el archivo descargado en mi computadora",
        "La sincronización de mis dispositivos no funciona correctamente",
        "El programa tarda mucho en cargar, incluso con buena conexión",
        "La interfaz de usuario no responde al hacer clic en botones",
        "Recibo un mensaje de error al intentar guardar mi trabajo"
    };

    int nombreIndex = rand() % (sizeof(nombres) / sizeof(nombres[0]));
    int problemaIndex = rand() % (sizeof(problemas) / sizeof(problemas[0]));

    snprintf(nuevo->nombre, 50, "%s", nombres[nombreIndex]);
    nuevo->id = rand() % 1000;
    snprintf(nuevo->problema, 500, "%s", problemas[problemaIndex]);
    nuevo->hora = rand() % 24;
    nuevo->minuto = rand() % 60;
    nuevo->prioridad = 3; // Por defecto baja

    return nuevo;
}

void registrar_Usuario_aleatorio(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo = crear_usuario_aleatorio();
    if (UsuarioNuevo == NULL) {
        puts("Error al asignar memoria para el usuario.");
        return;
    }

    list_pushBack(Usuarios, UsuarioNuevo);
    list_pushBack(baja, UsuarioNuevo);

    printf("Usuario registrado aleatoriamente:\n");
    printf("ID: %d, Nombre: %s, Prioridad: %d, Hora de Ingreso: %02d:%02d\n", 
        UsuarioNuevo->id, UsuarioNuevo->nombre, UsuarioNuevo->prioridad, 
        UsuarioNuevo->hora, UsuarioNuevo->minuto);
}

int main() {
    char opcion;
    List *Usuarios = list_create();
    List *alta = list_create();
    List *media = list_create();
    List *baja = list_create();

    inicializarGeneradorAleatorio();

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        int usuarioID;

        switch (opcion) {
            case '1':
                registrar_Usuario_aleatorio(Usuarios, baja);
                break;
            case '2':
                printf("\nIngrese el ID del usuario para asignar prioridad (solo números): ");
                scanf("%d", &usuarioID);
                {
                    Usuario *usuarioActual = list_first(Usuarios);
                    while (usuarioActual != NULL) {
                        if (usuarioActual->id == usuarioID) {
                            asignar_prioridad(alta, media, baja, usuarioActual);
                            break;
                        }
                        usuarioActual = list_next(Usuarios);
                    }
                    if (usuarioActual == NULL) {
                        printf("Error: Usuario con ID %d no encontrado.\n", usuarioID);
                    }
                }
                break;
            case '3':
                imprimir_lista(Usuarios);
                break;
            case '4':
                procesar_siguiente_ticket(alta, media, baja, Usuarios);
                break;
            case '5':
                printf("\nIngrese el ID del usuario para buscar (solo números): ");
                scanf("%d", &usuarioID);
                if (!buscar_ticket_por_id(usuarioID, alta, media, baja, Usuarios)) {
                    printf("Error: El ticket con ID %d no se encuentra registrado.\n", usuarioID);
                }
                break;
            case '6':
                puts("Saliendo del sistema de gestión de tickets...");
                break;
            default:
                puts("Opción no válida. Por favor, intente de nuevo.");
        }

        presioneTeclaParaContinuar(); // Esta función está en extra.h
    } while (opcion != '6');

    // Liberar memoria: primero limpiar las listas
    list_clean(Usuarios);
    list_clean(alta);
    list_clean(media);
    list_clean(baja);
    
    // Luego liberar las listas mismas
    free(Usuarios);
    free(alta);
    free(media);
    free(baja);
    
    return 0;
}

void mostrarMenuPrincipal() {
    limpiarPantalla(); // Esta función está en extra.h
    puts("========================================");
    puts("     Sistema de Gestión Tickets de usuario");
    puts("========================================");
    puts("1) Registrar Usuario");
    puts("2) Asignar prioridad a Usuario");
    puts("3) Mostrar lista de Usuarios ");
    puts("4) Atender al siguiente Usuario");
    puts("5) Mostrar Usuarios por prioridad");
    puts("6) Salir");
}

void registrar_Usuario(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo = malloc(sizeof(Usuario));
    if (UsuarioNuevo == NULL) {
        puts("Error al asignar memoria para el usuario.");
        return;
    }

    do {
        printf("Ingrese su nombre (solo letras y espacios):\n");
        getchar();
        scanf(" %[^\n]", UsuarioNuevo->nombre);
    } while (!esNombreValido(UsuarioNuevo->nombre));

    printf("Ingrese su ID (solo números):\n");
    scanf("%d", &UsuarioNuevo->id);
    getchar();

    Usuario *usuarioActual = list_first(Usuarios);
    while (usuarioActual != NULL) {
        if (usuarioActual->id == UsuarioNuevo->id) {
            printf("Error: El ID %d ya está registrado.\n", UsuarioNuevo->id);
            free(UsuarioNuevo);
            return;
        }
        usuarioActual = list_next(Usuarios);
    }

    printf("Ingrese su problema (máximo 500 caracteres):\n");
    scanf(" %[^\n]", UsuarioNuevo->problema);

    do {
        printf("Ingrese la hora de ingreso (HH:MM): ");
        scanf("%d:%d", &UsuarioNuevo->hora, &UsuarioNuevo->minuto);
    } while (!esHoraValida(UsuarioNuevo->hora, UsuarioNuevo->minuto));

    UsuarioNuevo->prioridad = 3;
    list_pushBack(Usuarios, UsuarioNuevo);
    list_pushBack(baja, UsuarioNuevo);
    printf("Usuario registrado exitosamente.\n");
}

void imprimir_lista(List *Usuarios) {
    if (list_size(Usuarios) == 0) {
        printf("No hay usuarios registrados.\n");
        return;
    }
    
    printf("\nLista de usuarios:\n");
    printf("--------------------------------------------------\n");
    printf("ID\tNombre\t\tPrioridad\tHora\tProblema\n");
    printf("--------------------------------------------------\n");
    
    Usuario *usuarioActual = list_first(Usuarios);
    while (usuarioActual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
            usuarioActual->id, 
            usuarioActual->nombre, 
            usuarioActual->prioridad,
            usuarioActual->hora, 
            usuarioActual->minuto,
            usuarioActual->problema);
        usuarioActual = list_next(Usuarios);
    }
    printf("--------------------------------------------------\n");
}

void asignar_prioridad(List *alta, List *media, List *baja, Usuario *UsuarioNuevo) {
    int prioridad;
    do {
        printf("========================================\n");
        printf("Asignar prioridad a su problema\n");
        printf("========================================\n");
        printf("1) Alta prioridad\n");
        printf("2) Prioridad media\n");
        printf("3) Baja prioridad\n");
        printf("Ingrese el nivel de prioridad (1, 2, o 3): ");
        scanf("%d", &prioridad);

        if (prioridad >= 1 && prioridad <= 3) {
            // Eliminar de todas las listas de prioridad
            eliminar_usuario_por_id(alta, UsuarioNuevo->id);
            eliminar_usuario_por_id(media, UsuarioNuevo->id);
            eliminar_usuario_por_id(baja, UsuarioNuevo->id);
            
            UsuarioNuevo->prioridad = prioridad;
            
            // Agregar a la lista correspondiente
            if (prioridad == 1) list_pushBack(alta, UsuarioNuevo);
            else if (prioridad == 2) list_pushBack(media, UsuarioNuevo);
            else list_pushBack(baja, UsuarioNuevo);
            
            printf("Prioridad asignada con éxito.\n");
            break;
        } else {
            printf("Error: Opción no válida.\n");
        }
    } while (1);
}

int buscar_en_lista_por_id(List *lista, int id, const char *prioridad) {
    if (lista == NULL) return 0;
    
    Usuario *usuarioActual = list_first(lista);
    while (usuarioActual != NULL) {
        if (usuarioActual->id == id) {
            imprimir_ticket(usuarioActual, prioridad);
            return 1;
        }
        usuarioActual = list_next(lista);
    }
    return 0;
}

void imprimir_ticket(Usuario *usuario, const char *prioridad) {
    printf("\n============================\n");
    printf("        TICKET #%d\n", usuario->id);
    printf("============================\n");
    printf("Nombre: %s\n", usuario->nombre);
    printf("Problema: %s\n", usuario->problema);
    printf("Prioridad: %s\n", prioridad);
    printf("Registrado: %02d:%02d\n", usuario->hora, usuario->minuto);
    printf("============================\n\n");
}

int buscar_ticket_por_id(int id, List *alta, List *media, List *baja, List *Usuarios) {
    if (buscar_en_lista_por_id(alta, id, "Alta")) return 1;
    if (buscar_en_lista_por_id(media, id, "Media")) return 1;
    if (buscar_en_lista_por_id(baja, id, "Baja")) return 1;
    
    // Buscar en la lista general si no está en las de prioridad
    Usuario *user = list_first(Usuarios);
    while (user != NULL) {
        if (user->id == id) {
            printf("Usuario encontrado, pero sin prioridad asignada:\n");
            imprimir_ticket(user, "Sin prioridad");
            return 1;
        }
        user = list_next(Usuarios);
    }
    return 0;
}

int esNombreValido(char *nombre) {
    if (strlen(nombre) == 0) return 0;
    
    for (int i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ') return 0;
    }
    return 1;
}

int esHoraValida(int hora, int minuto) {
    return !(hora < 0 || hora > 23 || minuto < 0 || minuto > 59);
}

void procesar_siguiente_ticket(List *alta, List *media, List *baja, List *Usuarios) {
    Usuario *user = NULL;
    const char *prioridad = "";
    
    // Intentar procesar por orden de prioridad
    if (list_size(alta) > 0) {
        user = list_popFront(alta);
        prioridad = "Alta";
    } else if (list_size(media) > 0) {
        user = list_popFront(media);
        prioridad = "Media";
    } else if (list_size(baja) > 0) {
        user = list_popFront(baja);
        prioridad = "Baja";
    } 
    
    if (user == NULL) {
        printf("No hay tickets pendientes.\n");
        return;
    }
    
    printf("Atendiendo ticket (%s prioridad):\n", prioridad);
    imprimir_ticket(user, prioridad);
    
    // Eliminar también de la lista general
    eliminar_usuario_por_id(Usuarios, user->id);
    free(user);
}

void generarNombreAleatorio(char *nombre) {
    const char *nombres[] = {"Juan", "Maria", "Pedro", "Ana", "Luis", "Carlos", "Sofia"};
    int index = rand() % (sizeof(nombres) / sizeof(nombres[0]));
    snprintf(nombre, 50, "%s", nombres[index]);
}

void generarProblemaAleatorio(char *problema) {
    const char *problemas[] = {
        "El sistema se congela al intentar iniciar sesión",
        "No puedo acceder a mis archivos después de la actualización",
        "La aplicación se cierra inesperadamente al abrir un archivo",
        "Mi contraseña no es aceptada, aunque la introduzco correctamente",
        "La conexión a internet sigue fallando sin razón aparente",
        "No puedo encontrar el archivo descargado en mi computadora",
        "La sincronización de mis dispositivos no funciona correctamente",
        "El programa tarda mucho en cargar, incluso con buena conexión",
        "La interfaz de usuario no responde al hacer clic en botones",
        "Recibo un mensaje de error al intentar guardar mi trabajo"
    };
    int index = rand() % (sizeof(problemas) / sizeof(problemas[0]));
    snprintf(problema, 500, "%s", problemas[index]);
}

void generarHoraAleatoria(int *hora, int *minuto) {
    *hora = rand() % 24;
    *minuto = rand() % 60;
}

int comparar_id(void *elemento, void *id) {
    Usuario *user = (Usuario *)elemento;
    int *buscar_id = (int *)id;
    return (user->id == *buscar_id);
}