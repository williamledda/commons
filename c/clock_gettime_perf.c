#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
  struct timespec start;
  struct timespec sample;
  struct timespec end;
} data;

static inline uint64_t ts_2_nsec(const struct timespec* ts) {
  return ts->tv_sec * 1000000000 + ts->tv_nsec;
} 

int main(int argc, char** argv) {
  int i = 0;
  double sum = 0.0;
  struct timespec tmp = {0, 0};
  int iterations = atoi(argv[1]);
  
  data *buffer = (data*) malloc(iterations * sizeof(data));
  
  clock_gettime(CLOCK_MONOTONIC, &tmp);
  
  for(i = 0; i < iterations; i++) {
    clock_gettime(CLOCK_MONOTONIC, &buffer[i].start);
    clock_gettime(CLOCK_MONOTONIC, &buffer[i].sample);
    clock_gettime(CLOCK_MONOTONIC, &buffer[i].end);
  }
  
  for(i = 0; i < iterations; i++) {
    printf("%ld.%09ld %ld.%09ld %ld.%09ld %llu\n", 
      buffer[i].start.tv_sec, buffer[i].start.tv_nsec,
      buffer[i].sample.tv_sec, buffer[i].sample.tv_nsec, 
      buffer[i].end.tv_sec, buffer[i].end.tv_nsec, 
      ts_2_nsec(&buffer[i].end) - ts_2_nsec(&buffer[i].start)
    );
    sum += ts_2_nsec(&buffer[i].end) - ts_2_nsec(&buffer[i].start);
  }
  
  printf("%% Avg: %.6lf\n", sum/iterations);
  
  free(buffer);
}
