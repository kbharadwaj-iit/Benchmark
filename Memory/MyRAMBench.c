#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include <sys/time.h>



typedef struct data{
   int  threads;
   int  block_size;

} t_data;
char *str;
char *randstring(size_t length) {

static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
char *randomString = NULL;

if (length) {
    randomString = malloc(sizeof(char) * (length +1));

    if (randomString) {            
        for (int n = 0;n < length;n++) {            
            int key = rand() % (int)(sizeof(charset) -1);
            randomString[n] = charset[key];
        }

        randomString[length] = '\0';
    }
}

return randomString;
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


void write_output(char *filename, int threads, int blocksize, double benchval,double th_val, double efficiency)
{
    char *op_file = replace(filename, ".dat",".out.dat");
    char op_path[100] = "/exports/home/kbharadwaj/cs553-pa1/memory/output/";
  
    strcat(op_path,op_file);
    puts(op_path);
    FILE *fp = fopen (op_path , "w" );
    fprintf(fp, "Workload\t Concurrency\t BlockSize\t MyRAMBenchValue\t TheoreticalValue\t MyRAMBenchEfficiency\n");
    fprintf(fp,"10GB\t\t %d \t\t\t %d \t\t %f \t\t %f \t\t %f \n",threads,blocksize,benchval,th_val,efficiency);
    
}

void *random_rw(void *args)
{
    printf("%s\n","random");
    long memory = 1000 * 1000 * 1000; //allocate 100 mb for aws
    t_data *data;
    data = (t_data*) args;
    printf("threads: %d\n",data->threads);
    int block_size = data->block_size;
    int blocks = memory / block_size;
    int blcks_per_thrd = blocks / data->threads;
    int i;
    long iter = 100000000/data->threads;
    int loc;
      int j = 0;
    if(block_size != 1)
    {
        char *src= (char*)malloc(memory*sizeof(char)/data->threads);
	char *dst= (char*)malloc(memory*sizeof(char)/data->threads);
    strcpy(src,str);
    while(j<100)
    {
  for(i = 0; i < blcks_per_thrd; ++i)
    {
        loc = rand()%blcks_per_thrd;
        memcpy(dst + (loc*block_size), src + (loc*block_size),block_size);
     //   printf("%s\n",dst);
    }
    
    ++j;
    }
    free(src);
    free(dst);
    }
    else
    {
         char *src= (char*)malloc(iter);
        char *dst= (char*)malloc(iter);
      
        for(i=0;i<iter;++i)
        {
            loc = rand()%iter;
            memcpy(dst+loc,src+loc,block_size);
        }
        free(src);
        free(dst);   
    }   
    pthread_exit(NULL);
}

void *seq_rw(void *args)
{
  printf("%s\n","sequential");
     
    t_data *data;
    data = (t_data*) args;
    printf("threads: %d\n",data->threads);
    long memory = 1000 * 1000 * 1000/data->threads;
    int block_size = data->block_size;
    int blocks = memory / block_size;
    int blcks_per_thrd = blocks / data->threads;
    int i;
    int iter = 100000000/data->threads;
    printf("blocks per thread: %d\n",blcks_per_thrd);
    int j = 0;
     char *dst= (char*)malloc(memory*sizeof(char));
        char *src= (char*)malloc(memory*sizeof(char));
        strcpy(src,str);
    if(block_size != 1)
    {
      
    while(j<100)
    {
        
       
    for(i = 0; i < blcks_per_thrd; i++)
    {
        memcpy(dst + (i*block_size), src + (i*block_size),block_size);
     //   printf("%s\n",dst);
    }
      
    ++j;
    }
      free(src);
        free(dst);
    }
    else
    {
        char *src= (char*)malloc(iter);
        char *dst= (char*)malloc(iter);
      
        for(i=0;i<iter;++i)
        {
            memcpy(dst+i,src+i,block_size);
        }
        free(src);
        free(dst);
       
    }
   
    
    pthread_exit(NULL);
}



int main(int argc, char **argv)
{
    char *filename;
  char path[200] = "/exports/home/kbharadwaj/cs553-pa1/memory/";
  
  if(argc > 1)
  {
      filename = argv[1]; 
  }
  strcat(path,filename);
  
   char * line = NULL;
   char pattern[10]  ;
   char thread[10]  ;
   char blocksize[20] ;
    size_t len = 0;
    ssize_t read;
    puts(path);
     FILE *file = fopen (path , "r" );
     int i = 1;
   
      while ((read = getline(&line, &len, file)) != -1) 
           {
               
              if(i == 1)
              {
                  
                   strcpy(pattern,line);
              }
              if(i == 2)
              {
                   strcpy(blocksize,line);
              }
              if(i == 3)
              {
                   strcpy(thread,line);
              }
              ++i;
           }
          
  t_data data;
  data.block_size = atoi(blocksize);
  data.threads = atoi(thread);
  printf("%s\n",path);
  strtok(pattern,"\n");
printf("%s \n","Starting Execution");
str = randstring(1000000000/data.threads);
pthread_t threads[data.threads]; long tid; int rc;
//pthread_mutex_init(&mutex, NULL); 
//int num_threads = NUM_THREADS;
struct timeval start, end;
gettimeofday(&start, NULL);  
for (tid = 0; tid < data.threads; tid++) {
 if(strcmp(pattern,"RWR") == 0)
 {
 pthread_create(&threads[tid], NULL, random_rw, (void*) &data);
 }
 else if(strcmp(pattern,"RWS") == 0)
 {
     pthread_create(&threads[tid], NULL, seq_rw, (void*) &data);
 }
if (rc) {
printf("Could not create thread %ld\n", tid);
}
}
for (tid = 0; tid < data.threads; tid++) {
rc = pthread_join(threads[tid], NULL);
if (rc) {
printf("Could not join thread %ld\n", tid);
}
}
//pthread_mutex_destroy(&mutex);

      gettimeofday(&end, NULL);
printf("elapsed: %fs\n",
(float) (end.tv_usec - start.tv_usec) / 1000000 +
(float) (end.tv_sec - start.tv_sec));

double tot_time = (float) (end.tv_usec - start.tv_usec) / 1000000 + (float) (end.tv_sec - start.tv_sec);
double benchval;
double th_val_th = 68.256;
double th_val_lat = 0.01406;
double perc_eff ;
if(data.block_size != 1)
{

 benchval = 100.00/tot_time;
 perc_eff = (benchval/th_val_th)*100;
 write_output(argv[1],data.threads, data.block_size,benchval,th_val_th,perc_eff);
}
else
{
    benchval = tot_time/100.00;
    perc_eff = (1- (th_val_lat/benchval))*100;
    write_output(argv[1],data.threads, data.block_size,benchval,th_val_lat,perc_eff);
}
   printf("throughput: %f\n",benchval); 


} 
