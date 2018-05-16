#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include <sys/time.h>
#include <dirent.h>
#include<string.h>
#define NUM_THREADS 4
pthread_mutex_t mutex;
long shared_var;

char * replace(
    char const * const original, 
    char const * const pattern, 
    char const * const replacement
);

void write_output(char *filename, int threads, double benchval,double th_val, double efficiency)
{
    char *op_file = replace(filename, ".dat",".out.dat");
    char op_path[100] = "/exports/home/kbharadwaj/cs553-pa1/cpu/output/";
  
    strcat(op_path,op_file);
    puts(op_path);
    FILE *fp = fopen (op_path , "w" );
    puts("writing op");
    fprintf(fp, "Concurrency\t  MyBenchValue\t TheoreticalValue\t MyBenchEfficiency\n");
    fprintf(fp,"%d \t\t %f \t\t %f \t\t %f \t\t \n",threads,benchval,th_val,efficiency);
    
}


char * replace(
    char const * const original, 
    char const * const pattern, 
    char const * const replacement
) {
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}



void *floatops(void *args)
{
  double x = 10.40;
  double y = 20.15;
  int num_threads = (int) args;
  long  i = 0;
  long  maxval = 100000000000/num_threads;
  printf("Iterations: %ld\n",maxval);
  while( i<maxval)
  {
    x+y;
    x-y;
    x*y;
    x/y;
    x+1.4;
    x-1.98;
    y+1.76;
    y-1.54;
    i++;
  }
  pthread_exit(NULL);
}

void *intops(void *args)
{
  int x = 10;
  int y = 20;
  int num_threads = (int) args;
  long  i = 0;
  long  maxval = 100000000000/num_threads;
  printf("Iterations: %ld\n",maxval);
  while( i<maxval)
  {
    x+y;
    x-y;
    x*y;
    x/y;
    x+1;
    x-1;
    y+1;
    y-1;
    i++;
  }
  pthread_exit(NULL);
}

void *halfprec(void *args)
{
  short int x = 10;
  short int y = 20;
  int num_threads = (int) args;
  long  i = 0;
  long  maxval = 100000000000/num_threads;
  printf("Iterations: %ld\n",maxval);
  while( i<maxval)
  {
    x+y;
    x-y;
    x*y;
    x/y;
    x+1;
    x-1;
    y+1;
    y-1;
    i++;
  }
  pthread_exit(NULL);
}

void *charops(void *args)
{
  char x = 'a';
  char y = 'b';
  int num_threads = (int) args;
  long  i = 0;
  long  maxval = 100000000000/num_threads;
  printf("Iterations: %ld",maxval);
  while( i<maxval)
  {
    x+y;
    x-y;
    x*y;
    x/y;
    x+1;
    x-1;
    y+1;
    y-1;
    i++;
  }
  pthread_exit(NULL);
}



int main(int argc, char **argv)
{
   char *filename;
  char path[200] = "/exports/home/kbharadwaj/cs553-pa1/cpu/";
  
  if(argc > 1)
  {
      filename = argv[1];
  }
  strcat(path,filename);
  struct timeval start, end;
   char * line = NULL;
   char prec[3]  ;
   char num_threads[2];
   char thread[10]  ;
   char blocksize[20] ;
    size_t len = 0;
    ssize_t read;
     FILE *file = fopen (path , "r" );
     int i = 1;


           while ((read = getline(&line, &len, file)) != -1) 
           {
                if(read == 2)
                {
                    strcpy(num_threads,line);
                }
                if(read == 3)
                {
                    strcpy(prec,line);
                }
           }
           
        
        int no_threads = atoi(num_threads);
        pthread_t threads[no_threads]; long tid; int rc;
               printf("precision: %s\n",strtok(prec,"\n"));
               
              printf("Threads: %d\n",no_threads);
    gettimeofday(&start, NULL);          
 for (tid = 0; tid < no_threads; tid++) {
    if(strcmp(prec,"SP") == 0 )
    {
       
        rc = pthread_create(&threads[tid], NULL, intops, (void *) no_threads);
       
    }   
   else if(strcmp(prec,"QP") == 0)
    {
       
        rc = pthread_create(&threads[tid], NULL, charops, (void *) no_threads);
        
    }
    else if(strcmp(prec,"HP") == 0)
    {
          
        rc = pthread_create(&threads[tid], NULL, halfprec, (void *) no_threads);
         
    }
    else if(strcmp(prec,"DP") == 0)
    {
        
        rc = pthread_create(&threads[tid], NULL, floatops, (void *) no_threads);
      
    }
if (rc) {
printf("Could not create thread %ld\n", tid);
}
}
for (tid = 0; tid < no_threads; tid++) {
rc = pthread_join(threads[tid], NULL);
if (rc) {
printf("Could not join thread %ld\n", tid);
}
}
        fclose(file);
          
           
      
      gettimeofday(&end, NULL);
printf("elapsed: %fs\n",
(float) (end.tv_usec - start.tv_usec) / 1000000 +
(float) (end.tv_sec - start.tv_sec));

float cpu_theo = 2 * 2.3;
double benchval;
float tot_time = (float) (end.tv_usec - start.tv_usec) / 1000000 +
(float) (end.tv_sec - start.tv_sec);
double perc_eff;
if(strcmp(prec,"QP") == 0)
{
    cpu_theo = cpu_theo * 64;
    benchval = 1000.00/tot_time;
    perc_eff = (benchval/cpu_theo)*100;
}
if(strcmp(prec,"HP") == 0)
{
    cpu_theo = cpu_theo * 32;
    benchval = 1000.00/tot_time;
    perc_eff = (benchval/cpu_theo)*100;
}
if(strcmp(prec,"SP") == 0)
{
    cpu_theo = cpu_theo * 16;
    benchval = 1000.00/tot_time;
    perc_eff = (benchval/cpu_theo)*100;
}
if(strcmp(prec,"DP") == 0)
{
    cpu_theo = cpu_theo * 8;
    benchval = 1000.00/tot_time;
    perc_eff = (benchval/cpu_theo)*100;
}

write_output(argv[1],no_threads,benchval,cpu_theo,perc_eff);
printf("%s \n","End");

} 
