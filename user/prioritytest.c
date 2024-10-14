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
