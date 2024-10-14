# Sistema de Prioridades en xv6

## Funcionamiento y Lógica del Sistema de Prioridades

El objetivo de esta tarea fue modificar el scheduler para implementar un sistema de prioridades en xv6. Cada proceso tiene dos nuevos campos:

-   **Priority**: Es un valor que indica el orden en el que los procesos deben ejecutarse. En este caso, un número más bajo de priority indica una mayor prioridad. Al comienzo, todos los procesos inician con una prioridad de 0, que es la prioridad más alta.

-   **Boost**: Es un valor que indica cuánto aumenta o disminuye la prioridad de un proceso en cada ciclo del scheduler. Los procesos comienzan con un boost de 1, lo que significa que su prioridad aumenta en cada ciclo de scheduler. Cuando el boost es -1, la prioridad del proceso disminuye en cada ciclo.

### Funcionalidad

-   Al iniciar, la prioridad de todos los procesos se establece en 0, lo que significa que todos los procesos tienen la prioridad más alta.

-   Cada vez que un proceso entra en el scheduler, su prioridad se incrementa por el valor del boost. Al comienzo, el boost es 1, lo que hace que la prioridad del proceso aumente progresivamente.

-   Si la prioridad de un proceso llega al valor 9, que es la prioridad más baja, el boost cambia a -1. Esto significa que en los siguientes ciclos, la prioridad del proceso comenzará a disminuir para ajustarse a una posible mejor prioridad.

-   Si la prioridad de un proceso llega al valor 0, que es la prioridad más alta, el boost vuelve a cambiar a 1, de manera que en los siguientes ciclos la prioridad comenzará a subir.

Este mecanismo asegura que los procesos no queden estancados con una prioridad baja o alta indefinidamente, manteniendo un balance entre los procesos.

## Modificaciones Realizadas

### `proc.h`

Se agregaron en el `proc.h` dos valores: `priority` y `boost`

```c
int priority;
int boost;
```

### `proc.c`

En el `proc.c` se implementaron cambios en dos funciones: `allocproc()` y `scheduler()`
En `allocproc()` primeramente se inicializan los valores `priority` y `boost`

```c
p->priority = 0;
p->boost = 1;
```

En `scheduler()` se implementa la lógica que aumenta la prioridad de los procesos, ajustando los valores de boost cuando la prioridad alcanza los valores de 0 o 9

```c
p->priority += p->boost;

    if (p->priority >= 9)
    {
      p->boost = -1;
    }

    if (p->priority <= 0)
    {
        p->boost = 1;
    }
```
### `prioritytest.c`

Se creó un programa de prueba que utiliza fork() para crear 20 procesos y que cada proceso imprima un mensaje que indica su PID, esperando un tiempo con un sleep() para que se vea correctamente la ejecución de estos 20 procesos con el sistema de prioridades. 

```c

#include "kernel/types.h"
#include "user/user.h"

void
test_priority_system(void)
{
  int pid;
  for (int i = 0; i < 20; i++) {
    pid = fork();  
    if (pid < 0) {
      printf("Error al crear proceso\n");
      exit(1);
    }

    if (pid == 0) {
      printf("Ejecutando proceso %d\n", getpid());
      sleep(10);  
      exit(0);  
    }
    
    sleep(10);  
  }

  for (int i = 0; i < 20; i++) {
    wait(0);
  }

  exit(0);
}

int
main(void)
{
  test_priority_system();
  exit(0);
}
```

### `Makefile`
Se añadió al UPROGS una linea para ejecutar la prueba:

```c

$U/_prioritytest\

```

### `Make clean && make qemu`

Ya teniendo todo listo, se ejecuta un Make clean y un Make qemu para compilar y ejecutar el sistema.
Ya todo en orden, se ejecuta `prioritytest` y comienzan a salir los mensajes de los procesos en orden.

```
make clean && make qemu

priority test

```

### RESULTADO: 

```
$ prioritytest
Ejecutando proceso 4
Ejecutando proceso 5
Ejecutando proceso 6
Ejecutando proceso 7
Ejecutando proceso 8
Ejecutando proceso 9
Ejecutando proceso 10
Ejecutando proceso 11
Ejecutando proceso 12
Ejecutando proceso 13
Ejecutando proceso 14
Ejecutando proceso 15
Ejecutando proceso 16
Ejecutando proceso 17
Ejecutando proceso 18
Ejecutando proceso 19
Ejecutando proceso 20
Ejecutando proceso 21
Ejecutando proceso 22
Ejecutando proceso 23

```

## Dificultades Encontradas y Soluciones Implementadas
### Salida desordenada de los procesos: 
Al comienzo, la salida de los procesos estaba mezclada debido a que varios procesos intentaban escribir en la consola al mismo tiempo, esto hacía que los mensajes se mezclaran y no fuera clara la salida.

## Solución:
Se implementó el sleep() con pequeñas pausas entre la creación de los procesos en el programa de prueba, ya que así los procesos se toman un tiempo en la ejecución permitiendo ver de manera mas clara la salida. 

### Corrupción de XV6: 
Al inicio, pasó que me salía un error de panic, y no se ejcutaba nada. Se debía a 2 posibles cosas, las cuales eran que se modifico mal el scheduler o el allocproc o en el Makefile se coloco mal el programa de prueba. Según yo es la primera.

### Solución: 
Se hizo todo de nuevo desde 0, modificando con mas cuidado y atencion el scheduler y el allocproc. Después en el Makefile se puso de nuevo el programa de prueba con mucha mas cautela y funcionó.

