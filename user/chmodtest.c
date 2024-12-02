// user/chmodtest.c

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
