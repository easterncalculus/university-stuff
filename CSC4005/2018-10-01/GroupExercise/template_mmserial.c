#include <stdio.h>
#include <stdlib.h>               
#include <time.h>
#include "timing.h"

#define ORDER 769u
const int test_sizes[] = {
  31, 32, 96, 97, 127, 128, 129, 191, 192, 229,
  255, 256, 257, 319, 320, 321, 417, 479, 480, 511, 512, 639, 640,
  767, 768, 769};
#define N_SIZES (sizeof (test_sizes) / sizeof (int))
static double matmul (const int M, double **a, double **b, double **c);

int main (int argc, char *argv[]) 
{
int	i, j, l, x;

double	** a;
double	** b;
double	** c;
double mflop_s;

/* Allocate pointer memory for the first dimension of a matrix[][]; */
a = (double **) malloc(ORDER * sizeof(double *));
if(NULL == a){free(a); printf("Memory allocation failed while allocating for a[].\n"); exit(-1);}
b = (double **) malloc(ORDER * sizeof(double *));
if(NULL == b){free(b); printf("Memory allocation failed while allocating for b[].\n"); exit(-1);}
c = (double **) malloc(ORDER * sizeof(double *));
if(NULL == c){free(c); printf("Memory allocation failed while allocating for c[].\n"); exit(-1);}

/* Allocate integer memory for the second dimension of a matrix[][]; */
for(x = 0; x < ORDER; x++)
{
  a[x] = (double *) malloc(ORDER * sizeof(double));
  if(NULL == a[x]){free(a[x]); printf("Memory allocation failed while allocating for a[x][].\n"); exit(-1);}
  b[x] = (double *) malloc(ORDER * sizeof(double));
  if(NULL == b[x]){free(a[x]); printf("Memory allocation failed while allocating for b[x][].\n"); exit(-1);}
  c[x] = (double *) malloc(ORDER * sizeof(double));
  if(NULL == c[x]){free(a[x]); printf("Memory allocation failed while allocating for c[x][].\n"); exit(-1);}
}

  /*** Initialize matrices ***/
  for (i=0; i<ORDER; i++)
    for (j=0; j<ORDER; j++)
      a[i][j]= drand48 ();

  for (i=0; i<ORDER; i++)
    for (j=0; j<ORDER; j++)
      b[i][j]= drand48 ();

   for (l = 0; l < N_SIZES; ++l) {
	   const int M = test_sizes[l];
	   mflop_s = matmul(M, a, b, c);
	   printf ("Size: %u\tmflop/s: %lg\n", M, mflop_s);
   }	
}

double matmul (const int M, double **a, double **b, double **c)
{
	struct timespec start, finish;
	double flops, mflops, mflop_s, secs, result;
  int	i, j;
  get_time (&start);
  
  for (i=0; i<M; i++){
    for (j=0; j<M; j++){
      result = 0;
      for (int x=0; x<M; x++){
        result = result + (a[i][x] * b[x][j]);
        //two floating point operations
      }
      c[i][j] = result;
    }
  }
  
  get_time (&finish); 
  secs = timespec_diff (start, finish);
  flops = ((2*M*M*M));
  mflops = flops / 1000000;
  mflop_s = mflops / secs;
	return mflop_s;
}
