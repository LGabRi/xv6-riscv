#include "kernel/types.h"
#include "user/user.h"

int main() {
    // Prueba de getppid()
    int ppid = getppid();  // Llamada al sistema getppid()
    printf("El PID de mi padre es: %d\n", ppid);  // Imprime el resultado

    // Prueba de getancestor()
    printf("Mi PID es %d\n", getpid());
    printf("Ancestro 0 (yo mismo): %d\n", getancestor(0));  // Debe ser igual a getpid()
    printf("Ancestro 1 (padre): %d\n", getancestor(1));  // Debe ser igual a getppid()
    printf("Ancestro 2 (abuelo): %d\n", getancestor(2));  // Puede ser el PID del abuelo
    printf("Ancestro no válido (-1): %d\n", getancestor(3));  // Debe devolver -1 si no hay suficiente ancestro

    // Proceso hijo para probar las llamadas al sistema en un proceso separado
    int pid = fork();
    if (pid == 0) {  // Proceso hijo
        printf("Proceso hijo: mi ancestro 0 es %d (yo mismo)\n", getancestor(0));
        printf("Proceso hijo: mi ancestro 1 es %d (padre)\n", getancestor(1));
        printf("Proceso hijo: mi ancestro 2 es %d (abuelo)\n", getancestor(2));
        printf("Proceso hijo: ancestro no válido (-1): %d\n", getancestor(3));  // Debe devolver -1
        exit(0);
    } else {  // Proceso padre
        wait(0);  // Espera a que el proceso hijo termine
    }

    exit(0);
}
