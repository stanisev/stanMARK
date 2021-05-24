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

int * intdup(int const * src, size_t len)
{
   int * p = malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
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
 
    // making dups of the array
    int *shellArray = intdup(arr, n);
    int *selectionArray = intdup(arr, n);
    int *heapArray = intdup(arr, n);

    clock_t beginBubble = clock();
    bubbleSort(arr, n);
    clock_t endBubble = clock();
    double timeBubble = (double)(endBubble - beginBubble) / CLOCKS_PER_SEC;
    //printf("Time for bubble: %f\n", timeBubble);
    
    clock_t beginShell = clock();
    shellSort(shellArray, n);
    clock_t endShell = clock();
    double timeShell = (double)(endShell - beginShell) / CLOCKS_PER_SEC;
    //printf("Time for shell: %f\n", timeShell);


    clock_t beginSelection = clock();
    selectionSort(selectionArray, n);
    clock_t endSelection = clock();
    double timeSelection = (double)(endSelection - beginSelection) / CLOCKS_PER_SEC;
    //printf("Time for selection: %f\n", timeSelection);


    clock_t beginHeap = clock();
    heapSort(heapArray, n);
    clock_t endHeap = clock();
    double timeHeap = (double)(endHeap - beginHeap) / CLOCKS_PER_SEC;
    //printf("Time for head: %f\n", timeHeap);

    char result[] = "Time for bubble: ";
    

    // write individual results

    data bubble = {"Bubble Sort", timeBubble};
    data shell = {"Shell Sort", timeShell};
    data selection = {"Selection Sort", timeSelection};
    data heap = {"Heap Sort", timeHeap};

    float time[4] = {timeBubble, timeShell, timeSelection, timeHeap};
    float smallest = time[0];
    float biggest = time[0];
    int index = 0, slow=0;
    char buf[64] = "\nTime: ";

    char res1[] = "Best time with Bubble sort: ";
    char res2[] = "Best time with Shell sort: ";
    char res3[] = "Best time with Sellection sort: ";
    char res4[] = "Best time with Heap sort: ";

    for (int i = 0; i < 4; i++) {
      // find the quickest sort
      if (time[i] < smallest) {
         smallest = time[i];
         index = i;
      }
      // find the slowest sort
      if(time[i]>biggest){
         biggest = time[i];
         slow = i;
      }
   }
   switch(index) {
    case 0:
        
        printf("Best time with Bubble sort with %f s\n", smallest);

        snprintf(buf, sizeof buf, "%f", smallest);
        strcat(res1, buf);
        write(sockfd, res1, 64);
        break;
     case 1:
        printf("Best time with Shell sort with %f s\n", smallest);
        
        snprintf(buf, sizeof buf, "%f", smallest);
        strcat(res2, buf);
        write(sockfd, res2, 64);
        break;
     case 2:
        printf("Best time with Selection sort with %f s\n", smallest);

        snprintf(buf, sizeof buf, "%f", smallest);
        strcat(res3, buf);
        write(sockfd, res3, 64);
        break;
     case 3:
        printf("Best time with Heap sort with %f s\n", smallest);
        
        snprintf(buf, sizeof buf, "%f", smallest);
        strcat(res4, buf);
        write(sockfd, res4, 64);
        break;
    default:
        printf("Best time with Bubble sort with %f s\n", smallest);

        snprintf(buf, sizeof buf, "%f", smallest);
        strcat(res1, buf);
        write(sockfd, res1, 64);
        break;
   }

    data algos[4] = {bubble, shell, selection, heap};
    
    ///////////////////////////////////////////////////////
    // Write result to file

    int fd = open("result.txt", O_WRONLY | O_APPEND);
    
    write(fd, &algos, sizeof(data));
}
