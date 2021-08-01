
#include <pthread.h>
#include "ourints_lib.h"
#include <assert.h>



#define MAX_THREADS 12


int **square;
int squareSize;
long perfectValue;


volatile int perfectRows = 1;
volatile int perfectColumns = 1;
int perfectDiagonals = 1;



int rowsIndexes[MAX_THREADS / 2];
int columnIndexes[MAX_THREADS / 2];




//-------------------------FUNCTIONS RELATED TO THE THREADS---------------------------//

//Checks if a certain portion of the rows are perfect and if they aren't, changes the perfectRows variable to 0.
//The first thread will also check if the left diagonal is perfect and it it isn't, changes perfectDiagonal to 0.
void *rowThread(void *arg) {

    int index = (int)  arg;
    //has 6 threads


//    if (index == 0)
//    {
//        if(sumLeftDiagonal(square,squareSize) != perfectValue)
//            perfectDiagonals = 0;
//    }

    for (int i = index * (squareSize / 6); i < rowsIndexes[index]; ++i) {
        if(!perfectColumns || !perfectRows) break;
        if (ints_sum(square[i], squareSize) != perfectValue) {
            perfectRows = 0;
            break;
        }
    }



    return NULL;

}


//Checks if a certain portion of the columns are perfect and if they aren't, changes the perfectColumns variable to 0.
//The first thread will also check if the right diagonal is perfect and it it isn't, changes perfectDiagonal to 0.
void *columnThread(void *arg) {

    int index = (int) arg;



//    if (index == 0)
//    {
//        if(sumRightDiagonal(square,squareSize) != perfectValue)
//            perfectDiagonals = 0;
//    }

    for (int i = index * (squareSize / 6); i < columnIndexes[index]; ++i) {
        if(!perfectColumns || !perfectRows) break;

        if (sumColumn(square,squareSize,i)  != perfectValue ) {
            perfectColumns = 0;
            break;
        }
    }


    return NULL;
}


//Calculates the intervals that each thread will be in charge of whem making the sums.
//Essentially, it divides the total number of rows/columns in sections and each thread will be in charge of one
void setThreadIndexes(int *threadsAllocated)
{


    if(squareSize < *threadsAllocated)
    {
        *threadsAllocated = squareSize;
        for (int i = 0; i < *threadsAllocated - 1; ++i) {
            rowsIndexes[i] = (i + 1);
            columnIndexes[i] = (i + 1);
        }
        rowsIndexes[squareSize - 1] = squareSize;
        columnIndexes[squareSize - 1] = squareSize;
    }
    else
    {
        int step = squareSize / (*threadsAllocated);
        for (int i = 0; i < *threadsAllocated - 1 ; ++i) {
            rowsIndexes[i] = (i + 1) * step;
            columnIndexes[i] = (i + 1) * step;
        }
        rowsIndexes[*threadsAllocated - 1] = squareSize;
        columnIndexes[*threadsAllocated - 1] = squareSize;
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
    if(resultRightDiag != perfectValue || resultLeftDiag != perfectValue) perfectDiagonals = 0;
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

//-----------------------FUNCTIONAL TESTING ------------------------------
//3x3 perfect magic square
void functional_test_perfectSquare_ps()
{

    int input[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
     squareSize = 3;
     square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
     perfectValue = ints_sum(square[0], squareSize);

    pthread_t threads[MAX_THREADS];

    int threadsAllocated = MAX_THREADS / 2;

    setThreadIndexes(&threadsAllocated);


    for (int i = 0; i < threadsAllocated; ++i) {
        int rc1 = pthread_create(&threads[i], NULL, rowThread, (void *) i);
        if (rc1) {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        int rc2 = pthread_create(&threads[i + threadsAllocated], NULL, columnThread, (void *) i);
        if (rc2) {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    for (int i = 0; i < threadsAllocated * 2; i++) {
        pthread_join(threads[i], NULL);
    }


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
     squareSize = 12;
    square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
     perfectValue = ints_sum(square[0], squareSize);



    pthread_t threads[MAX_THREADS];

    int threadsAllocated = MAX_THREADS / 2;

    setThreadIndexes(&threadsAllocated);


    for (int i = 0; i < threadsAllocated; ++i) {
        int rc1 = pthread_create(&threads[i], NULL, rowThread, (void *) i);
        if (rc1) {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        int rc2 = pthread_create(&threads[i + threadsAllocated], NULL, columnThread, (void *) i);
        if (rc2) {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    for (int i = 0; i < threadsAllocated * 2; i++) {
        pthread_join(threads[i], NULL);
    }

    //It's a perfect magic square
    assert(perfectRows && perfectColumns && perfectDiagonals);
}

//3x3 imperfect magic square
void functional_test_perfectSquare_is()
{

    int input[9] = {7,6,2,5,1,9,3,8,4};
    squareSize = 3;
    square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    perfectValue = ints_sum(square[0], squareSize);


    pthread_t threads[MAX_THREADS];

    int threadsAllocated = MAX_THREADS / 2;

    setThreadIndexes(&threadsAllocated);


    for (int i = 0; i < threadsAllocated; ++i) {
        int rc1 = pthread_create(&threads[i], NULL, rowThread, (void *) i);
        if (rc1) {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        int rc2 = pthread_create(&threads[i + threadsAllocated], NULL, columnThread, (void *) i);
        if (rc2) {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    for (int i = 0; i < threadsAllocated * 2; i++) {
        pthread_join(threads[i], NULL);
    }


    //It's an imperfect magic square
    assert(perfectRows && perfectColumns && !perfectDiagonals);
}

//3x3 non magic square
void functional_test_perfectSquare_nms()
{
    int input[9] = {7,6,2,5,1,3,3,8,4};
    squareSize = 3;
    square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);
    perfectValue = ints_sum(square[0], squareSize);



    pthread_t threads[MAX_THREADS];

    int threadsAllocated = MAX_THREADS / 2;

    setThreadIndexes(&threadsAllocated);


    for (int i = 0; i < threadsAllocated; ++i) {
        int rc1 = pthread_create(&threads[i], NULL, rowThread, (void *) i);
        if (rc1) {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        int rc2 = pthread_create(&threads[i + threadsAllocated], NULL, columnThread, (void *) i);
        if (rc2) {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    for (int i = 0; i < threadsAllocated * 2; i++) {
        pthread_join(threads[i], NULL);
    }


    //It's not a magic square
    assert(!perfectColumns || !perfectRows );
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

    pthread_t threads[MAX_THREADS];

    //Both rows and columns will have 6 threads each
    int threadsAllocated = MAX_THREADS / 2;

    setThreadIndexes(&threadsAllocated);


    for (int i = 0; i < threadsAllocated; ++i) {
        int rc1 = pthread_create(&threads[i], NULL, rowThread, (void *) i);
        if (rc1) {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
        int rc2 = pthread_create(&threads[i + threadsAllocated], NULL, columnThread, (void *) i);
        if (rc2) {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    for (int i = 0; i < threadsAllocated * 2; i++) {
        pthread_join(threads[i], NULL);
    }

    if(perfectRows && perfectColumns) areDiagonalsPerfect(squareSize, square);

    printPerfectSquare();

//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Code executed in %f milliseconds.\n", elapsed);




}

void functional_tests()
{
    functional_test_perfectSquare_ps();
    functional_test_perfectSquare_ps_2();
    functional_test_perfectSquare_is();
    functional_test_perfectSquare_nms();

    printf("%s\n", "All functional tests passed with success.");
}



//-------------------------------------MAIN FUNCTION-----------------------------//

int main(int argc, const char **argv) {
    const char *filename = "test.txt";
//    functional_tests();
    test_perfectSquare_pthread(argc > 0 ? argv[1] : filename);
    return 0;
}
