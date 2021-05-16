#include <stdio.h>
#include "sorts.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct data
{
    char fname[20];
    double time;
} data;

void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void sort(int arr[], int n)
{

    int *arrShell = arr;

    clock_t beginBubble = clock();
    bubbleSort(arr, n);
    clock_t endBubble = clock();
    double timeBubble = (double)(endBubble - beginBubble) / CLOCKS_PER_SEC;
    printf("Time for bubble: %f", timeBubble);
    
    clock_t beginShell = clock();
    bubbleSort(arrShell, n);
    clock_t endShell = clock();
    double timeShell = (double)(endShell - beginBubble) / CLOCKS_PER_SEC;
    printf("Time for shell: %f", timeShell);


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
       printf ("Sort = %s time = %s\n", inp.fname, inp.time);
    fclose (inf);


}

int main() {
   /*

    FILE * fp;

    int arr3[1000] = {0};


    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i=0;
    
    if (fp = fopen("../files/SEQUENCE1.txt", "r")) {
        while (fscanf(fp, "%d", &arr3[i]) != EOF) {
            ++i;
        }
        fclose(fp);
    }

    int n3 = sizeof(arr3)/sizeof(arr3[0]);

    sort(arr3, n3);

    printArray(arr3, n3);
    

    */

    
    FILE *fp = fopen("../files/SEQUENCE3.txt", "r");

    //read file into array
    int arr[10001];
    int i;

    for (i = 0; i < 10000; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }

    sort(arr, 10000);
    printf("\n");


    return 0;
}