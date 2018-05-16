

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<pthread.h>
#include <sys/time.h>
#define BUFSIZE 32000

long totsize = 100000000000;
char hostname[30];
int stop_transmit = 0;
pthread_mutex_t mutex;
/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

typedef struct data{
   
   int  threads;
   int  block_size;

} t_data;

char * replace(
    char const * const original, 
    char const * const pattern, 
    char const * const replacement
) ;

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

char *content;

pthread_mutex_t mutex;

void *client(void *args)
{
    
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
   // char *hostname;
    
     long iter;
   t_data *data;
   data = (t_data*) args;
   iter = (long)(1000000/(data->threads*data->block_size));
    long i = 0;
    char buf[data->block_size];
    /* check command line arguments */
    
  //  hostname = "localhost";
    portno = 9991;
    
    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(strtok(hostname,"\n"));
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, data->block_size);
  //  printf("Please enter msg: ");
   // fgets(buf, BUFSIZE, stdin);
      //  puts(content);
    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    if(data -> block_size != 1)
    {
    while(stop_transmit == 0)
    {
        
        n = sendto(sockfd, content, strlen(content), 0,  (struct sockaddr *)&serveraddr, serverlen);
if (n < 0) 
{error("ERROR in sendto");}

    }
    }
    else
    {
        while(i < iter)
        {
            n = sendto(sockfd, "D", 1, 0,  (struct sockaddr *)&serveraddr, serverlen);
            if (n < 0) 
            {
                error("ERROR in sendto");
            }
           n = recvfrom(sockfd, buf, 1, 0,
		 (struct sockaddr *)&serveraddr, &serverlen); 
            ++i;
        }
    }
  
   // printf("Echo from server: %s", buf);
    
    
    return 0;
}

void *tcp_server(void *args)
{
      int socket_desc , client_sock , c , *new_sock,rc;
    struct sockaddr_in server , client;
    char client_rc[10];
     
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
       pthread_exit(NULL);
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
  
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ;
    while(1)
    {
    rc = recv(client_sock, client_rc, 10, 0);
    puts("recd");
    while(totsize > 0)
    {
      // printf("%ld\n",totsize); //do nothing
    }
    printf("totsize1: %ld\n",totsize);
    pthread_mutex_lock(&mutex);
    totsize = 100000000000;
        pthread_mutex_unlock(&mutex);
        printf("totsize2: %ld\n",totsize);
    rc = send(client_sock,"d",1,0);
    }
}

int server() {
  int sockfd; 
  int portno; 
  int clientlen; 
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;
  struct hostent *hostp; 
  char buf[32000]; 
  char *hostaddrp; 
  int optval;  
  ssize_t n; 

  //long totsize = 10000000;
  portno = 9991;

  /*initiate a tcp server to keep track of total size*/
   pthread_t initthrd[1];  int r;
   r = pthread_create(&initthrd[0], NULL, tcp_server, NULL);
  /*end of tcp logic*/
  
  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);


  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

   
  //  printf("%ld\n",totsize);
  clientlen = sizeof(clientaddr);
  while (1) {
        bzero(buf, 32000);
        n = recvfrom(sockfd, buf, 32000, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0){
    error("ERROR in recvfrom");}
    if(strlen(buf) != 1 && n >0)
    {
        pthread_mutex_lock(&mutex);
        totsize = totsize - strlen(buf);
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        n = sendto(sockfd, "D", 1, 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    }
       // printf("%ld\n",totsize);
     }
  return 0;
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
    puts("here");
     he = gethostbyname(strtok(hostname,"\n"));
     puts("init");
    //puts(hostname);
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
       puts("sending ack");
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
   //     printf("%d\n",stop_transmit);
     
    close(sock);
   puts("socket closed");
 //pthread_exit(NULL);
}


int main(int argc , char *argv[])
{

    int res;
    pthread_mutex_init(&mutex,NULL);
    if(strcmp(argv[1],"server") ==0)
    {
       // pthread_mutex_init(&mutex, NULL);
     //   srv= (char*)malloc(1000000000*sizeof(char));
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
  strcat(path,filename);
  puts(path);
   char * line = NULL;
   char pattern[10]  ;
   char thread[10]  ;
   char blocksize[20] ;
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
   pthread_t initthrd[1];  int r;
   char hostnamepath[200] = "/exports/home/kbharadwaj/cs553-pa1/network/";
   strcat(hostnamepath,argv[2]);
   file = fopen(hostnamepath,"r");
    while ((read = getline(&line, &len, file)) != -1) 
    {
        strcpy(hostname,line);
    }           
  if(data.block_size != 1)  //throughput
  {
      puts(hostname);
   content = randstring(data.block_size);
   /*initialize tcp connection*/
   strtok(hostname,"\n");
    r = pthread_create(&initthrd[0], NULL, init_conn, NULL);
    
  }
   // int thread = 2;
  
pthread_t threads[data.threads]; long tid; int rc;
//pthread_mutex_init(&mutex, NULL); 
//int num_threads = NUM_THREADS;
struct timeval start, end;
gettimeofday(&start, NULL);  
for (tid = 0; tid < data.threads; tid++) {

 rc = pthread_create(&threads[tid], NULL, client, &data);

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


printf("time: %fs",tot_time);
double benchval;
double th_val_th = 56000.00;
double th_val_lat = 0.7;
double perc_eff ;
if(data.block_size != 1)
{

 benchval = (100.00/tot_time)*1000*8;
 perc_eff = (benchval/th_val_th)*100;
 write_output(argv[3],data.threads, data.block_size,benchval,th_val_th,perc_eff);
}
else
{
    benchval = tot_time/1000.00;
    perc_eff = (1- (th_val_lat/benchval))*100;
    write_output(argv[3],data.threads, data.block_size,benchval,th_val_lat,perc_eff);
}


printf("time: %fs",tot_time);
    pthread_mutex_destroy(&mutex);
//write_output(argv[1],thread, 1024,tot_time,0.0,0.0);
    }
}