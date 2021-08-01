#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>
#include "timemeasuringlib.h"
#include "ourints_lib.h"
#include "perfectsquarelib.h"

//GLOBAL VARIABLES

//const int MAX_LENGTH = 145000000;



//----------MAGIC SQUARE SEQUENTIAL PROBLEM RELATED FUNCTIONS---------------


//According the the perfect square requisites, prints if the given bidimensional array
// is a MagicSquare, imperfectMagicSquare or not a magicSquare at all.
void printPerfectSquare(int **square, int squareSize, long perfectValue)
{

    if(areALlRowsPerfect(square,squareSize,perfectValue) && areALlColumnsPerfect(square,squareSize,perfectValue))
    {
        if (areDiagonalsPerfect(square,squareSize,perfectValue))
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

//-----------------------FUNCTIONAL TESTING ------------------------------
//3x3 perfect magic square
void functional_test_perfectSquare_ps()
{

    int input[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
    int squareSize = 3;
    int **square = ints_matrix_new(squareSize, squareSize);
     fillMatrix(input, square, squareSize);
    int perfectValue = ints_sum(square[0], squareSize);

    int perfectRows = areALlRowsPerfect(square,squareSize,perfectValue);
    int perfectColumns = areALlColumnsPerfect(square,squareSize,perfectValue);
    int perfectDiagonals = areDiagonalsPerfect(square,squareSize,perfectValue);

    //It's a perfect magic square
    assert(perfectRows && perfectColumns && perfectDiagonals);
}

//12x12 perfect magic square
void functional_test_perfectSquare_ps_2()
{

    int input[144] = {64,51,50,61,144,131,130,141,32,19,18,29,
                      53,58,59,56,133,138,139,136,21,26,27,24,
                      57,54,55,60,137,134,135,140,25,22,23,28,
                      52,63,62,49,132,143,142,129,20,31,30,17,
                      48,35,34,45,80,67,66,77,112,99,98,109,
                      37,42,43,40,69,74,75,72,101,106,107,104,
                      41,38,39,44,73,70,71,76,105,102,103,108,
                      36,47,46,33,68,79,78,65,100,111,110,97,
                      128,115,114,125,16,3,2,13,96,83,82,93,
                      117,122,123,120,5,10,11,8,85,90,91,88,
                      121,118,119,124,9,6,7,12,89,86,87,92,
                      116,127,126,113,4,15,14,1,84,95,94,81};
    int squareSize = 12;
    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    int perfectValue = ints_sum(square[0], squareSize);

    int perfectRows = areALlRowsPerfect(square,squareSize,perfectValue);
    int perfectColumns = areALlColumnsPerfect(square,squareSize,perfectValue);
    int perfectDiagonals = areDiagonalsPerfect(square,squareSize,perfectValue);

    //It's a perfect magic square
    assert(perfectRows && perfectColumns && perfectDiagonals);
}

//3x3 imperfect magic square
void functional_test_perfectSquare_is()
{

    int input[9] = {7,6,2,5,1,9,3,8,4};
    int squareSize = 3;
    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    int perfectValue = ints_sum(square[0], squareSize);

    int perfectRows = areALlRowsPerfect(square,squareSize,perfectValue);
    int perfectColumns = areALlColumnsPerfect(square,squareSize,perfectValue);
    int perfectDiagonals = areDiagonalsPerfect(square,squareSize,perfectValue);

    //It's a imperfect magic square
    assert(perfectRows && perfectColumns && !perfectDiagonals);
}

//3x3 non magic square
void functional_test_perfectSquare_nms()
{
    int input[9] = {7,6,2,5,1,3,3,8,4};
    int squareSize = 3;
    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    int perfectValue = ints_sum(square[0], squareSize);

    int perfectRows = areALlRowsPerfect(square,squareSize,perfectValue);
    int perfectColumns = areALlColumnsPerfect(square,squareSize,perfectValue);


    //It's not a magic square
    assert(!perfectRows || !perfectColumns );
}

void functional_tests()
{
    functional_test_perfectSquare_ps();
    functional_test_perfectSquare_ps_2();
    functional_test_perfectSquare_is();
    functional_test_perfectSquare_nms();

    printf("%s\n", "All functional tests passed with success.");
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


 int clearFilePath(const char* path)
{
     int i =  (int) strlen(path) - 1;

    while((path[i] != 'p' && path[i] != 'i' && path[i] != 'n') )
    {
            i--;
    }
    return i ;
}



//-------------------TEST FUNCTION FOR THE SEQUENTIAL PERFECT SQUARE----------

void test_perfectSquare_sequential(const char *filename)
{

    struct timeval t0;
    struct timeval t1;
    float elapsed;


    gettimeofday(&t0, 0);

//    printf("%s\n", filename);

   int startIndex = clearFilePath(filename);
//    printf("%d\n", test);
//    return 0;


//
    FILE *f = fopen(filename, "r");
    assert( f != NULL);


    int size = returnSquareFileSize(filename + startIndex);

//    printf("%d", size);

    int *input = (int *) malloc(size * size * sizeof(int));


//
//    struct timeval t0reading;
//    struct timeval t1reading;
//    float elapsedReading;

//    gettimeofday(&t0reading, 0);

    int n = fints_get(input, f);
//
//    gettimeofday(&t1reading, 0);
//    elapsedReading = timedifference_msec(t0reading, t1reading);
//    printf("Reading executed in %f milliseconds.\n", elapsedReading);
//
//
//
    int squareSize = sqrt( (double) n ) ;
//
//
//
    int **square = ints_matrix_new(squareSize,squareSize);
//
//
    fillMatrix(input,square,squareSize);
//

//
    long perfectValue = ints_sum(square[0], squareSize);
//
//
    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("Code executed in %f milliseconds.\n", elapsed);

    printPerfectSquare(square,squareSize,perfectValue);


//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Code executed in %f milliseconds.\n", elapsed);
}





int main(int argc, const char **argv) {

    const char *filename = "test.txt";
    test_perfectSquare_sequential(argc > 0 ? argv[1] : filename );

    //functional test
//    functional_tests();

    // Unit-tests for both libraries
//    perfectSquare_uni_tests();
//    ourintslib_uni_tests();

    return 0;
}
