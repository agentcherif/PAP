#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int f(int i){
  sleep(1);
  return 2*i;
}

int g(int i){
  sleep(1);
  return 2 * i + 1;
}

int main()
{
   
  #pragma omp parallel
  {
    #pragma omp master
    {
    int x;
    int y;
    #pragma omp task shared(x) //firstprivate(y)
    x = f(2);

    #pragma omp task shared(y) //firstprivate(x)
    y = g(3);
    #pragma omp taskwait
    printf("résultat %d\n", x+y);
  }
}
  
  
  return 0;
}
