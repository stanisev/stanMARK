#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "sort.h"
#include "time.h"
#include <stdlib.h>
#include <string.h>
#define SIZE 10000;


void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


typedef struct data
{
    char algorithmName[20];
    double time;
} data;


int * intdup(int const * src, size_t len)
{
   int * p = malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}


void sort(int arr[], int n)
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
        break;
     case 1:
        printf("Best time with Shell sort with %f s\n", smallest);
        break;
     case 2:
        printf("Best time with Selection sort with %f s\n", smallest);
        break;
     case 3:
        printf("Best time with Heap sort with %f s\n", smallest);
        break;
    default:
        printf("Best time with Bubble sort with %f s\n", smallest);
        break;
   }

    data algos[4] = {bubble, shell, selection, heap};
    
    ///////////////////////////////////////////////////////

    FILE *out;

    out = fopen("../results/data.txt", "w");

    if (out == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }

    char buffer_in[256], buffer_out[256];

     for(int i=0; i<4;i++){
        if(i!=slow){
          sprintf(buffer_out, "%d,%s,%f\n", i, algos[i].algorithmName, algos[i].time);
          fwrite(buffer_out, sizeof(char), strlen(buffer_out), out);

        }
     }


    printf("\n");




}


int main(int argc, char *argv[]){
    FILE *fp = fopen("../files/SEQUENCE3.txt", "r");

    //read file into array
    int arr[10001];
    int i;

    for (i = 0; i < 10000; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }

    sort(arr, 10000);
    fclose(fp);
    printf("\n");
}
