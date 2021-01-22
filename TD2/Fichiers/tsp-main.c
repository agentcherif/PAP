#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <sys/time.h>
// #include <omp.h>

#define MAX_NBVILLES 22

typedef int DTab_t[MAX_NBVILLES][MAX_NBVILLES];
typedef int chemin_t[MAX_NBVILLES];

/* macro de mesure de temps, retourne une valeur en �secondes */
#define TIME_DIFF(t1, t2) \
  ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec))

/* dernier minimum trouv� */
int minimum = INT_MAX;

/* tableau des distances */
DTab_t distance;

/* nombre de villes */
int nbVilles;

/* profondeur du parallélisme */
int grain;

#define MAXX 100
#define MAXY 100
typedef struct
{
  int x, y;
} coor_t;

typedef coor_t coortab_t[MAX_NBVILLES];

void initialisation(int Argc, char *Argv[])
{

  if (Argc < 4 || Argc > 5)
  {
    fprintf(stderr, "Usage: %s  <nbVilles> <seed> [grain] <kernel>\n", Argv[0]);
    exit(1);
  }

 grain = (Argc == 5) ? atoi(Argv[3]) : 0;


  /* initialisation du tableau des distances */
  /* on positionne les villes aléatoirement sur une carte MAXX x MAXY  */

  coortab_t lesVilles;

  int i, j;
  int dx, dy;

  nbVilles = atoi(Argv[1]);
  if (nbVilles > MAX_NBVILLES)
  {
    fprintf(stderr, "trop de villes, augmentez MAX_NBVILLES\n");
    exit(1);
  }

  srand(atoi(Argv[2]));

  for (i = 0; i < nbVilles; i++)
  {
    lesVilles[i].x = rand() % MAXX;
    lesVilles[i].y = rand() % MAXY;
  }

  for (i = 0; i < nbVilles; i++)
    for (j = 0; j < nbVilles; j++)
    {
      dx = lesVilles[i].x - lesVilles[j].x;
      dy = lesVilles[i].y - lesVilles[j].y;
      distance[i][j] = (int)sqrt((double)((dx * dx) + (dy * dy)));
    }
}

/* résolution du problème du voyageur de commerce */

inline int present(int ville, int mask)
{
  return mask & (1 << ville);
}

void verifier_minimum(int lg, chemin_t chemin)
{
  // if (lg + distance[0][chemin[nbVilles - 1]] < minimum)
  // {
    if (lg + distance[0][chemin[nbVilles - 1]] < minimum)
    {
      #pragma omp critical
      {
      minimum = lg + distance[0][chemin[nbVilles - 1]];
      printf("%3d :", minimum);
      for (int i = 0; i < nbVilles; i++)
        printf("%2d ", chemin[i]);
      printf("\n");
      }
    }
  // }
}

void tsp_seq(int etape, int lg, chemin_t chemin, int mask)
{
  int ici, dist;

  if (etape == nbVilles)
    verifier_minimum(lg, chemin);
  else
  {
    ici = chemin[etape - 1];

    for (int i = 1; i < nbVilles; i++)
    {
      if (!present(i, mask))
      {
        chemin[etape] = i;
        dist = distance[ici][i];
        tsp_seq(etape + 1, lg + dist, chemin, mask | (1 << i));
      }
    }
  }
}

void tsp_ompfor(int etape, int lg, chemin_t chemin, int mask)
{
  // if (etape > grain) { // version séquentielle tsp_seq(...);
  //   tsp_seq(1, 0, chemin, 1);
  //   } else { // version parallèle #pragma omp parallel for

  //   }
  int ici, dist;
  
  if (etape == nbVilles)
    verifier_minimum(lg, chemin);
  else
  {
    #pragma omp parallel
    {
      chemin_t tmp_chemin;
      memcpy(tmp_chemin, chemin, etape * sizeof(chemin_t)); //chemin[0]

      ici = chemin[etape - 1];
      #pragma omp for 
      for (int i = 1; i < nbVilles; i++)
      {
        if (!present(i, mask))
        {
          tmp_chemin[etape] = i;
          // chemin[etape] = i;
          dist = distance[ici][i];
          tsp_seq(etape + 1, lg + dist, tmp_chemin, mask | (1 << i));
          // tsp_seq(etape + 1, lg + dist, chemin, mask | (1 << i));
        }
      }
    }
  }
}

int main(int argc, char **argv)
{
  unsigned long temps;
  struct timeval t1, t2;
  chemin_t chemin;
  // set OMP_NESTED=FALSE 
  initialisation(argc, argv);

  printf("nbVilles = %3d - grain %d \n", nbVilles, grain);

  //omp_set_max_active_levels(grain);

  gettimeofday(&t1, NULL);

  chemin[0] = 0;

  if (!strcmp(argv[argc-1],"seq")) 
    tsp_seq(1, 0, chemin, 1);
  else if (!strcmp(argv[argc-1],"ompfor")) 
    tsp_ompfor(1, 0, chemin, 1);
  else
  {
      printf("kernel inconnu\n");
      exit(1);
  }
      
  gettimeofday(&t2, NULL);

  temps = TIME_DIFF(t1, t2);
  fprintf(stderr, "%ld.%03ld\n", temps / 1000, temps % 1000);

  return 0;
}
