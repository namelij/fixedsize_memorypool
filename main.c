#include "memory_pool.h"
#include <sys/time.h>
#include <malloc.h>
#include <stdio.h>

int main() {
  s_memory_pool *mp = memory_pool_create(4);
  struct timeval start;
  struct timeval end;

  int t[] = {20000, 40000, 80000, 100000, 120000, 140000, 160000, 180000, 200000};
  int s = sizeof(t)/sizeof(int);

  int i = 0;
  for (; i < s; ++i) {
    gettimeofday(&start, NULL); 
    int j = 0;
    for (; j < t[i]; ++j) {
     
      void *p = memory_alloc(mp);
      memory_free(mp, p);
     //
     //void *p = malloc(8);
     //free(p);
    }
    gettimeofday(&end, NULL);
    long cost = 1000000 * (end.tv_sec - start.tv_sec) +
                  end.tv_usec - start.tv_usec;

    printf("%ld\n",cost);
  }
  return 0;
}
