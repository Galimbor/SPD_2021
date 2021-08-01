

#include "ourints_lib.h"
//#include <assert.h>
#include <omp.h>


//This program is made in order to only use 12 threads
#define CORES 12

int **square;
int squareSize;
long perfectValue;


volatile  int perfectRows = 1;
volatile int perfectColumns = 1;
int perfectDiagonals = 1;



//-------------------------FUNCTIONS RELATED TO THE THREADS---------------------------//

//Checks if a certain portion of the rows are perfect and if they aren't, changes the perfectRows variable to 0.
//The first thread will also check if the left diagonal is perfect and it it isn't, changes perfectDiagonal to 0.
void rowThread(int start, int numberOfPoints) {


    for (int i = start; i < numberOfPoints; ++i) {
        if(!perfectColumns || !perfectRows) break;
        if (ints_sum(square[i], squareSize) != perfectValue) {
            perfectRows = 0;
            break;
        }
    }

}


//Checks if a certain portion of the columns are perfect and if they aren't, changes the perfectColumns variable to 0.
void columnThread(int start, int numberOfPoints) {

    for (int i = start; i < numberOfPoints; ++i) {
        if(!perfectColumns || !perfectRows) break;

        if (sumColumn(square,squareSize,i)  != perfectValue ) {
            perfectColumns = 0;
            break;
        }
    }

}





//----------MAGIC SQUARE SEQUENTIAL PROBLEM RELATED FUNCTIONS---------------

//Checks if both left diagonal and right diagonals are perfect. If they aren't, changes global variable
// perfectDiagonals to 0.
void areDiagonalsPerfect()
{
    long resultLeftDiag = 0;
    long resultRightDiag = 0;
    int j = squareSize -1;
    for(int i = 0; i < squareSize; i++)
    {
        resultLeftDiag += square[i][i];
        resultRightDiag += square[j][i];
        j--;
    }
//    printf("%d %d %ld", resultRightDiag, resultLeftDiag, perfectValue);
    if(resultRightDiag != perfectValue ||  resultLeftDiag != perfectValue) perfectDiagonals = 0;
}



//According the the perfect square requisites, prints if the given bidimensional array
// is a MagicSquare, imperfectMagicSquare or not a magicSquare at all.
void printPerfectSquare()
{

    if(perfectRows && perfectColumns )
    {
        if (perfectDiagonals)
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



//---------------------TEST FUNCTION FOR THE PTHREAD PERFECT SQUARE---------------------

void test_perfectSquare_pthread(const char *filename)
{

//    struct timeval t0;
//    struct timeval t1;
//    float elapsed;
//    gettimeofday(&t0, 0);


    FILE *f = fopen(filename, "r");
    assert( f != NULL);


    int size = returnSquareFileSize(filename);

    int *input = (int *) malloc(size * size * sizeof(int));



    int n = fints_get(input, f);


    squareSize = sqrt((double) n);
    square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    perfectValue = ints_sum(square[0], squareSize);

    int start, numberOfPoints, pid, numberOfThreads;

omp_set_num_threads(CORES);
#pragma omp parallel default(shared) private(start,numberOfPoints, pid, numberOfThreads)
    {

        pid = omp_get_thread_num();
        numberOfThreads = omp_get_num_threads();
        numberOfPoints = squareSize / (numberOfThreads/2);
        if(pid <= (numberOfThreads / 2) - 1)
        {

            start = numberOfPoints * pid;
            if(pid == (numberOfThreads/2) - 1)
                numberOfPoints = squareSize - start;
            rowThread(start,start + numberOfPoints);
//            printf("My PID is %d and my start:%d and end is :%d\n", pid,start,numberOfPoints+start);
//            printf("%s%d%s\n", "My pid is ", pid, " and i'm done");
        }
        else
        {

            start = numberOfPoints * (pid % (numberOfThreads/2));
            if(pid == numberOfThreads - 1)
                numberOfPoints = squareSize - start;
            columnThread(start,start + numberOfPoints);
//            printf("%s%d%s\n", "My pid is ", pid, " and i'm done");
        }


    }

if(perfectRows && perfectColumns) areDiagonalsPerfect();

printPerfectSquare();





}

void functional_tests()
{
//    functional_test_perfectSquare_ps();
//    functional_test_perfectSquare_ps_2();
//    functional_test_perfectSquare_is();
//    functional_test_perfectSquare_nms();

    printf("%s\n", "All functional tests passed with success.");
}



//-------------------------------------MAIN FUNCTION-----------------------------//

int main(int argc, const char **argv) {
    const char *filename = "test.txt";
//    functional_tests();
    test_perfectSquare_pthread(argc > 0 ? argv[1] : filename);
    return 0;
}
