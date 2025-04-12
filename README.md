# Sistema de Gestión de Tikets de Usuario.

## Descripción

Este sistema permite a los usuarios gestionar y crear tikets de usuarios para informar de posibles problemas que presente la plataforma a la cual se le asignara esta app. 
Los usuarios pueden registrar nuevos pacientes, asignar prioridades, ver la lista de espera, y más. Esta herramienta está diseñada para mejorar la eficiencia en la atención
de los usuarios, asegurando que aquellos en situaciones de mayor prioridad reciban atención lo más pronto posible.

## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, 
como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal (por ejemplo, `main.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando (ajusta el nombre si el archivo principal tiene otro nombre):
        
        ```bash
        gcc *.c -o tarea1
        ```
        
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```
        ./tarea1
        ```
        

## Funcionalidades

### Testeando el programa


-Al ingresar a la Opcion 0 tendremos que:
-  Se generan usuarios aleatorios con datos básicos simulados y prioridades asignadas automáticamente.
- Las prioridades se asignan de forma aleatoria para facilitar pruebas rápidas y variadas del sistema.
- Se recomienda luego ir a la opcion de volver al menu, para con los datos ya creados porder probar las opciones, 3, 4 y 5.


### Funcionando correctamente:

- Registrar usuarios con sus datos básicos y una prioridad inicial que sera la mas baja.
- Asignar o modificar la prioridad de los usuarios.
- Ver la lista de espera de los usuarios, ordenada por prioridad y hora de registro.
- Atender al siguiente usuario, respetando el orden de prioridad.

### Problemas conocidos:

- Al momento de registrar la hora del usuario esto se hace manualmente, pudiendose mejorar para que la misma este vinculada con la biblioteca time, dando datos mas fidedignos.
- El programa libera la memoria de los usuarios desde la lista principal, pero las listas de prioridades (alta, media, baja) no liberan explícitamente sus nodos antes de liberar los structs Usuario. Esto podría causar problemas en implementaciones futuras si se modifican las estructuras de datos. Se recomienda revisar la gestión de memoria para garantizar consistencia.
- Si la creación de alguna lista falla, el programa termina sin liberar las listas creadas anteriormente. En escenarios con recursos limitados, esto podría causar fugas de memoria. Se planea mejorar el manejo de errores en futuras versiones.
- La generación de IDs únicos para usuarios aleatorios depende de un rango limitado (0-999) y una verificación lineal, lo que puede ser ineficiente para listas grandes. En futuros desarrollos, se considerará usar un rango más amplio o un mecanismo más eficiente, como un contador global.
  
### A mejorar:

- Implementar una interfaz de usuario más amigable.
- Permitir la edición de los datos de los Usuarios.
- Mostrar por pantalla el tiempo de espera aproximado que le queda a cada usuario.
- Ingresar la hora de atencion y fecha de forma automatica mediante la biblioteca time.
- Agregar Lista de problemas comunes que el ususario pueda seleccionar, con la finalidad de agilizar los tiempos de atencion para cada usuario, evitando que todos deban ingresar texto .

## Ejemplo de uso

**Paso 1: Registrar un Nuevo Usuario**

Se comienza registrando un nuevo Usuario que acaba de ingresar a la aplicacion que buscar crear tikets de atencion.

```
Opción seleccionada: 1) Registrar Usuario
Ingrese el nombre del USuario: Pablito Perez
Ingrese el ID del Usuario: 29
Ingrese el problema del Ususario: No me corre el sitema de forma eficiente.
Ingrese su hora en formato hora militar : 14:24
```

El sistema registra a Pablito Perez con una prioridad inicial "Bajo" y guarda la hora actual de registro. La prioridad inicial puede ser ajustada más tarde basada en una evaluación del problema más detallada.

**Paso 2: Asignar Prioridad a un Usuario**

Tras una evaluación inicial, el encargado de estudiar los problemas expuestos por el usuario determinara el nivel de prioridad que se le debe asignar a Pablito.

```
Opción seleccionada: 2) Asignar prioridad a Usuario
Ingrese el nombre del paciente: Pablito Perez
Seleccione el nuevo nivel de prioridad (Alto(1), Medio(2), Bajo(3)): 1
```

El sistema actualiza la prioridad de Pablito Perez a "Alto", asegurando que será un de los próximos ususarios en ser atendido.

**Paso 3: Ver la Lista de Espera**

El usuario actual revisa la lista de espera para ver todos los usuarios y sus prioridades.

```
Opción seleccionada: 3) Mostrar lista de espera
```

La lista muestra a Pablito Perez en la parte superior, indicando su prioridad alta y que es el siguiente en línea para recibir atención.

**Paso 4: Atender al Siguiente Usuario**

Pablito Perez es atendido basándose en su prioridad.

```
Opción seleccionada: 4) Atender al siguiente paciente
```

El sistema muestra que Pablito Perez está siendo atendido y lo elimina de la lista de espera.

```
Opción seleccionada: 5) Mostrar Usuarios por prioridad
```
El sistema permite al usuario ingresar un ID específico para buscar un ticket en las listas de prioridad alta, media, baja y en la lista general.
Si el ticket con el ID ingresado existe, se muestra su información correspondiente.
Si no se encuentra, el sistema notifica al usuario con un mensaje de error indicando que el ID no está registrado.

```
Opción seleccionada: 6) Salir
```
Como su nombre lo indica , ya culminado el uso de la aplicacion, sino se desea hacer mas nada , se sale de la misma .

```
Opción seleccionada: 0) [Modo Pruebas]
```
- Este modo está diseñado para facilitar el testeo del programa sin necesidad de ingresar datos manualmente.
- Al ingresar a esta opción, se muestra un submenú con las siguientes funcionalidades:

  - **Opción 1:** Registra un usuario aleatorio con datos generados automáticamente y lo agrega con prioridad baja por defecto.
  - **Opción 2:** Asigna prioridades aleatorias a todos los usuarios registrados en la lista general, y los distribuye en las listas de prioridad alta, media y baja.
  - **Opción 3:** Permite volver al menú principal.
 
Observación: El codigo a lo largo de su implementación cuenta  con algunos comentarios que serviran para que cualquier persona que desee posiblemente intentar entender su codigo base pueda comprender el mismo, se busco hacer uso de nombre de funciones amigables para que este codigo sea accesible a todo aquel que se interese por entener su estructura. 


- Ideal para realizar pruebas rápidas del sistema y validar el correcto funcionamiento de las funciones de registro, prioridad y organización sin depender del ingreso manual de información.


