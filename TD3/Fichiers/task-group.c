#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <omp.h>

int id = 0;

void creer_tache(char *nom, int maman)
{
#pragma omp task firstprivate(nom,maman)
#pragma omp task firstprivate(nom,maman)
      {
	sleep(1);
	printf("%s [petite fille de %d]\n",nom,maman);
      }
}

  
int main()
{
#pragma omp parallel num_threads(4)
  {
  int me ;
  #pragma omp atomic capture
  me = id++;
#pragma omp single nowait
  #pragma omp taskgroup
  {
    printf("tache %d va créer A et B \n", me);
    creer_tache("A",me);
    creer_tache("B",me);
  }

#pragma omp taskwait
#pragma omp single nowait
  #pragma omp taskgroup
    {
      printf("tâche %d va créer C et D \n", me);
      creer_tache("C",me);
      creer_tache("D",me);
    }
    
  // }
#pragma omp taskwait
    printf("tache %d a passé taskgroup \n", me);
}
  return 0;
}
