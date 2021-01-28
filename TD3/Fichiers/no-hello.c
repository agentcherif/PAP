#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main()
{
  #pragma omp parallel  
  {
    int me = omp_get_thread_num();
    #pragma omp task firstprivate(me)
    printf("Bonjour de %d executer part %d\n",me,omp_get_thread_num() );
    #pragma omp task firstprivate(me)
    printf("\tAu revoir de %d executer part %d\n",me,omp_get_thread_num() );    
  }
  return 0;
}
