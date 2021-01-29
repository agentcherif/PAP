#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

void generer()
{
  char chaine[]="jusqu'ici tout va bien";
  
  for(int i = 0; i < 10 ; i++)
#pragma omp task firstprivate(chaine) firstprivate(i)
    printf("tache %d par thread %d >>>> %s <<<< \n",
	   i,omp_get_thread_num(), chaine);
  
// #pragma omp taskwait
}


int main()
{
  
#pragma omp parallel
  {
#pragma omp single
    {
      generer();
      printf("%d est sorti de generer \n", omp_get_thread_num());
    }
  }
  return 0;
}
  
