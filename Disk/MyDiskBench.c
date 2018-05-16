#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



typedef struct data{
int   threads; // no. of threads
int  block_size; // Block size

} t_data;

pthread_mutex_t mutex;

int writefd;
int readfd;

char *randstring(size_t length);


char *write_buf;
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

void write_output(char *filename, int threads, int blocksize, double benchval,double th_val, double efficiency,char* pattern)
{
    char *op_file = replace(filename, ".dat",".out.dat");
    char op_path[100] = "/exports/home/kbharadwaj/cs553-pa1/disk/output/";
  
    strcat(op_path,op_file);
    puts(op_path);
    FILE *fp = fopen (op_path , "w" );
    fprintf(fp, "Workload\t Concurrency\t BlockSize\t MyDISKBenchValue\t TheoreticalValue\t MyDISKBenchEfficiency\n");
    fprintf(fp,"%s\t\t %d \t\t\t %d \t\t %f \t\t %f \t\t %f \n",pattern,threads,blocksize,benchval,th_val,efficiency);
    
}


void *write_sq(void *args)
{
printf("entered sequential read\n");

t_data *data;
data = (t_data*)args;

int blocksize = data->block_size * 1000;
long iter;// = 10000000000/(data->threads * blocksize);
 puts("accessed variables");
int i;  
int n;
//char *write_buf = randstring(blocksize);
char path[100] = "/tmp/test_file.dat";
writefd = open(path,O_RDWR|O_APPEND|O_CREAT,S_IRWXU);
puts("file opened");
if(blocksize != 1000)
{
    iter = 10000000000/(data->threads * blocksize);
for(i=0;i<iter;++i)
{
 
 n = write(writefd,write_buf,blocksize);
 //printf("%d\n",n);
}
}
else    //latency
{
    iter = 1000000/data->threads ;
    for(i=0;i<iter;++i)
    {
        n = write(writefd,write_buf,blocksize);
    }
}
close(writefd);
}

void *write_rand(void *args)
{
printf("entered random write\n");

t_data *data;
data = (t_data*)args;

int blocksize = data->block_size * 1000;
long iter = 10000000000/(data->threads * blocksize);
 puts("accessed variables");
int i;  
int n;
//char *write_buf = randstring(blocksize);
int w = 10000000000 - (2*blocksize);
char path[100] = "/tmp/test_file.dat";
writefd = open(path,O_RDWR|O_APPEND|O_CREAT,S_IRWXU);
puts("file opened");
if(blocksize != 1000)
{
for(i=0;i<iter;++i)
{
 
 n = pwrite(writefd,write_buf,blocksize,(int)rand()%w);
 //printf("%d\n",n);
}
}
else    //latency
{
    iter = 1000000/data->threads ;
    for(i=0;i<iter;++i)
    {
        n = pwrite(writefd,write_buf,blocksize,(int)rand()%w);
    }
}
close(writefd);
}



void *read_sq(void *args)
{
printf("entered sequential read\n");

t_data *data;
data = (t_data*)args;

int blocksize = data->block_size * 1000;
long iter = 10000000000/(data->threads * blocksize);
 puts("accessed variables");
int i;  
int n;
char *read_buf = malloc(blocksize*sizeof(char));

//char path[100] = "test_file.dat";
char path[100] = "/tmp/test_file.dat";
readfd = open(path,O_RDONLY);
puts("file opened");
if(blocksize != 1000)
{
for(i=0;i<iter;++i)  
{
 //lseek(readfd,blocksize*i,SEEK_CUR);
 n = read(readfd,read_buf,blocksize);
 //printf("%d\n",n);
}
}
else   //latency
{
    iter = 1000000/data->threads ;
    for(i=0;i<iter;++i)
    {
        //lseek(readfd,blocksize,SEEK_CUR);
        n = read(readfd,read_buf,blocksize);
    }
}
close(readfd);

}

void *read_rand(void *args)
{
printf("entered random read\n");

t_data *data;
data = (t_data*)args;

int blocksize = data->block_size * 1000;

 puts("accessed variables");
int i;  
int n;
long r = 10000000000 - (2*blocksize);
char *read_buf = malloc(blocksize*sizeof(char));

char path[100] = "/tmp/test_file.dat";
readfd = open(path,O_RDONLY);

FILE *fp = fopen(path,"r");


if(blocksize != 1000)
{
    long iter = 10000000000/(data->threads * blocksize);
    int pos;
for(i=0;i<iter;++i)
{
 
  //n = lseek(readfd, (rand()%(int)iter)*blocksize,SEEK_SET);
 pos = rand()%iter;
 fseek(fp, pos, SEEK_CUR);
fread(read_buf,blocksize,1,fp); 
//puts(read_buf);
}
}
else    //latency
{
   long iter = 1000000/data->threads ;
    for(i=0;i<iter;++i)
    {
        // n = lseek(readfd,((int)rand()%iter*blocksize),SEEK_SET);
         n= pread(readfd,read_buf,blocksize,((int)rand()%iter*blocksize)); 
    }
}
close(readfd);
}


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

void *create_file(void *args)
{
char *content = randstring(10000000);
FILE *fp = fopen("/tmp/test_file.dat","a+");
int i;
for(i=0;i<100;++i)
{
    fprintf(fp,"%s",content);
}
puts("done my job");
}

int main(int argc, char **argv)
{
remove("/tmp/test_file.dat");

/*create file*/
pthread_t fthreads[10]; 

long fid; 

int rc;
char *filename;
char path[200] = "/exports/home/kbharadwaj/cs553-pa1/disk/";
if(argc > 1)
{
filename = argv[1];
}
remove("/tmp/test_file.dat");
strcat(path,filename);
puts(path);
char * line = NULL;
char pattern[10]  ;
char thrd[10]  ;
char b_s[20] ;
size_t len = 0;
ssize_t read;
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
           strcpy(b_s,line);
      }
      if(i == 3)
      {
           strcpy(thrd,line);
      }
      ++i;
   }
   strtok(pattern,"\n");
  if(strcmp(pattern,"RS") ==0 || strcmp(pattern,"RR") == 0)
{
    
  for (fid = 0; fid < 10; fid++) {
rc = pthread_create(&fthreads[fid], NULL, create_file, NULL);

if (rc) {
printf("Could not create thread %ld\n", fid);
}
}
for (fid = 0; fid < 10; fid++) {
rc = pthread_join(fthreads[fid], NULL);
if (rc) {
printf("Could not join thread %ld\n", fid);
}
} 
}  
t_data data;
data.block_size = atoi(b_s);
data.threads = atoi(thrd);
long sz = data.block_size * 1000;
//read_buf = randstring(data.block_size);
write_buf = randstring(sz);
printf("%d\n",strlen(write_buf));
printf("Executing Disk Benchmark\n");

pthread_t threads[data.threads]; 
//printf("thread");
long tid; 
//printf("tid");

//printf("rc");
struct timeval start, end;
gettimeofday(&start, NULL); 

//printf("gettime"); 
for (tid = 0; tid < data.threads; tid++) {
if(strcmp(pattern,"RR") == 0)
 {
 pthread_create(&threads[tid], NULL, read_rand, (void*) &data);
 }
 else if(strcmp(pattern,"RS") == 0)
 {
     pthread_create(&threads[tid], NULL, read_sq, (void*) &data);
 }
 else if(strcmp(pattern,"WS") == 0)
 {
     pthread_create(&threads[tid], NULL, write_sq, (void*) &data);
 }
  else if(strcmp(pattern,"WR") == 0)
 {
     pthread_create(&threads[tid], NULL, write_rand, (void*) &data);
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
gettimeofday(&end, NULL);
printf("%s\n","DISK BENCHMARK");
printf("Workload: 10GB , %s", strtok(pattern,"\n"));
printf ("\t Concurrency: %s \t", strtok(thrd,"\n"));
printf("Time Taken  : %fs\n",
(float) (end.tv_usec - start.tv_usec) / 1000000 +
(float) (end.tv_sec -
start.tv_sec));

double tot_time = (float) (end.tv_usec - start.tv_usec) / 1000000 + (float) (end.tv_sec - start.tv_sec);
double benchval;
double th_val_th_seqR = 540;
double th_val_th_wriS = 410;
double th_val_th_wriR = 172;
double th_val_th_ranR = 372;
double th_val_lat = 0.5;
double perc_eff ;
if(data.block_size != 1)
{

 benchval = (10.00/tot_time)*1000;
 if(strcmp(pattern,"RS") == 0 )
 {
 perc_eff = (benchval/(th_val_th_seqR * data.threads))*100;
  write_output(argv[1],data.threads, data.block_size,benchval,th_val_th_seqR * data.threads,perc_eff,pattern);
 }
if(strcmp(pattern,"WS") == 0 )
 {
 perc_eff = (benchval/(th_val_th_wriS * data.threads))*100;
  write_output(argv[1],data.threads, data.block_size,benchval,th_val_th_wriS * data.threads,perc_eff,pattern);
 }
 if(strcmp(pattern,"RR") == 0 )
 {
 perc_eff = (benchval/(th_val_th_ranR * data.threads))*100;
  write_output(argv[1],data.threads, data.block_size,benchval,th_val_th_ranR * data.threads,perc_eff,pattern);
 }
 if(strcmp(pattern,"WR") == 0 )
 {
 perc_eff = (benchval/(th_val_th_wriR * data.threads))*100;
  write_output(argv[1],data.threads, data.block_size,benchval,th_val_th_wriR * data.threads,perc_eff,pattern);
 }
 
}
else
{
    benchval = tot_time/100.00;
    perc_eff = (1- (th_val_lat/benchval))*100;
    write_output(argv[1],data.threads, data.block_size,benchval,th_val_lat,perc_eff,pattern);
}

remove("/tmp/test_file.dat");
return 0;
} 