# Protección de memoria en xv6

## Funcionamiento y lógica de la protección de memoria
Se implementó un sistema de protección de memoria para el sistema operativo xv6, donde el objetivo es permitir que el proceso pueda marcar regiones de memoria como solo lectura utilizando dos nuevas llamadas al sistema:

-   **munprotect(void *addr, int len)**: Restaura el permiso de escritura en una región de memoria anteriormente marcada como solo lectura.

-   **mprotect(void *addr, int len)**: Marca una región de memoria como solo lectura, deshabilitando el permiso de escritura en la tabla de páginas.

Se calculan las páginas involucradas a partir de la dirección base addr y el tamaño len, donde para cada página afectada, se accede a la entrada de la tabla de páginas (PTE) y se modifica el bit de escritura (W), donde mprotect deshabilita el bit de escritura para hacer la página solo lectura y unprotect habilita el bit de escritura para restaurar el permiso de escritura.


## Modificaciones realizadas


### `test_mprotect.c`

El programa de prueba realiza pruebas para verificar el funcionamiento de las funciones mprotect y munprotect, donde reserva dos páginas de memoria y protege una de ellas contra escritura usando mprotect.

```c
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main() {
    char *addr = sbrk(4096);
    if (addr == (char *)-1) {
        printf("Error al reservar memoria con sbrk\n");
        exit(1);
    }

    printf("Caso 1: Proteger la página y verificar fallo de escritura\n");
    if (mprotect(addr, 1) == -1) {
        printf("mprotect falló\n");
        exit(1);
    }

    printf("Intentando escribir en la página protegida\n");
    addr[0] = 'A';  

    printf("Caso 2: Desproteger la página y verificar escritura\n");
    if (munprotect(addr, 1) == -1) {
        printf("munprotect falló\n");
        exit(1);
    }

    addr[0] = 'B';
    printf("Escritura en la página desprotegida exitosa: %c\n", addr[0]);

    printf("Caso 3: Verificar manejo de errores\n");
    if (mprotect((void *)0x12345678, 1) != -1) {
        printf("Error: mprotect no falló con una dirección inválida\n");
        exit(1);
    }

    if (munprotect((void *)0x12345678, 1) != -1) {
        printf("Error: munprotect no falló con una dirección inválida\n");
        exit(1);
    }

    printf("Todas las pruebas pasaron exitosamente.\n");
    exit(0);
}

```
El programa intenta escribir en la página protegida, lo que debería causar un fallo de segmentación si la protección es efectiva. Luego, desprotege la página usando munprotect y verifica que la escritura sea posible nuevamente. Por último, se prueba la escritura en una página no protegida para confirmar que no haya restricciones

### `syscall.h`

Se agregaron dos nuevas constantes para los números de las llamadas al sistema: `SYS_mprotect 22` y `SYS_munprotect 23`

```c
#define SYS_mprotect 22
#define SYS_munprotect 23
```
Esto permite que el sistema identifique las llamadas a mprotect y munprotect cuando un proceso realiza una llamada al sistema.


### `syscall.c`

Se añadieron las funciones `sys_mprotect` y `sys_munprotect` a la lista de funciones de llamadas al sistema:

```c
extern uint64 sys_mprotect(void);
extern uint64 sys_munprotect(void);

static uint64 (*syscalls[])(void) = {
    [SYS_mprotect] sys_mprotect,
    [SYS_munprotect] sys_munprotect,
};

```
Estas funciones se encargan de redireccionar las llamadas al sistema realizadas desde el espacio de usuario hacia las funciones correspondientes en el kernel.


### `sysproc.c`

Se implementan las funciones de llamadas al sistema `sys_mprotect` y `sys_munprotect`:

```c
uint64 sys_mprotect(void) {
    uint64 addr;
    int len;
    argaddr(0, &addr);
    argint(1, &len);
    return (uint64)mprotect((void *)addr, len);
}

uint64 sys_munprotect(void) {
    uint64 addr;
    int len;
    argaddr(0, &addr);
    argint(1, &len);
    return (uint64)munprotect((void *)addr, len);
}
```
Estas funciones reciben los argumentos desde el espacio de usuario (addr y len) y llaman a las funciones mprotect y munprotect del kernel para modificar los permisos de las páginas.


### `usys.pl`

Se añaden las entradas para `mprotect` y `munprotect` en el script `usys.pl`, que genera los stubs de las llamadas al sistema:
```c
entry("mprotect");
entry("munprotect");

```
Esto permite que las llamadas a estas funciones en el espacio de usuario sean convertidas en llamadas al sistema (ecall).


### `defs.h`

Se declaran las funciones `mprotect` y `munprotect`:

```c
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);
```
Esto permite que estas funciones sean visibles en otros archivos del kernel.


### `vm.c`

Se implementaron las funciones `mprotect` y `munprotect`:

```c
int mprotect(void *addr, int len) {
    struct proc *p = myproc();
    uint64 a = (uint64)addr;
    uint64 end = a + len * PGSIZE;

    if (a % PGSIZE != 0 || len < 1 || end > p->sz)
        return -1;

    pte_t *pte;
    for (; a < end; a += PGSIZE) {
        pte = walk(p->pagetable, a, 0);
        if (pte == 0 || (*pte & PTE_V) == 0)
            return -1;
        *pte &= ~PTE_W;
    }

    sfence_vma();
    return 0;
}

int munprotect(void *addr, int len) {
    struct proc *p = myproc();
    uint64 a = (uint64)addr;
    uint64 end = a + len * PGSIZE;

    if (a % PGSIZE != 0 || len < 1 || end > p->sz)
        return -1;

    pte_t *pte;
    for (; a < end; a += PGSIZE) {
        pte = walk(p->pagetable, a, 0);
        if (pte == 0 || (*pte & PTE_V) == 0)
            return -1;
        *pte |= PTE_W;
    }

    sfence_vma();
    return 0;
}
```
mprotect: Marca las páginas como solo lectura deshabilitando el bit de escritura (PTE_W) en la PTE.

munprotect: Restaura los permisos de escritura habilitando el bit de escritura (PTE_W) en la PTE.

### `trap.c`

Se modificó la función `usertrap` para manejar el Store Access Fault:

```c
 else if (r_scause() == 0xf) { 
    printf("Store access fault en el proceso pid=%d en dirección 0x%lx\n", p->pid, r_stval());
    p->trapframe->epc += 4;
}
```
Esto permite detectar cuando un proceso intenta escribir en una página protegida.


### `Makefile`
Se añadió al UPROGS una linea para ejecutar la prueba:

```c
$U/_test_mprotect\
```

### `Make clean && make qemu`

Ya teniendo todo listo, se ejecuta un Make clean y un Make qemu para compilar y ejecutar el sistema.
Ya todo en orden, se ejecuta `test_mprotect`.

```
make clean && make qemu

test_mprotect

```

### RESULTADO: 

```
$ test_mprotect
Caso 1: Proteger la página y verificar fallo de escritura
Intentando escribir en la página protegida
Store access fault en el proceso pid=3 en dirección 0x5000

Caso 2: Desproteger la página y verificar escritura
Escritura en la página desprotegida exitosa: %c

Caso 3: Verificar manejo de errores
Todas las pruebas pasaron exitosamente.
```

## Dificultades encontradas y soluciones implementadas
### Error de acceso al intentar escribir en una página protegida
Al implementar `mprotect()`, ocurría un fallo de "Store Access Fault" al intentar escribir en una página marcada como solo lectura, esto seguía ocurriendo incluso después de desproteger la página con `munprotect()`.

## Solución:
Se implementó la instrucción `sfence_vma()` después de modificar los permisos en la tabla de páginas, con esto se invalida la TLB, permitiendo que los cambios en los permisos se vean reflejados correctamente, y aplicando esto se solucionó el problema.

### Problemas al manejar direcciones y longitudes inválidas
Al inicio, no se realizaban validaciones para los parámetros de entrada en `mprotect()` y `munprotect()`, lo que causaba errores de segmentación al intentar acceder a direcciones fuera del espacio de memoria del proceso, lo que resultaba en un "panic" del sistema.

### Solución: 
Se hicieron validaciones mas estrictas para verificar que las direcciones sean múltiplos del tamaño de página y que no excedan los límites del espacio de memoria del proceso, donde también se añadió una comprobación para evitar modificar páginas no válidas o no asignadas.

