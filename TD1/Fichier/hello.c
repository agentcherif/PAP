#include <stdio.h>
#include <omp.h>
int main()
{
  #pragma omp critical
  #pragma omp parallel
  {
    printf("nb thread  %d qui execute ce Bonjour !\n",omp_get_thread_num());
    // #pragma omp barrier
    printf("nb thread  %d qui dit Au revoir !\n",omp_get_thread_num());
  }

  return 0;
}

//cat /proc/cpuinfo | grep processor | wc -l savoir le nombre de coeur d'une machine lunix
//sysctl hw.physicalcpu savoir le nombre de coeur d'une machine mac
// sysctl hw.logicalcpu savoir le nombre de coeur d'une machine mac