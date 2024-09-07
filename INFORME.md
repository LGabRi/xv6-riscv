# INFORME.md

## Tarea 1: Implementación de Llamadas al Sistema en xv6

### Objetivo
El objetivo de esta tarea fue implementar y modificar llamadas al sistema en xv6. Se implementaron dos nuevas llamadas al sistema: `getppid()` para la Parte I y `getancestor(int)` para la Parte II.

## Parte I: Implementación Básica - `getppid()`

### Descripción de la Llamada al Sistema `getppid()`
La llamada al sistema `getppid()` retorna el ID del proceso padre (PID) del proceso que la invoca. Esta función es similar a `getpid()`, pero en lugar de devolver el PID del proceso actual, devuelve el PID de su padre.

### Modificaciones Realizadas

1. **Implementación de la Función de Llamada al Sistema (`sysproc.c`)**:
   Se agregó una nueva función en el archivo `sysproc.c` para manejar la lógica de `getppid()`. Esta función accede a la estructura de datos del proceso actual y retorna el PID de su proceso padre.

2. **Declaración y Mapeo en la Tabla de Llamadas al Sistema (`syscall.c`)**:
   Se declaró la nueva función en el archivo `syscall.c` y se añadió a la tabla de llamadas al sistema en el kernel para que pueda ser invocada desde el espacio de usuario.

3. **Definición de un Número de Sistema Único (`syscall.h`)**:
   Se definió un número único para la llamada al sistema `getppid` en el archivo `syscall.h`, asegurando que no haya conflictos con otras llamadas.

4. **Actualización de los Archivos de Cabecera (`user.h`)**:
   Se añadió la declaración de la función `getppid()` en el archivo de cabecera `user.h` para que los programas de usuario puedan invocar esta llamada al sistema.

5. **Generación de Stubs para el Espacio de Usuario (`usys.pl`)**:
   Se modificó el script `usys.pl` encargado de generar los stubs de las llamadas al sistema para incluir `getppid()`.

6. **Programa de Prueba (`user/yosoytupadre.c`)**:
   Se creó un programa de prueba llamado `yosoytupadre.c` en `user/` que invoca `getppid()` y muestra el PID del proceso padre. Este programa se agregó al sistema de archivos de xv6 y se compiló correctamente.

### Resultados de la Prueba
El programa `yosoytupadre` se ejecutó correctamente en xv6, mostrando el PID del proceso padre como se esperaba. Esto se hizo compilando con un `make clean` y luego `make qemu`, para despues llamar a `yosoytupadre` directamente desde el shell. 

Al ejecutar `yosoytupadre` en la shell de xv6, la salida fue la siguiente:

```sh
$ yosoytupadre
El PID de mi padre es: 2
Mi PID es 5
```

## Parte II: Implementación Avanzada - `getancestor(int)`

### Descripción de la Llamada al Sistema `getancestor(int)`
La llamada al sistema `getancestor(int)` devuelve el PID del ancestro del proceso que la invoca, según el nivel especificado:
- `getancestor(0)` retorna el PID del proceso que invoca la llamada.
- `getancestor(1)` retorna el PID del proceso padre.
- `getancestor(2)` retorna el PID del abuelo.
- Si no hay suficientes ancestros, retorna `-1`.

### Modificaciones Realizadas

1. **Implementación de la Función de Llamada al Sistema (`sysproc.c`)**:
   Se añadió una nueva función en el archivo `sysproc.c` para manejar la lógica de `getancestor(int)`. Esta función recorre la cadena de procesos padres hasta llegar al ancestro indicado por el parámetro o hasta que no haya más ancestros disponibles.

2. **Declaración y Mapeo en la Tabla de Llamadas al Sistema (`syscall.c`)**:
   Se declaró la nueva función `getancestor()` en `syscall.c` y se añadió a la tabla de llamadas al sistema en el kernel para que pueda ser invocada desde el espacio de usuario.

3. **Definición de un Número de Sistema Único (`syscall.h`)**:
   Se asignó un número único para la llamada al sistema `getancestor` en el archivo `syscall.h`, garantizando que no se superponga con otras llamadas al sistema.

4. **Actualización de los Archivos de Cabecera (`user.h`)**:
   Se añadió la declaración de la función `getancestor(int)` en el archivo de cabecera `user.h` para que los programas de usuario puedan invocar esta llamada al sistema.

5. **Generación de Stubs para el Espacio de Usuario (`usys.pl`)**:
   Se modificó `usys.pl`, el cual se encarga de generar los stubs de las llamadas al sistema para incluir `getancestor(int)`.

6. **Actualización del Programa de Prueba (`user/yosoytupadre.c`)**:
   Se modificó el programa de prueba `yosoytupadre.c` en el directorio `user/` para incluir pruebas de la nueva llamada al sistema `getancestor(int)`. El programa ahora muestra el PID del proceso actual, del padre, del abuelo y maneja correctamente el caso en que no haya más ancestros.

### Resultados de la Prueba
Al ejecutar `yosoytupadre` en xv6, el programa mostró correctamente los PIDs correspondientes para los ancestros solicitados. La salida incluyó tanto el proceso padre como los ancestros hasta el nivel máximo disponible, y devolvió `-1` cuando se solicitó un nivel que no existe, verificando que la implementación de `getancestor(int)` es correcta.

Al ejecutar `yosoytupadre` en la shell de xv6, la salida fue la siguiente:

```sh
El PID de mi padre es: 2
Mi PID es 5
Ancestro 0 (yo mismo): 5
Ancestro 1 (padre): 2
Ancestro 2 (abuelo): 1
Ancestro no válido (-1): -1
Proceso hijo: mi ancestro 0 es 6 (yo mismo)
Proceso hijo: mi ancestro 1 es 5 (padre)
Proceso hijo: mi ancestro 2 es 2 (abuelo)
Proceso hijo: ancestro no válido (-1): 1
```

## Dificultades Encontradas y Soluciones

1. **Errores de Compilación por Declaraciones Implícitas**:
   Se encontró un error de compilación debido a declaraciones implícitas. Para resolver esto, se añadió la declaración de las funciones `getppid()` y `getancestor()` en el archivo de cabecera correspondiente (`user.h`).

2. **Problemas de Vínculo (`undefined reference`)**:
   Hubo errores de referencia indefinida al no mapear correctamente las funciones en la tabla de llamadas al sistema. Este problema se solucionó asegurando que todas las declaraciones y asignaciones en la tabla de llamadas al sistema estuvieran correctas.

## Conclusión
La implementación de las llamadas al sistema `getppid()` y `getancestor(int)` en xv6 fue exitosa. Ambas funciones funcionan según lo esperado y proporcionan las funcionalidades necesarias para interactuar con los procesos y sus ancestros en el sistema operativo.