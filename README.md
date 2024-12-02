# Implementación de Permisos Básicos en xv6 (RISC-V)

## Funcionamiento y lógica
Se implementó un sistema de permisos para archivos en xv6, permitiendo asignar permisos de lectura, escritura y ejecución. Además, se añadió una característica de inmutabilidad que protege los archivos de modificaciones, eliminaciones o cambios.


## Modificaciones realizadas

### kernel/fs.h: Añadimos el campo permission en la estructura inode para almacenar los permisos de cada archivo.
### kernel/fs.c: Actualizamos las funciones de lectura y escritura para verificar los permisos antes de realizar operaciones.
### kernel/sysfile.c: Modificamos sys_open para incluir la verificación de permisos al abrir archivos.
### kernel/syscall.h y kernel/syscall.c: Agregamos la llamada al sistema chmod para permitir cambiar los permisos de archivos.
### user/chmod.c: Implementamos el comando chmod para que los usuarios puedan modificar permisos desde la línea de comandos.

### RESULTADO: 

Creando un archivo chmodtest.c: 

```c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd;
  char *filename = "testfile";
  char *data = "Hello, xv6!";

  fd = open(filename, O_CREATE | O_RDWR);
  if(fd < 0){
    printf("Error: No se pudo crear el archivo\n");
    exit(1);
  }

  if(write(fd, data, strlen(data)) != strlen(data)){
    printf("Error: No se pudo escribir en el archivo\n");
    close(fd);
    exit(1);
  }
  close(fd);

  if(chmod(filename, 1) < 0){
    printf("Error: chmod a solo lectura falló\n");
    exit(1);
  }

  fd = open(filename, O_WRONLY);
  if(fd >= 0){
    printf("Error: Se abrió el archivo en modo escritura sin permiso\n");
    close(fd);
  } else {
    printf("Correcto: No se pudo abrir el archivo en modo escritura\n");
  }

  if(chmod(filename, 3) < 0){
    printf("Error: chmod a lectura/escritura falló\n");
    exit(1);
  }

  fd = open(filename, O_RDWR);
  if(fd < 0){
    printf("Error: No se pudo abrir el archivo en modo lectura/escritura\n");
    exit(1);
  }
  if(write(fd, data, strlen(data)) != strlen(data)){
    printf("Error: No se pudo escribir en el archivo después de cambiar permisos\n");
    close(fd);
    exit(1);
  }
  close(fd);

  printf("Pruebas completadas exitosamente\n");
  exit(0);
}
```
El test realiza pruebas básicas para verificar el correcto funcionamiento del sistema de permisos en xv6:
- Creación y escritura inicial: Se crea un archivo llamado testfile con permisos de lectura/escritura y se escribe un mensaje inicial.
- Cambio a solo lectura: Se cambia el permiso del archivo a solo lectura usando chmod(filename, 1) y se verifica que no se puede abrir en modo escritura.
- Cambio a lectura/escritura: Se restablecen los permisos a lectura/escritura con chmod(filename, 3) y se verifica que se puede escribir nuevamente.


## Dificultades encontradas y soluciones implementadas
### Conflictos de tipos en argint y funciones similares
Las funciones estaban definidas con el tipo de retorno void en lugar de int, lo que causaba errores de compilación al no retornar valores esperados

## Solución:
Cambié el tipo de retorno a int y añadimos valores de retorno adecuados (0 para éxito, -1 para error).

### Errores al verificar permisos en operaciones de archivo: 
Las verificaciones de permisos en readi, writei y sys_open no manejaban correctamente la lógica de lectura/escritura ni consideraban archivos inmutables.

### Solución: 
Incorporamos condiciones para validar los permisos antes de realizar operaciones, devolviendo errores cuando no se cumplían los requisitos.
