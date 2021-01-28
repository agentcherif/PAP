#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int traitement(int i){
  int x = i * i;
  printf("le double de %d est %d\n",i,x);
}
int
main()
{
 #pragma omp parallel
 #pragma omp for schedule(static,10)
 for(int i=0;i<10;i++) 
  traitement(i);

 return 0;
}
