#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include "ourints_lib.h"
#include <omp.h>
#include <sys/time.h>


//This program is made in order to only use 12 threads
#define CORES 8

int **square;
int squareSize;
long perfectValue;


 int imperfectRows = 0;
 int imperfectColumns = 0;
int imperfectDiagonals = 0;


//Checks if a certain portion of the rows are perfect and if they aren't, changes the imperfectRows variable to 1.
void areRowsPerfect(const int *start, const int *end, int extraWork, const int *process_Rank,
                    const int *sizeOfCluster, int *foundAnyImperfectRowOrColumn) {



    omp_set_num_threads(CORES);
    #pragma omp parallel for
    for (int i = *start; i < *end; ++i) {

//        printf("Process is %d | Index is %d\n", *process_Rank, i);
#pragma omp critical
        {
            MPI_Allreduce(&imperfectRows, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
}

        if(*foundAnyImperfectRowOrColumn) continue;
        if (ints_sum(square[i], squareSize) != perfectValue) {
            imperfectRows = 1;
//            printf("CAUGHT IT, Process ID: %d | int i : %d \n", *process_Rank, i);
        }

    }
#pragma omp critical
    {
        MPI_Allreduce(&imperfectRows, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    }
    if(*foundAnyImperfectRowOrColumn) imperfectRows = 1;

    if(*process_Rank < (*sizeOfCluster / 2) - 1)
    {
        for(int i = 0; i < extraWork; i++) {
//            printf("EXTRA WORK: Process ID: %d | int i : %d \n", *process_Rank, i);
            #pragma omp critical
            {
                MPI_Allreduce(&imperfectRows, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            }
        }
    }



}


//Checks if a certain portion of the columns are perfect and if they aren't, changes the imperfectColumns variable to 0.
void areColumnsPerfect(const int *start, const int *end, int extraWork, const int *process_Rank,
                       const int *sizeOfCluster, int *foundAnyImperfectRowOrColumn) {

    omp_set_num_threads(CORES);
    #pragma omp parallel for
    for (int i = *start; i < *end; ++i) {

//        printf("Process is %d | Index is %d\n", *process_Rank, i);
#pragma omp critical
        {
            MPI_Allreduce(&imperfectColumns, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
}

        if(*foundAnyImperfectRowOrColumn) continue;
        if (sumColumn(square,squareSize,i)  != perfectValue ) {
            imperfectColumns = 1;
//            printf("CAUGHT IT, Process ID: %d | int i : %d \n", *process_Rank, i);
        }

    }
#pragma omp critical
    {
        MPI_Allreduce(&imperfectColumns, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    }
    if(*foundAnyImperfectRowOrColumn) imperfectColumns = 1;

    if(*process_Rank < (*sizeOfCluster) - 1)
    {
        for(int i = 0; i < extraWork; i++) {
//            printf("EXTRA WORK: Process ID: %d | int i : %d \n", *process_Rank, i);
#pragma omp critical
            {
                MPI_Allreduce(&imperfectColumns, foundAnyImperfectRowOrColumn, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            }
        }
    }


}





//----------MAGIC SQUARE SEQUENTIAL PROBLEM RELATED FUNCTIONS---------------

//Checks if both left diagonal and right diagonals are perfect. If they aren't, changes global variable
// imperfectDiagonals to 0.
void areDiagonalsPerfect()
{
    int resultLeftDiag = 0;
    int resultRightDiag = 0;
    int j = squareSize - 1;
    for(int i = 0; i < squareSize; i++)
    {
        resultLeftDiag += square[i][i];
        resultRightDiag += square[j][i];
        j--;
    }
    if(resultRightDiag != perfectValue || resultLeftDiag != perfectValue) imperfectDiagonals = 1;
}



//According the the perfect square requisites, prints if the given bidimensional array
// is a MagicSquare, imperfectMagicSquare or not a magicSquare at all.
void printPerfectSquare()
{

    if(!imperfectRows && !imperfectColumns )
    {
        if (!imperfectDiagonals)
        {
            printf("%s\n", "Quadrado mágico");
        }
        else{
            printf("%s\n", "Quadrado mágico imperfeito");
        }
    }
    else
    {
        printf("%s\n", "Não é quadrado mágico");
    }


}



int returnSquareFileSize(const char *a)
{
    int result = 0;
    int i = 0;
    char *size = (char *) malloc(strlen(a) + 1);
    while(a[result] != '.') {

        if (a[result] != 'p' && a[result] != 'i' && a[result] != 'n')
        {
            size[i++] = a[result];
        }
        result++;

    }
    int y = atoi(size);

    return y;
}


//Calculating the limit assuming half size of the cluster for Rows and half for columns
void calcLimits(int *start, int *end, int sizeOfCluster, int processRank, int *extraWork)
{
    int processesPerSection = sizeOfCluster / 2;

    int delta = squareSize / (processesPerSection);
    *start = delta * (processRank % processesPerSection);

    //Last process might have to do a few more rows/columns than the others..
    if( (processRank % processesPerSection) == processesPerSection - 1)
        delta = squareSize - *start;

    *end  = *start + delta;

    *extraWork = squareSize % processesPerSection;

}

int clearFilePath(const char* path)
{
    int i =  (int) strlen(path) - 1;

    while((path[i] != 'p' && path[i] != 'i' && path[i] != 'n') )
    {
        i--;
    }
    return i ;
}

float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (float) (t1.tv_sec - t0.tv_sec) * 1000.0f + (float) (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


int main(int argc, char** argv){


    //---------MPI CODE------------------------------//
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    int process_Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);
    //-----Reading input and putting it in the matrix----------------//
    int startIndex = clearFilePath(argv[1]);
    FILE *f = fopen(argv[1], "r");
    assert( f != NULL);


    struct timeval t0reading;
    struct timeval t1reading;
    float elapsedReading;



    gettimeofday(&t0reading, 0);

    int size = returnSquareFileSize(argv[1] + startIndex);
    int *input = (int *) malloc(size * size * sizeof(int));
    int n = fints_get(input, f);
    squareSize = sqrt((double) n);
    square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);



    MPI_Barrier(MPI_COMM_WORLD);

    if(process_Rank == 0) {
        gettimeofday(&t1reading, 0);
        elapsedReading = timedifference_msec(t0reading, t1reading);
        printf("Reading executed in %f milliseconds.\n", elapsedReading);
    }


    perfectValue = ints_sum(square[0], squareSize);

    //-----------------SOME VARIABLES--------------------------//

    int  size_Of_Cluster;
    int start, end;


    int foundAnyImperfectRowOrColumn = 0;

    int extraWork;

    MPI_Barrier(MPI_COMM_WORLD);

    struct timeval t0;
    struct timeval t1;
    float elapsed;

    gettimeofday(&t0, 0);



    if(provided < MPI_THREAD_MULTIPLE)
    {
        printf("The threading support level is lesser than that demanded.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }




    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);


    //Calculating limits for the 'for' loop along with the extra cycles to avoid deadlocks
    calcLimits(&start, &end,size_Of_Cluster, process_Rank, &extraWork);



    //First half takes care of the rows
    if( process_Rank < size_Of_Cluster / 2)
    {
//        printf("ID:%d | START:%d | END: %d\n", process_Rank, start, end);
        areRowsPerfect(&start,&end, extraWork, &process_Rank, &size_Of_Cluster, &foundAnyImperfectRowOrColumn);
    }
        //Second half takes care of the columns
    else
    {
        areColumnsPerfect(&start,&end, extraWork, &process_Rank, &size_Of_Cluster, &foundAnyImperfectRowOrColumn);
    }



    MPI_Barrier(MPI_COMM_WORLD);


//    printf("RANK: %d | ImPerfect Rows : %d | ImPerfect Columns : %d\n",process_Rank, imperfectRows, imperfectColumns);

    if(process_Rank == 0)
    {
        if(!imperfectColumns && !imperfectRows) areDiagonalsPerfect();

        printPerfectSquare();

        gettimeofday(&t1, 0);
        elapsed = timedifference_msec(t0, t1);
        printf("Processing executed in %f milliseconds.\n", elapsed);
    }

    MPI_Finalize();

    return 0;
}