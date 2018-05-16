/*
    C socket server example, handles multiple clients using threads
*/
#include <sys/time.h>
#include <netinet/in.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#define BUFF_SIZE 64
#include<netdb.h>
#define ERROR -1
#include <string.h>
#include <time.h>
pthread_mutex_t mutex;
char hostname[30];
int stop_transmit = 0;
/* typedef struct server_mem_data{
    char *srvmem;
    int *sock;
}t_mem; */
long totsize = 100000000000;
int cons = 0;
typedef struct data{
   
   int  threads;
   int  block_size;

} t_data;

char *content;


 
long srv_itr = 0;

char * replace(
    char const * const original, 
    char const * const pattern, 
    char const * const replacement
);

void write_output(char *filename, int threads, int blocksize, double benchval,double th_val, double efficiency)
{
    char *op_file = replace(filename, ".dat",".out.dat");
    char op_path[100] = "/exports/home/kbharadwaj/cs553-pa1/network/output/";
  
    strcat(op_path,op_file);
    puts(op_path);
    FILE *fp = fopen (op_path , "w" );
    puts("writing op");
    fprintf(fp, "Workload\t Concurrency\t BlockSize\t MyBenchValue\t TheoreticalValue\t MyNETBenchEfficiency\n");
    fprintf(fp,"10GB\t\t %d \t\t\t %d \t\t %f \t\t %f \t\t %f \n",threads,blocksize,benchval,th_val,efficiency);
    
}

void *init_conn(void *args)
{
    
    struct hostent *he;
   t_data *data;
   struct in_addr **addr_list;
   data = (t_data*) args;
   
    long i = 0;
    int sock;
    struct sockaddr_in server;
    char message[32000] , server_reply[2000];
    char ip[40];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    
     he = gethostbyname(hostname);
    // puts("init");
    //puts(hostname);
     addr_list = (struct in_addr **) he->h_addr_list;
     
     for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
      //  puts(ip);        
    } 
     
    server.sin_addr.s_addr = inet_addr(ip);
   // puts("socket");
    server.sin_family = AF_INET;
   
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        
    }
     
  //  puts("Connected\n");
     
    //keep communicating with server
  
       
         
        //Send some data
       
        if( send(sock , "11" , 2 , 0) < 0)
        {
            puts("Send failed");
             pthread_exit(NULL);
        }

         puts("initialized");
        //Receive a reply from the server
        if( recv(sock , server_reply , 1 , 0) < 0)
        {
            puts("recv failed");
           
        }
                  
        stop_transmit = 1;
       puts("stopping");
        printf("%d\n",stop_transmit);
     
    close(sock);
   puts("socket closed");
 pthread_exit(NULL);
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


//the thread function
void *connection_handler(void *);

 
int server()
{
   
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
  
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
  /*   puts("Socket created");
     puts("allo"); */
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
 //   puts("Socket ");
    server.sin_addr.s_addr = INADDR_ANY;
   // puts(" created");
    server.sin_port = htons( 8888 );
     puts("Socket created");
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
  
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
      
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         

        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *args)
{
   
    int sock = *(int*)args;
    int read_size;
    char *message , client_message[32000];
     
 
    while( (read_size = recv(sock , client_message , 32000 , 0)) > 0 )
    {   
       if (strlen(client_message) != 1 && strlen(client_message) != 2)
       {
       pthread_mutex_lock(&mutex);
        totsize = totsize - strlen(client_message);
       pthread_mutex_unlock(&mutex);
       if(totsize <=0)
        {
            puts("recd 100 g");
            break;     
        }
       }
       else if(strlen(client_message) == 2)
       {
           while(totsize >0)
           {
               //do nothing
           }
           write(sock , "r" , 1);
             pthread_mutex_lock(&mutex);
            totsize = 100000000000;
           pthread_mutex_unlock(&mutex);
       }
       else if(strlen(client_message) == 1) //ping pong
       {
           send(sock,"P",1,0);
       }
        
       
    }
    
    //write(sock , "r" , 1);
    
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(args);
     pthread_exit(NULL);
    return 0;
}

 
 
void *client(void *args)
{
   // puts(content);
   long iter;
    struct hostent *he;
   t_data *data;
   struct in_addr **addr_list;
   data = (t_data*) args;
   iter = (long)(100000000000/(data->threads*data->block_size));
    long i = 0;
    int sock;
    struct sockaddr_in server;
    char message[32000] , server_reply[2000];
    char ip[40];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    
     he = gethostbyname(hostname);
    
     addr_list = (struct in_addr **) he->h_addr_list;
     
     for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        puts(ip);        
    } 
     
    server.sin_addr.s_addr = inet_addr(ip);
   // puts("socket");
    server.sin_family = AF_INET;
   
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        
    }
     
    puts("Connected\n");
     
    //keep communicating with server
  
       
         
        //Send some data
        while(stop_transmit == 0)
        {
        if( send(sock , content , strlen(content) , 0) < 0)
        {
            puts("Send failed");
          //  return 1;
        }
        
        }
        puts("finish");
        //Receive a reply from the server
close(sock);
   puts("socket closed");
   pthread_exit(NULL);
}

void *lat_client(void *args)
{
    long iter;
    struct hostent *he;
   t_data *data;
   struct in_addr **addr_list;
   data = (t_data*) args;
   iter = (long)(1000000/(data->threads));
    long i = 0;
    int sock;
    struct sockaddr_in server;
    char message[1] , server_reply[1];
    char ip[40];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    puts("host: ");
    puts(hostname);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    
     he = gethostbyname(strtok(hostname,"\n"));
    
     addr_list = (struct in_addr **) he->h_addr_list;
     puts("hgg");
     for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        puts(ip);        
    } 
     
    server.sin_addr.s_addr = inet_addr(ip);
   // puts("socket");
    server.sin_family = AF_INET;
   
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        
    }
     
    puts("Connected\n");
     
    //keep communicating with server
  
       
         
        //Send some data
        while(i < iter)
        {
            int n;
        if(n= send(sock , "S" , 1 , 0) < 0)
        {
            puts("Send failed");
         //   printf("%d\n");
            break;
            
          //  return 1;
        }
        
        if(recv(sock ,server_reply  , 1 , 0) < 0)
        {
            puts("failed");
        }
        ++i;
       // printf("%d\n",i);
        }
        
        //Receive a reply from the server
close(sock);
   puts("socket closed");
   pthread_exit(NULL);
}

int main(int argc , char *argv[])
{

    int res;
    pthread_mutex_init(&mutex, NULL);
     char hostnamepath[200] = "/exports/home/kbharadwaj/cs553-pa1/network/";
    if(strcmp(argv[1],"server") ==0)
    {
        pthread_mutex_init(&mutex, NULL);
      //  srv= (char*)malloc(1000000000*sizeof(char));
        res = server();
    }
    else//client logic
    {
     char *filename;
  char path[200] = "/exports/home/kbharadwaj/cs553-pa1/network/";
  
  if(argc > 2)
  {
      filename = argv[3]; 
      
      
  }
  strcat(hostnamepath,argv[2]);
  strcat(path,filename);    
  puts(hostnamepath);
       FILE *file = fopen (hostnamepath , "r" );
   
   

   char * line = NULL;
   

   char pattern[10]  ;
   char thread[10]  ;
   char blocksize[20] ;
    size_t len = 0;
    ssize_t read;

      while ((read = getline(&line, &len, file)) != -1) 
    {
        strcpy(hostname,line);
    }

/* r = pthread_join(initthrd[0], NULL);
if (r) {
printf("Could not join thread");
} */

     
    /*end initial connect*/
      file = fopen (path , "r" );
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
      puts(pattern);
        puts(blocksize);
        puts(thread);
  t_data data;
  data.block_size = atoi(blocksize);
  data.threads = atoi(thread);
   pthread_t initthrd[1];  int r;
   // int thread = 2;
   if(data.block_size != 1)
   {
     
    //puts(hostname);
    strtok(hostname,"\n");
    
    /*create initialize thread*/
    
     
     r = pthread_create(&initthrd[0], NULL, init_conn, NULL);
     

if (r) {
printf("Could not create thread\n");
}
   }
   
  
   
    content = randstring(data.block_size);
     puts("hee");
pthread_t threads[data.threads]; long tid; int rc;
//pthread_mutex_init(&mutex, NULL); 
//int num_threads = NUM_THREADS;
struct timeval start, end;
gettimeofday(&start, NULL);  
for (tid = 0; tid < data.threads; tid++) {
 if(data.block_size != 1)
 {
 rc = pthread_create(&threads[tid], NULL, client, &data);
 }
 else
 {
     puts("dude");
     rc = pthread_create(&threads[tid], NULL, lat_client, &data);
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


double tot_time = (float) (end.tv_usec - start.tv_usec) / 1000000 + (float) (end.tv_sec - start.tv_sec);

printf("time: %fs",tot_time);
double benchval;
double th_val_th = 56000.00;
double th_val_lat = 0.7;
double perc_eff ;
if(data.block_size != 1)
{

 benchval = (100.00/tot_time)*8*1000;
 perc_eff = (benchval/th_val_th)*100;
 write_output(argv[3],data.threads, data.block_size,benchval,th_val_th,perc_eff);
}
else
{
    benchval = tot_time/1000.00;
    perc_eff = (1- (th_val_lat/benchval))*100;
    write_output(argv[3],data.threads, data.block_size,benchval,th_val_lat,perc_eff);
}




/* r = pthread_join(initthrd[0], NULL); */
    pthread_mutex_destroy(&mutex);
//write_output(argv[1],thread, 1024,tot_time,0.0,0.0);
    }
}
 

