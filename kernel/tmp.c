#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int fibo(int n);
int main(){
// #pragma omp parallel master
// {
//     for (int i = 0; i < 10; i++)
//       #pragma omp task //firstprivate(me)
//        printf("hello, executer par %d\n", omp_get_thread_num());
//     #pragma omp taskwait
//      printf("bye\n");
//    // }
// }//barriere



// #pragma omp for
// for (unsigned i = 0; i < 20 * 10; i++)
//     printf("%d----->%d\n",i/20,i%20);


// #pragma omp for collapse(2)
// for (unsigned i = 0; i < 10; i++)
//  for (unsigned j = 0; j < 20; j++)
//     printf("%d----->%d\n",i,j);



// #pragma omp parallel 
// {
//     for (int i = 0; i < 1; i++)
//       #pragma omp task //firstprivate(me)
//        printf("hello, executer par %d\n", omp_get_thread_num());
//     #pragma omp barrier
//      printf("bye\n");
//    // }
// }//barriere

// int n ;
// #pragma omp parallel master shared(n)
// n = fibo(10);

// printf("fibo 10 est : %d",n);

// int a;
// #pragma omp parallel master
// {
//     #pragma omp task depend(out:a)
//     a = 1;
//     #pragma omp task depend(inout:a)
//     a++;
//     #pragma omp task depend(in:a)
//     printf("%d",a);
// }

// int a,b;
// #pragma omp parallel master
// {
//     #pragma omp task depend(out:a)
//     {a = 0;}
    
//     #pragma omp task depend(mutexinoutset:a)
//     a+=1;
//     #pragma omp task depend(mutexinoutset:b)
//     a+=2;
//     #pragma omp task depend(in:a,b)
//     printf("%d",a);
// }


// #pragma omp parallel master
// {
//     #pragma omp task
//     {printf("A1");printf("A2");}
//     #pragma omp task
//     {printf("B1");printf("B2");}
//     #pragma omp task
//     {printf("C1");printf("C2");}
// }

// #pragma omp parallel master
// {
//     #pragma omp task
//     {printf("A1");}
//     #pragma omp task
//     {printf("B1");}
//     #pragma omp task
//     {printf("C1");}
//     #pragma omp taskwait
//     #pragma omp task
//     {printf("A2");}
//     #pragma omp task
//     {printf("B2");}
//     #pragma omp task
//     {printf("C2");}
// }
int a,b,c;
#pragma omp parallel master
{
    #pragma omp task depend(out:a)
    {printf("A1");}
    #pragma omp task depend(out:b)
    {printf("B1");}
    #pragma omp task depend(out:c)
    {printf("C1");}
    #pragma omp task depend(in:a,b)
    {printf("A2");}
    #pragma omp task depend(in:a,b,c)
    {printf("B2");}
    #pragma omp task depend(in:b,c)
    {printf("C2");}
}

return 0;
}

int fibo(int n){
    int x,y;
    if(n<2) return n;
    #pragma omp task shared(x,n)
        x = fibo(n-1);
    #pragma omp task shared(y,n)
        y = fibo(n-2);
    #pragma omp taskwait
    return x+y;
}