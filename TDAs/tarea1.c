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
/*
Comienzo declarando unas estructura en la cual posteriormetne guardare los datos de cada ususario que use la app.
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
La funcion crear_usuario_aleatorio crea un nuevo usuario con un nombre, ID, problema y hora aleatorios. Utiliza arreglos de nombres y problemas predefinidos para seleccionar aleatoriamente un nombre y un problema. La hora y el minuto también se generan aleatoriamente.
*/
Usuario *crear_usuario_aleatorio() {
    Usuario *nuevo = malloc(sizeof(Usuario));// Aqui se asigna memoria para un nuevo usuario.
    if (nuevo == NULL) return NULL;

    const char *nombres[] = {"Juan", "Maria", "Pedro", "Ana", "Luis", "Carlos", "Sofia"}; //declaracion de nombres aleatorios para el testeo automatico de la app.
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

    //Divido el tamaño total del arreglo entre el tamaño de un solo elemento, lo que me dara
    //el total de elementos que hay en el arreglo.
    //Luego uso el operador modulo para obtener un numero aleatorio entre 0 y el total de elementos -1.
    //Esto me permite seleccionar un nombre aleatorio de la lista de nombres.
    int nombreIndex = rand() % (sizeof(nombres) / sizeof(nombres[0]));    
    int problemaIndex = rand() % (sizeof(problemas) / sizeof(problemas[0]));

    snprintf(nuevo->nombre, sizeof(nuevo->nombre), "%s", nombres[nombreIndex]);//se usa la funcion snprintf para controlar cuántos caracteres se copian como máximo, lo que evita desbordamientos de buffer.
    nuevo->id = rand() % 1000;
    snprintf(nuevo->problema, sizeof(nuevo->problema), "%s", problemas[problemaIndex]);
    nuevo->hora = rand() % 24;
    nuevo->minuto = rand() % 60;
    nuevo->prioridad = 3;

    return nuevo; // retorno el nuevo usuario creado.
}

/* La siguiente funcion se encarga de eliminar un usuario de la lista de usuarios.
Recibe como argumento la lista de usuarios y el ID del usuario a eliminar. 
Si encuentra el usuario, lo elimina de la lista y retorna 1. Si no lo encuentra, retorna 0.
*/
int eliminar_usuario_por_id(List *lista, int id) {
    if (lista == NULL) return 0;

    Usuario *actual = list_first(lista);//Obtengo el primer elemento de la lista.
    while (actual != NULL) {
        if (actual->id == id) {
            list_popCurrent(lista);//si el id coincide, elimino el usuario actual de la lista.
            return 1;
        }
        actual = list_next(lista);//sino avanzo al siguiente elemento de la lista.
    }
    return 0;
}


/*
La siguiente funcion se encarga de eliminar un usuario de todas las listas (alta, media y baja)
Recibe como argumento las listas y el ID del usuario a eliminar.
*/
void eliminar_de_todas_las_listas(List *alta, List *media, List *baja, int id) {
    eliminar_usuario_por_id(alta, id);
    eliminar_usuario_por_id(media, id);
    eliminar_usuario_por_id(baja, id);
}

/*
La siguiente funcion se encarga de comparar las horas de llegada de los ususarios para porder ordenar los mismos pro quien tiene un hora de llegada menor en comparacion al otro
*/
int lower_than(void *data1, void *data2) {
    Usuario *u1 = (Usuario *)data1;//Hago un cast para tratar los void* como punteros a Usuario, y 
    Usuario *u2 = (Usuario *)data2;//poder acceder a sus campos (hora, minuto).
    if (u1->hora < u2->hora) return 1;
    if (u1->hora == u2->hora && u1->minuto < u2->minuto) return 1;
    return 0;
}

/*
La siguiente funcion se encarga de crear un nuevo usuario aleatorio, asegurarse de que su ID no esté repetido, y luego registrarlo en el sistema, agregándolo tanto a la lista general como a la lista de prioridad baja. Esto para los casos de testeo de la app.
*/
void registrar_Usuario_aleatorio(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo;
    int idUnico;// uso esta funcion como bandera para asegurarme de que el id sea unico.
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
        }//este fragmento de codigo me verifica si el id que estoy generando es unico, si no lo es, libero la memoria y vuelvo a generar un nuevo usuario.
    } while (!idUnico);

    list_pushBack(Usuarios, UsuarioNuevo);// luego de verificar que el id es unico, lo agrego a la lista de usuarios, sin que me importe mucho el orden.
    list_sortedInsert(baja, UsuarioNuevo, lower_than);//Aqui lo agrego a la lista de baja prioridad, ordenandolo por hora de ingreso.

    printf("Usuario registrado aleatoriamente:\n");
    printf("ID: %d, Nombre: %s, Prioridad: %d, Hora de Ingreso: %02d:%02d\n", 
    UsuarioNuevo->id, UsuarioNuevo->nombre, UsuarioNuevo->prioridad, 
    UsuarioNuevo->hora, UsuarioNuevo->minuto);
}


/*
La siguiente funcion se encarga de asignar prioridades aleatorias a los usuarios registrados en el sistema.Recibe como argumento las listas de usuarios y las listas de prioridades (alta, media y baja).
*/
void asignar_prioridades_aleatorias(List *Usuarios, List *alta, List *media, List *baja) {
    if (list_first(Usuarios) == NULL) {
        printf("No hay usuarios registrados para asignar prioridades.\n");
        return;//verifico que la lista de usuarios no este vacia, si lo esta, salgo de la funcion.
    }

    Usuario *actual = list_first(Usuarios);//obtengo el primer elemento de la lista de usuarios.
    while (actual != NULL) {
        eliminar_de_todas_las_listas(alta, media, baja, actual->id);
        actual->prioridad = (rand() % 3) + 1;//asigno una prioridad aleatoria entre 1 y 3.
        
        if (actual->prioridad == 1) {
            list_sortedInsert(alta, actual, lower_than);
        } else if (actual->prioridad == 2) {
            list_sortedInsert(media, actual, lower_than);
        } else {
            list_sortedInsert(baja, actual, lower_than);
        }//aqui en estas tres condiciones ordeno el usuario en la lista correspondiente a su prioridad.

        printf("Asignada prioridad %d al usuario ID %d (%s)\n", 
        actual->prioridad, actual->id, actual->nombre);//aqui imprimo el id y el nombre del usuario al que le asigne la prioridad.
        actual = list_next(Usuarios);//avanzamos al siguiente usuario en la lista.
    }
    printf("Prioridades aleatorias asignadas exitosamente.\n");
}

/*
La siguiente funcion se encarga de leer un entero desde la entrada estándar.
Recibe como argumento un mensaje que se mostrará al usuario. La función valida la entrada, asegurándose de que sea un número entero no negativo. Si la entrada es inválida, vuelve a pedir al usuario que ingrese un número.
*/
int leerEntero(const char *mensaje) {
    char buffer[10];//Habilito un espacio de 10 caracteres para leer el entero para evitar 
    int valor;      //desbordamientos.
    printf("%s", mensaje);
    while (fgets(buffer, sizeof(buffer), stdin) && 
        (sscanf(buffer, "%d", &valor) != 1 || valor < 0)) {
        printf("Entrada inválida. %s", mensaje);
    }//uso la funcion fgets para leer la entrada del usuario y evitar desbordamiento de memoria y sscanf para convertirla a un entero.
    // Si la conversión falla o el número es negativo, se vuelve a pedir la entrada.
    return valor;
}
/*
La siguiente funcion se encarga de leer una cadena de caracteres desde la entrada estándar.
Recibe como argumento un mensaje que se mostrará al usuario, un buffer donde se almacenará la cadena leída y el tamaño máximo del buffer. La función elimina el salto de línea al final de la cadena.
*/
void leerCadena(const char *mensaje, char *buffer, size_t size) {
    printf("%s", mensaje);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;//Esa línea elimina el \n al final de la cadena leída con fgets(), reemplazándolo con el carácter nulo \0 para que no afecte en operaciones posteriores.
}


/*
La siguiente funcion se implementara para garantizar que el usuario ingrese un nombre válido, la función esNombreValido verifica que la cadena no esté vacía y que contenga solo letras y espacios.
Si la cadena es válida, devuelve 1; de lo contrario, devuelve 0.
*/
int esNombreValido(const char *nombre) {
    if (strlen(nombre) == 0) return 0;//verifico que la cadena no este vacia.
    
    for (int i = 0; nombre[i]; i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ') return 0;
    }//recorro la cadena y verifico que cada caracter sea una letra o un espacio.

    
    return 1;
}

/*
La siguiente funcion se encarga de verificar que la hora y el minuto ingresados sean válidos.   
Recibe como argumento la hora y el minuto a verificar. La función devuelve 1 si la hora y el minuto son válidos; de lo contrario, devuelve 0.
*/
int esHoraValida(int hora, int minuto) {
    return (hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59);
}

/*
La siguiente funcion se encarga de registrar un nuevo usuario en el sistema.
Recibe como argumento la lista de usuarios y la lista de baja prioridad. La función solicita al usuario que ingrese su nombre, ID, problema y hora de ingreso. Luego, verifica que el ID no esté repetido y que la hora y el minuto sean válidos. Finalmente, agrega el nuevo usuario a la lista de usuarios y a la lista de baja prioridad.
*/
void registrar_Usuario(List *Usuarios, List *baja) {
    Usuario *UsuarioNuevo = malloc(sizeof(Usuario));
    if (UsuarioNuevo == NULL) {
        puts("Error al asignar memoria para el usuario.");
        return;//si no se puede asignar memoria, salgo de la funcion.
    }

    do {
        leerCadena("Ingrese su nombre (solo letras y espacios):\n", 
        UsuarioNuevo->nombre, sizeof(UsuarioNuevo->nombre));
    } while (!esNombreValido(UsuarioNuevo->nombre));//verifico que el nombre sea valido.

    int id;
    do {
        id = leerEntero("Ingrese su ID (solo números):\n");
        Usuario *actual = list_first(Usuarios);//obtengo el primer elemento de la lista de usuarios.
        // Verifico si el ID ya está registrado en la lista de usuarios.
        while (actual != NULL) {
            if (actual->id == id) {
                printf("Error: El ID %d ya está registrado.\n", id);
                id = -1;
                break;
            }//si el id ya esta registrado, le asigno -1 a la variable id para que vuelva a pedir un nuevo id.
            actual = list_next(Usuarios);// avanzo al siguiente usuario en la lista.   
        }
    } while (id == -1);// si el id es invalido, vuelvo a pedir un nuevo id.
    UsuarioNuevo->id = id;

    leerCadena("Ingrese su problema (máximo 500 caracteres):\n", 
        UsuarioNuevo->problema, sizeof(UsuarioNuevo->problema));// Guardo el problema que el usuario presente en el momento de registrarse y verifico que el mismo no exceda los 500 caracteres.

    do {
        UsuarioNuevo->hora = leerEntero("Ingrese la hora (0-23): ");
        UsuarioNuevo->minuto = leerEntero("Ingrese los minutos (0-59): ");
    } while (!esHoraValida(UsuarioNuevo->hora, UsuarioNuevo->minuto));// verifico que la hora y el minuto sean validos.
    

    UsuarioNuevo->prioridad = 3;
    list_pushBack(Usuarios, UsuarioNuevo);
    list_sortedInsert(baja, UsuarioNuevo, lower_than);// Asigno la prioridad baja por defecto y lo 
    printf("Usuario registrado exitosamente.\n");//agrego a la lista de usuarios.
}

/*
La siguiente funcion se encarga de imprimir la lista de usuarios registrados en el sistema.
Recibe como argumento la lista de usuarios, la lista de alta prioridad, la lista de media prioridad y la lista de baja prioridad. La función imprime los usuarios ordenados por prioridad y hora de ingreso.
*/

void imprimir_lista(List *Usuarios, List *alta, List *media, List *baja) {
    if (list_first(Usuarios) == NULL) {
        printf("No hay usuarios registrados.\n");
        return;//verifico que la lista de usuarios no este vacia, si lo esta, salgo de la funcion.
    }

    printf("\nLista de usuarios (ordenada por prioridad y hora de ingreso):\n");
    printf("--------------------------------------------------\n");
    printf("ID\tNombre\t\tPrioridad\tHora\tProblema\n");
    printf("--------------------------------------------------\n");

    Usuario *actual = list_first(alta);// obtengo el primer elemento de la lista de alta prioridad.
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);//imprimo los datos del primer usuario de la lista alta.
        actual = list_next(alta);// avanzo al siguiente usuario en la lista.
    }

    actual = list_first(media);
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);
        actual = list_next(media);
    }//imprimo los datos del usuario para el primero de la lista media

    actual = list_first(baja);
    while (actual != NULL) {
        printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
        actual->id, actual->nombre, actual->prioridad,
        actual->hora, actual->minuto, actual->problema);
        actual = list_next(baja);
    }// imprimo los datos del usuario para el primero de la lista baja.

    actual = list_first(Usuarios);
    while (actual != NULL) {
        if (actual->prioridad < 1 || actual->prioridad > 3) {
            printf("%d\t%-10s\t%d\t\t%02d:%02d\t%.30s...\n",
            actual->id, actual->nombre, actual->prioridad,
            actual->hora, actual->minuto, actual->problema);
        }//imprimo los datos del usuario para el primero de la lista de usuarios, que no tiene prioridad asignada.
        actual = list_next(Usuarios);
    }
    printf("--------------------------------------------------\n");
}

/*
La siguiente funcion se encarga de asignar una prioridad a un usuario registrado en el sistema.
Recibe como argumento las listas de alta, media y baja prioridad, y el usuario al que se le asignará la prioridad. La función solicita al usuario que ingrese la prioridad deseada (alta, media o baja) y actualiza la lista de prioridades del usuario.    
*/
void asignar_prioridad(List *alta, List *media, List *baja, Usuario *UsuarioNuevo) {
    int prioridad;
    do {
        printf("========================================\n");
        printf("Asignar prioridad a su problema\n");
        printf("========================================\n");
        printf("1) Alta prioridad\n");
        printf("2) Prioridad media\n");
        printf("3) Baja prioridad\n");
        prioridad = leerEntero("Ingrese el nivel de prioridad (1, 2, o 3): ");// le pido al usuario que ingrese la prioridad deseada, validando que sea un numero entre 1 y 3.
        
        if (prioridad >= 1 && prioridad <= 3) break;
        printf("Error: Opción no válida.\n");
    } while (1);//si la prioridad no es valida, vuelvo a pedirla.

    eliminar_de_todas_las_listas(alta, media, baja, UsuarioNuevo->id);//elimino al usuario de todas las listas para evitar duplicados.
    UsuarioNuevo->prioridad = prioridad;

    if (prioridad == 1) list_sortedInsert(alta, UsuarioNuevo, lower_than);// aqui lo agrego a la lista de alta prioridad, ordenandolo por hora de ingreso.
    else if (prioridad == 2) list_sortedInsert(media, UsuarioNuevo, lower_than);// aqui lo agrego a la lista de media prioridad, ordenandolo por hora de ingreso.
    else list_sortedInsert(baja, UsuarioNuevo, lower_than);// aqui lo agrego a la lista de baja prioridad, ordenandolo por hora de ingreso.

    printf("Prioridad asignada con éxito.\n");
}

/*
La siguiente funcion se encarga de imprimir un ticket de usuario.
Recibe como argumento el usuario y la prioridad asignada. La función imprime el ID, nombre, problema, prioridad y hora de ingreso del usuario en un formato legible.
*/

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
    const char *prioridades[] = {"Alta", "Media", "Baja"};// aqui declaro un arreglo de cadenas que contiene los nombres de las prioridades.
    List *listas[] = {alta, media, baja};// aqui declaro un arreglo de listas que contiene las listas de prioridades.

    for (int i = 0; i < 3; i++) {
        Usuario *actual = list_first(listas[i]);
        while (actual != NULL) {
            if (actual->id == id) {
                imprimir_ticket(actual, prioridades[i]);
                return 1;
            }
            actual = list_next(listas[i]);
        }// aqui recorro cada lista de prioridades y busco el id que me pasaron por parametro, si lo encuentro, imprimo el ticket y retorno 1.
    }

    Usuario *actual = list_first(Usuarios);
    while (actual != NULL) {
        if (actual->id == id) {
            printf("Usuario encontrado, pero sin prioridad asignada:\n");
            imprimir_ticket(actual, "Sin prioridad");
            return 1;
        }// aqui recorro la lista de usuarios y busco el id que me pasaron por parametro, si lo encuentro, imprimo el ticket y retorno 1.
        actual = list_next(Usuarios);// avanzo al siguiente usuario en la lista.
    }
    return 0;
}

/*
La siguiente funcion se encarga de procesar el siguiente ticket en la lista de prioridades.
Recibe como argumento las listas de alta, media y baja prioridad, y la lista de usuarios. La función verifica cuál es el siguiente ticket a atender (alta, media o baja) y lo procesa, imprimiendo su información y eliminándolo de la lista de usuarios.
*/

void procesar_siguiente_ticket(List *alta, List *media, List *baja, List *Usuarios) {
    Usuario *user = NULL;
    const char *prioridad = "";// inicializo la variable prioridad como una cadena vacía.

    if (list_size(alta) > 0) {
        user = list_popFront(alta);
        prioridad = "Alta";
    } else if (list_size(media) > 0) {
        user = list_popFront(media);
        prioridad = "Media";
    } else if (list_size(baja) > 0) {
        user = list_popFront(baja);
        prioridad = "Baja";
    }// aqui verifico si hay tickets en la lista de alta, media o baja prioridad, y si hay, elimino el primer ticket de la lista correspondiente y lo guardo en la variable user.

    if (user == NULL) {
        printf("No hay tickets pendientes.\n");
        return;// si no hay tickets pendientes, salgo de la funcion.
    }

    printf("Atendiendo ticket (%s prioridad):\n", prioridad);
    imprimir_ticket(user, prioridad);// imprimo el ticket del usuario que estoy atendiendo.
    eliminar_usuario_por_id(Usuarios, user->id);//Luego elimino el usuario ya antendido de la lista de usuarios.
    free(user);
}

/*
La siguiente funcion se encargara de mostrarme lo que sera mi menu principal , la parte visual de la app.
*/
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

/*
La siguiente funcion se encargara de mostrarme lo que sera mi menu de pruebas, la parte visual de la app que solo me funcionara para el testeo de la app, con valores aleatorios.
*/

void mostrarMenuPruebas() {
    limpiarPantalla();
    puts("========================================");
    puts("     Modo de Pruebas");
    puts("========================================");
    puts("1) Generar usuario aleatorio");
    puts("2) Asignar prioridades aleatorias a usuarios existentes");
    puts("3) Volver al menú principal");
}

/*
La siguiente funcion se encargara de leer uan opcion del menu, para poder direccionar al usuario y darle las distintas opciones que tiene para navegar por la interfaz de la app.
Recibe como argumento un mensaje que se mostrará al usuario. La función lee la opción ingresada y la devuelve.
*/

char leerOpcion(const char *mensaje) {
    char opcion;
    printf("%s", mensaje);// le pido al usuario que ingrese una opcion.
    opcion = getchar(); 
    while (getchar() != '\n');// limpio el buffer de entrada para evitar problemas con entradas no deseadas.
    return opcion;
}

/*
La siguiente funcion sera la declaracion de la funcion main, donde se inicializan las listas y se ejecuta el programa.
La función principal crea las listas de usuarios y prioridades, inicializa el generador de números aleatorios y muestra el menú principal. Luego, procesa las opciones seleccionadas por el usuario hasta que se elige salir del programa.      
*/
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
    } while (opcion != '6');// Se trabaja con la variable opcion para que el usuario pueda elegir entre las distintas opciones del menu principal. son 6 casos principales, que permitiran al usuario navegar por la app, teniendo varias acciones para realizar.

    Usuario *actual = list_first(Usuarios);
    while (actual != NULL) {
        free(actual);
        actual = list_next(Usuarios);//Este while recorre la lista principal Usuarios y libera uno por uno los structs de tipo Usuario que fueron reservados dinámicamente con malloc o similar.
    }
    list_clean(Usuarios);
    list_clean(alta);
    list_clean(media);
    list_clean(baja);
    free(Usuarios);
    free(alta);
    free(media);
    free(baja);

    // Libero la memoria de las listas y de los usuarios para evitar fugas de memoria, algo sumamente importante para el correcto funcionamiento de la app.

    return 0;//aqui todo culmina y se retorna 0, indicando que el programa se ejecuto correctamente..
}