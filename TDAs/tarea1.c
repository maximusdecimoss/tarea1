#include "tdas/extra.h"
#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

/*Se creara una estructura de datos para almacenar la información de los usuarios.
Esta estructura contendrá el nombre, ID, prioridad, problema y la hora de ingreso del usuario. 
Se utilizará una lista enlazada para almacenar los usuarios registrados y se asignarán prioridades a los tickets según su gravedad. 
La prioridad se asignará de la siguiente manera: 1 (alta), 2 (media) y 3 (baja). 
Los tickets se ordenarán por prioridad y hora de ingreso, y se atenderán en ese orden.
*/
typedef struct {
    char nombre[50];
    int id;
    int prioridad;
    char problema[500];
    int hora;
    int minuto;
} Usuario;

/*
Procederemos a declarar las funciones que se utilizarán en el programa. Para de esta forma llevar un orden y claridad en el código.
*/
void inicializarGeneradorAleatorio();
Usuario *crear_usuario_aleatorio();
void registrar_Usuario_aleatorio(List *Usuarios, List *baja);
void registrar_Usuario(List *Usuarios, List *baja);
void asignar_prioridades_aleatorias(List *Usuarios, List *alta, List *media, List *baja);
void mostrarMenuPrincipal();
void mostrarMenuPruebas();
void imprimir_lista(List *Usuarios, List *alta, List *media, List *baja);
void asignar_prioridad(List *alta, List *media, List *baja, Usuario *UsuarioNuevo);
void procesar_siguiente_ticket(List *alta, List *media, List *baja, List *Usuarios);
int buscar_ticket_por_id(int id, List *alta, List *media, List *baja, List *Usuarios);
int eliminar_usuario_por_id(List *lista, int id);
void eliminar_de_todas_las_listas(List *alta, List *media, List *baja, int id);
int lower_than(void *data1, void *data2); // Función de comparación para list_sortedInsert
int esNombreValido(const char *nombre);
int esHoraValida(int hora, int minuto);
int leerEntero(const char *mensaje);
void leerCadena(const char *mensaje, char *buffer, size_t size);
void imprimir_ticket(Usuario *usuario, const char *prioridad);
char leerOpcion(const char *mensaje);
/*
La Funcion inicializarGeneradorAleatorio inicializa el generador de números aleatorios usando la función srand con el tiempo actual como semilla. Esto asegura que cada vez que se ejecute el programa, se generen números aleatorios diferentes.
*/
void inicializarGeneradorAleatorio() {
    srand(time(NULL));
}

/*
La crear_usuario_aleatorio crea un nuevo usuario con un nombre, ID, problema y hora aleatorios. Utiliza arreglos de nombres y problemas predefinidos para seleccionar aleatoriamente un nombre y un problema. La hora y el minuto también se generan aleatoriamente.
*/
Usuario *crear_usuario_aleatorio() {
    Usuario *nuevo = malloc(sizeof(Usuario));// Aqui se asigna memoria para un nuevo usuario.
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
    }; //declaracion de textos bases que serviran para el testeo automatico de la app.

    int nombreIndex = rand() % (sizeof(nombres) / sizeof(nombres[0]));
    int problemaIndex = rand() % (sizeof(problemas) / sizeof(problemas[0]));

    snprintf(nuevo->nombre, sizeof(nuevo->nombre), "%s", nombres[nombreIndex]);
    nuevo->id = rand() % 1000;
    snprintf(nuevo->problema, sizeof(nuevo->problema), "%s", problemas[problemaIndex]);
    nuevo->hora = rand() % 24;
    nuevo->minuto = rand() % 60;
    nuevo->prioridad = 3;

    return nuevo;
}

int eliminar_usuario_por_id(List *lista, int id) {
    if (lista == NULL) return 0;

    Usuario *actual = list_first(lista);
    while (actual != NULL) {
        if (actual->id == id) {
            list_popCurrent(lista);
            return 1;
        }
        actual = list_next(lista);
    }
    return 0;
}

void eliminar_de_todas_las_listas(List *alta, List *media, List *baja, int id) {
    eliminar_usuario_por_id(alta, id);
    eliminar_usuario_por_id(media, id);
    eliminar_usuario_por_id(baja, id);
}

int lower_than(void *data1, void *data2) {
    Usuario *u1 = (Usuario *)data1;
    Usuario *u2 = (Usuario *)data2;
    if (u1->hora < u2->hora) return 1;
    if (u1->hora == u2->hora && u1->minuto < u2->minuto) return 1;
    return 0;
}

void registrar_Usuario_aleatorio(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo;
    int idUnico;
    do {
        UsuarioNuevo = crear_usuario_aleatorio();
        if (UsuarioNuevo == NULL) {
            puts("Error al asignar memoria para el usuario.");
            return;
        }
        idUnico = 1;
        Usuario *actual = list_first(Usuarios);
        while (actual != NULL) {
            if (actual->id == UsuarioNuevo->id) {
                idUnico = 0;
                free(UsuarioNuevo);
                break;
            }
            actual = list_next(Usuarios);
        }
    } while (!idUnico);

    list_pushBack(Usuarios, UsuarioNuevo);
    list_sortedInsert(baja, UsuarioNuevo, lower_than);

    printf("Usuario registrado aleatoriamente:\n");
    printf("ID: %d, Nombre: %s, Prioridad: %d, Hora de Ingreso: %02d:%02d\n", 
    UsuarioNuevo->id, UsuarioNuevo->nombre, UsuarioNuevo->prioridad, 
    UsuarioNuevo->hora, UsuarioNuevo->minuto);
}

void asignar_prioridades_aleatorias(List *Usuarios, List *alta, List *media, List *baja) {
    if (list_first(Usuarios) == NULL) {
        printf("No hay usuarios registrados para asignar prioridades.\n");
        return;
    }

    Usuario *actual = list_first(Usuarios);
    while (actual != NULL) {
        eliminar_de_todas_las_listas(alta, media, baja, actual->id);
        actual->prioridad = (rand() % 3) + 1;
        
        if (actual->prioridad == 1) {
            list_sortedInsert(alta, actual, lower_than);
        } else if (actual->prioridad == 2) {
            list_sortedInsert(media, actual, lower_than);
        } else {
            list_sortedInsert(baja, actual, lower_than);
        }

        printf("Asignada prioridad %d al usuario ID %d (%s)\n", 
        actual->prioridad, actual->id, actual->nombre);
        actual = list_next(Usuarios);
    }
    printf("Prioridades aleatorias asignadas exitosamente.\n");
}

int leerEntero(const char *mensaje) {
    char buffer[10];
    int valor;
    printf("%s", mensaje);
    while (fgets(buffer, sizeof(buffer), stdin) && 
        (sscanf(buffer, "%d", &valor) != 1 || valor < 0)) {
        printf("Entrada inválida. %s", mensaje);
    }
    return valor;
}

void leerCadena(const char *mensaje, char *buffer, size_t size) {
    printf("%s", mensaje);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

int esNombreValido(const char *nombre) {
    if (strlen(nombre) == 0) return 0;
    for (int i = 0; nombre[i]; i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ') return 0;
    }
    return 1;
}

int esHoraValida(int hora, int minuto) {
    return (hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59);
}

void registrar_Usuario(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo = malloc(sizeof(Usuario));
    if (UsuarioNuevo == NULL) {
        puts("Error al asignar memoria para el usuario.");
        return;
    }

    do {
        leerCadena("Ingrese su nombre (solo letras y espacios):\n", 
        UsuarioNuevo->nombre, sizeof(UsuarioNuevo->nombre));
    } while (!esNombreValido(UsuarioNuevo->nombre));

    int id;
    do {
        id = leerEntero("Ingrese su ID (solo números):\n");
        Usuario *actual = list_first(Usuarios);
        while (actual != NULL) {
            if (actual->id == id) {
                printf("Error: El ID %d ya está registrado.\n", id);
                id = -1;
                break;
            }
            actual = list_next(Usuarios);
        }
    } while (id == -1);
    UsuarioNuevo->id = id;

    leerCadena("Ingrese su problema (máximo 500 caracteres):\n", 
        UsuarioNuevo->problema, sizeof(UsuarioNuevo->problema));

    do {
        UsuarioNuevo->hora = leerEntero("Ingrese la hora (0-23): ");
        UsuarioNuevo->minuto = leerEntero("Ingrese los minutos (0-59): ");
    } while (!esHoraValida(UsuarioNuevo->hora, UsuarioNuevo->minuto));

    UsuarioNuevo->prioridad = 3;
    list_pushBack(Usuarios, UsuarioNuevo);
    list_sortedInsert(baja, UsuarioNuevo, lower_than);
    printf("Usuario registrado exitosamente.\n");
}

void imprimir_lista(List *Usuarios, List *alta, List *media, List *baja) {
    if (list_first(Usuarios) == NULL) {
        printf("No hay usuarios registrados.\n");
        return;
    }

    printf("\nLista de usuarios (ordenada por prioridad y hora de ingreso):\n");
    printf("--------------------------------------------------\n");
    printf("ID\tNombre\t\tPrioridad\tHora\tProblema\n");
    printf("--------------------------------------------------\n");

    Usuario *actual = list_first(alta);
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);
        actual = list_next(alta);
    }

    actual = list_first(media);
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);
        actual = list_next(media);
    }

    actual = list_first(baja);
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);
        actual = list_next(baja);
    }

    actual = list_first(Usuarios);
    while (actual != NULL) {
        if (actual->prioridad < 1 || actual->prioridad > 3) {
            printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
            actual->id, actual->nombre, actual->prioridad,
            actual->hora, actual->minuto, actual->problema);
        }
        actual = list_next(Usuarios);
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
        prioridad = leerEntero("Ingrese el nivel de prioridad (1, 2, o 3): ");
        
        if (prioridad >= 1 && prioridad <= 3) break;
        printf("Error: Opción no válida.\n");
    } while (1);

    eliminar_de_todas_las_listas(alta, media, baja, UsuarioNuevo->id);
    UsuarioNuevo->prioridad = prioridad;

    if (prioridad == 1) list_sortedInsert(alta, UsuarioNuevo, lower_than);
    else if (prioridad == 2) list_sortedInsert(media, UsuarioNuevo, lower_than);
    else list_sortedInsert(baja, UsuarioNuevo, lower_than);

    printf("Prioridad asignada con éxito.\n");
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
    const char *prioridades[] = {"Alta", "Media", "Baja"};
    List *listas[] = {alta, media, baja};

    for (int i = 0; i < 3; i++) {
        Usuario *actual = list_first(listas[i]);
        while (actual != NULL) {
            if (actual->id == id) {
                imprimir_ticket(actual, prioridades[i]);
                return 1;
            }
            actual = list_next(listas[i]);
        }
    }

    Usuario *actual = list_first(Usuarios);
    while (actual != NULL) {
        if (actual->id == id) {
            printf("Usuario encontrado, pero sin prioridad asignada:\n");
            imprimir_ticket(actual, "Sin prioridad");
            return 1;
        }
        actual = list_next(Usuarios);
    }
    return 0;
}

void procesar_siguiente_ticket(List *alta, List *media, List *baja, List *Usuarios) {
    Usuario *user = NULL;
    const char *prioridad = "";

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
    eliminar_usuario_por_id(Usuarios, user->id);
    free(user);
}

void mostrarMenuPrincipal() {
    limpiarPantalla();
    puts("========================================");
    puts("     Sistema de Gestión Tickets de Usuario");
    puts("========================================");
    puts("0) [Modo Pruebas]");
    puts("1) Registrar Usuario MANUAL");
    puts("2) Asignar prioridad a Usuario");
    puts("3) Mostrar lista de Usuarios");
    puts("4) Atender al siguiente Usuario");
    puts("5) Mostrar Usuarios por prioridad");
    puts("6) Salir");
}

void mostrarMenuPruebas() {
    limpiarPantalla();
    puts("========================================");
    puts("     Modo de Pruebas");
    puts("========================================");
    puts("1) Generar usuario aleatorio");
    puts("2) Asignar prioridades aleatorias a usuarios existentes");
    puts("3) Volver al menú principal");
}

char leerOpcion(const char *mensaje) {
    char opcion;
    printf("%s", mensaje);
    opcion = getchar(); // Lee un solo carácter
    // Limpiar el búfer de entrada
    while (getchar() != '\n'); 
    return opcion;
}

int main() {
    List *Usuarios = list_create();
    List *alta = list_create();
    List *media = list_create();
    List *baja = list_create();

    if (!Usuarios || !alta || !media || !baja) {
        printf("Error al crear las listas.\n");
        return 1;
    }

    inicializarGeneradorAleatorio();
    char opcion;

    do {
        mostrarMenuPrincipal();
        opcion = leerOpcion("Ingrese su opción: ");

        switch (opcion) {
            case '0': {
                char opcionPruebas;
                do {
                    mostrarMenuPruebas();
                    opcionPruebas = leerOpcion("Ingrese su opción: ");

                    switch (opcionPruebas) {
                        case '1':
                            printf("\n[Modo Pruebas] Registro aleatorio activado.\n");
                            registrar_Usuario_aleatorio(Usuarios, baja);
                            break;
                        case '2':
                            printf("\n[Modo Pruebas] Asignando prioridades aleatorias.\n");
                            asignar_prioridades_aleatorias(Usuarios, alta, media, baja);
                            break;
                        case '3':
                            printf("Volviendo al menú principal...\n");
                            break;
                        default:
                            puts("Opción no válida. Por favor, intente de nuevo.");
                    }
                    if (opcionPruebas != '3') presioneTeclaParaContinuar();
                } while (opcionPruebas != '3');
                break;
            }
            case '1':
                registrar_Usuario(Usuarios, baja);
                break;
            case '2': {
                int usuarioID = leerEntero("Ingrese el ID del usuario para asignar prioridad: ");
                Usuario *actual = list_first(Usuarios);
                while (actual != NULL) {
                    if (actual->id == usuarioID) {
                        asignar_prioridad(alta, media, baja, actual);
                        break;
                    }
                    actual = list_next(Usuarios);
                }
                if (actual == NULL) printf("Error: Usuario con ID %d no encontrado.\n", usuarioID);
                break;
            }
            case '3':
                imprimir_lista(Usuarios, alta, media, baja);
                break;
            case '4':
                procesar_siguiente_ticket(alta, media, baja, Usuarios);
                break;
            case '5': {
                int usuarioID = leerEntero("Ingrese el ID del usuario para buscar: ");
                if (!buscar_ticket_por_id(usuarioID, alta, media, baja, Usuarios)) {
                    printf("Error: El ticket con ID %d no se encuentra registrado.\n", usuarioID);
                }
                break;
            }
            case '6':
                puts("Saliendo del sistema de gestión de tickets...");
                break;
            default:
                puts("Opción no válida. Por favor, intente de nuevo.");
        }
        if (opcion != '0') presioneTeclaParaContinuar();
    } while (opcion != '6');

    Usuario *actual = list_first(Usuarios);
    while (actual != NULL) {
        free(actual);
        actual = list_next(Usuarios);
    }
    list_clean(Usuarios);
    list_clean(alta);
    list_clean(media);
    list_clean(baja);
    free(Usuarios);
    free(alta);
    free(media);
    free(baja);

    return 0;
}