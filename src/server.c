#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include<fcntl.h>
#include<dirent.h>
#include "sort.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


void *connection_handler(void *);
void useGivenFile(char file[100], int sockfd);
void benchmarkGivenFile(char file[100], int sockfd);
void sort(int arr[], int n, int sockfd);


typedef struct data
{
    char algorithmName[20];
    double time;
} data;

typedef struct CONTAINER
{
    int id;
    data data[4];
} CONTAINER;


int fileDesc;
int id = 1;


int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, "Port Number not provided");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error("Error opening socket");  
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    pthread_t thread_id;
    

    while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)){
    
        if(pthread_create( &thread_id , NULL ,  connection_handler , (void*) &newsockfd) < 0){
            error("could not create thread");
                    return 1;
        }

        

        int i = strncmp("Bye", buffer, 3);
        if(i == 0)
            break;  
    }

    if(newsockfd < 0) {
        error("Accept failed");
        return 1;   
    }

    close(newsockfd);
    close(sockfd);
    close(thread_id);
    return 0;

}

void *connection_handler(void *socket_desc)
{
    int sockfd = *(int *)socket_desc;
    int n;
    char buffer[2000];
    char actionLength[2];
    char action[10];
    char file[100];
    int actionLenghtNo;
    char *result;
    

    puts("Client connected.");

    
     
    while( (n = read(sockfd , buffer , 2000)) > 0 ) {

    strncpy(actionLength, buffer, 2);
    actionLenghtNo = atoi(actionLength);
    strncpy(action, buffer+1, actionLenghtNo);
    strcpy(file, buffer+actionLenghtNo+1);

    result = malloc(sizeof(char) * strlen(file));

    strncpy(result, file, strlen(file)-1);

    if(!strcmp(action, "USE")) 
        useGivenFile(result, sockfd);

    if(!strcmp(action, "SORT")) 
        benchmarkGivenFile(result, sockfd);


    bzero(buffer, 2000);

    strcpy(buffer, "ok");
        //write(sockfd , buffer , sizeof(buffer));
        
    bzero(buffer, 2000);
    bzero(actionLength, 2);
    bzero(action, 10);
    bzero(file, 100);
    }
     
    if(n == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(n == -1) {
        perror("recv failed");
    }

    return 0;
} 

void useGivenFile(char file[100], int sockfd) {

    // opening chosen file from the menu
    fileDesc = open(file, O_RDONLY, 0);
    char fail[] = "Failed to open and run file\n";
    char success[] = "Successfully opened the file\n";
    if(fileDesc == -1) {
        write(sockfd, fail, sizeof(fail));
        printf("failed to open and read file!\n");
        return ;
    } else {
        write(sockfd, success, sizeof(success));
        printf("sucessfully opened the file\n");    
    }
}

void benchmarkGivenFile(char file[100], int sockfd) {
    char fileBuffer[2048];
    int arr[1000];
    //read(fileDesc, fileBuffer, 2000);
    //write(sockfd, fileBuffer, sizeof(fileBuffer));

    // converting data from file and passing it to the array
    int i;
    for(i=0;i<1001;i++){
        arr[i]=fileBuffer[i]-'0';
    }
    // Doing the benchmark by calling sort()
    sort(arr, 2048, sockfd);

    printf("\n");
    bzero(fileBuffer, 2048);
    
    return ;
}

void sort(int arr[], int n, int sockfd)
{
 
    clock_t beginBubble = clock();
    bubbleSort(arr, n);
    clock_t endBubble = clock();
    double timeBubble = (double)(endBubble - beginBubble) / CLOCKS_PER_SEC;
    printf("Time for bubble: %f\n", timeBubble);
    
    clock_t beginShell = clock();
    shellSort(arr, n);
    clock_t endShell = clock();
    double timeShell = (double)(endShell - beginShell) / CLOCKS_PER_SEC;
    printf("Time for shell: %f\n", timeShell);


    clock_t beginSelection = clock();
    selectionSort(arr, n);
    clock_t endSelection = clock();
    double timeSelection = (double)(endSelection - beginSelection) / CLOCKS_PER_SEC;
    printf("Time for selection: %f\n", timeSelection);


    clock_t beginHeap = clock();
    heapSort(arr, n);
    clock_t endHeap = clock();
    double timeHeap = (double)(endHeap - beginHeap) / CLOCKS_PER_SEC;
    printf("Time for head: %f\n", timeHeap);

    char result[] = "Time for bubble: ";
    
    
    char b = timeBubble+'0';
    char c[] = "\nTime for shell: ";
    char s = timeShell+'0';
    char d[] = "\nTime for selection: ";
    char sel = timeSelection+'0';
    char e[] = "\nTime for heap: ";
    char h = timeHeap+'0';
    
    strcat(result, &b);
    strcat(result, c);
    strcat(result, &s);
    strcat(result, d);
    strcat(result, &sel);
    strcat(result, e);
    strcat(result, &h);
    
    write(sockfd, result, sizeof(result));

    // write individual results
    data bubble = {"Bubble Sort", timeBubble};
    data shell = {"Shell Sort", timeShell};
    data selection = {"Selection Sort", timeSelection};
    data heap = {"Heap Sort", timeHeap};

    data benchmark_d[4] = { bubble, shell, selection, heap };
    CONTAINER container = {id, benchmark_d };
    id++;
    printf("ID: %d, name: %s", id, benchmark_d[0].algorithmName);
    /*
FILE *out;

    fopen_s(&out, "../results/data.txt", "w");

    if (out == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }

    char buffer_in[256], buffer_out[256];


     data bubble = {"Bubble Sort", timeBubble};
     data shell = {"Shell Sort", timeShell};

     sprintf_s(buffer_out, 256, "%s,%f\n", bubble.fname, bubble.time);
     fwrite(buffer_out, sizeof(char), strlen(buffer_out), out);

     sprintf_s(buffer_out, 256, "%s,%f\n", shell.fname, shell.time);
     fwrite(buffer_out, sizeof(char), strlen(buffer_out), out);


    printf("\n");

     FILE *inf;
    struct data inp;
    inf = fopen ("../results/data.txt", "r");
    if (inf == NULL) {
       fprintf(stderr, "\nError to open the file\n");
       exit (1);
    }
    while(fread(&inp, sizeof(struct data), 1, inf))
       printf ("Sort = %s time = %f\n", inp.fname, inp.time);
    fclose (inf);


    */
}
