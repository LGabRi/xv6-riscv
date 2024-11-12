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
