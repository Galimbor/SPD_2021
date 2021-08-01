//
// Created by artur on 3/5/21.
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef SPD_LAB1_OURINTS_LIB_H
#define SPD_LAB1_OURINTS_LIB_H

//---------MEMORY ALLOCATION AND CLEARING--------

//Author : Professor Pedro Guerreiro
// Create a dynamically allocated array with capacity `n`

int *ints_new(int n)
{
    return (int *) malloc (n * sizeof(int) );
}

//Author : Professor Pedro Guerreiro
//Dynamic allocation for the matrix
int **ints_matrix_new(int rows, int cols)
{
    int **result = (int **) malloc(rows * sizeof(int *));
    int *p = ints_new(rows * cols);
    for (int i = 0; i < rows; i++, p += cols) result[i] = p;
    return result;
}

//Author : Professor Pedro Guerreiro
//Function that clears the memory

void ints_clear(int *a, int n)
{
    memset(a, 0, n * sizeof(int));
}

//-------------FUNCTIONS FOR PRINTING----------------------

//Author : Professor Pedro Guerreiro
// Write array elements in a line, separated by a space.
void ints_println_basic(const int *a, int n)
{
    if (n > 0)
    {
        printf("%d", a[0]);
        for (int i = 1; i < n; i++)  // i = 1
            printf(" %d", a[i]);
    }
    printf("\n");
}

//Author : Professor Pedro Guerreiro
void ints_printf(int *a, int n, const char *fmt)
{
    for (int i = 0; i < n; i++)
        printf(fmt, a[i]);
}

//Author : Professor Pedro Guerreiro
void ints_printfln(int *a, int n, const char *fmt)
{
    ints_printf(a, n, fmt);
    printf("\n");
}

//Author : Professor Pedro Guerreiro
void ints2_printf(int **m, int rows, int columns, const char *fmt)
{
    for (int i = 0; i < rows; i++) ints_printfln(m[i], columns, fmt);
}

//-------------FUNCTIONS TO READ--------------

//Author : Professor Pedro Guerreiro
// Read array elements until no more items exists for reading.
int ints_get(int *a)
{
    int result = 0;
    int x;
    while (scanf("%d", &x) != EOF)
        a[result++] = x;
    return result;
}

//Author : Professor Pedro Guerreiro
// Read array elements until no more items exists for reading.
int fints_get(int *a, FILE *file)
{
    int result = 0;
    int x;
    while (fscanf(file,"%d", &x) != EOF) {
        a[result++] = x;
    }
    return result;
}


//Given a one-dimensional array, fills a bidimensional array with its content.
void fillMatrix(const int *input, int **array, int arraySize) {
    int index = 0;

    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            array[i][j] = input[index++];
        }
    }
}


//-------ARITHMETIC FUNCTIONS-------------

//Author : Professor Pedro Guerreiro
// Sum of all elements of the array.
long ints_sum(const int *a, int n)
{
    long result = 0;
    for (int i = 0; i < n; i++)
        result += a[i];
//    printf("Sum value is %ld\n", result);
    return result;
}

//Uni test created by : Pedro Guerreiro
void unit_test_ints_sum(void)
{
    int a1[8] = {6,7,1,8, 9,3,3,5};
    assert(ints_sum(a1, 8) == 42);
    assert(ints_sum(a1, 4) == 22);
    assert(ints_sum(a1, 2) == 13);
    assert(ints_sum(a1, 1) == 6);
    assert(ints_sum(a1, 0) == 0);
    int a2[10] = {1,5,9,13, 17,21,25,29, 33,37};
    assert(ints_sum(a2, 10) == 190);
    assert(ints_sum(a2, 5) == 45);
}


//Given a bidimensional array and the column index, sums the corresponding column and returns the sum.
long sumColumn(int **array, int n, int columnIndex) {
    long result = 0;
    for (int i = 0; i < n; i++) {
        result += array[i][columnIndex];
    }
    return result;
}


void uni_test_sumColumn()
{

    int input[9] = {7,6,2,
                    5,1,3,
                    3,8,4};
    int squareSize = 3;

    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);

    assert(sumColumn(square,squareSize, 0) == 15);
    assert(sumColumn(square,squareSize, 1) == 15);
    assert(sumColumn(square,squareSize, 2) == 9);

    int input2[9] = {8, 1, 6,
                     3, 5, 7,
                     1, 9, 2};
    fillMatrix(input2, square, squareSize);

    assert(sumColumn(square,squareSize, 0) == 12);
    assert(sumColumn(square,squareSize, 1) == 15);
    assert(sumColumn(square,squareSize, 2) == 15);

    int input3[144] = {64,51,50,61,144,131,130,141,32,19,18,29,
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
    fillMatrix(input3, square, squareSize);

    assert(sumColumn(square,squareSize, 0) == 870);
    assert(sumColumn(square,squareSize, 1) == 870);
    assert(sumColumn(square,squareSize, 2) == 870);
    assert(sumColumn(square,squareSize, 10) == 870);
    assert(sumColumn(square,squareSize, 11) == 870);

}




/*Returns the sum of the left diagonal of a given bidimensional array.
 */
long sumLeftDiagonal(int **array, int squareSize) {


    long sum = 0;
    for (int j = 0; j < squareSize; ++j) {
        sum += array[j][j];
    }

    return sum;
}

/*Returns the sum of the right diagonal of a given bidimensional array.
 */
long sumRightDiagonal(int **array, int squareSize) {


    long sum = 0;

    int column = squareSize - 1;

    for (int j = 0; j < squareSize; ++j) {
        sum += array[j][column--];

    }

    return sum;
}


void uni_test_sumDiagonals()
{

    int input[9] = {7,6,2,
                    5,1,3,
                    3,8,4};
    int squareSize = 3;

    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);

    assert(sumLeftDiagonal(square,squareSize) == 12);
    assert(sumRightDiagonal(square,squareSize) == 6);

    int input2[9] = {8, 1, 6,
                     3, 5, 7,
                     4, 9, 2};
    fillMatrix(input2, square, squareSize);

    assert(sumLeftDiagonal(square,squareSize) == 15);
    assert(sumRightDiagonal(square,squareSize) == 15);

    int input3[144] = {64,51,50,61,144,131,130,141,32,19,18,29,
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
    fillMatrix(input3, square, squareSize);

    assert(sumLeftDiagonal(square,squareSize) == 870);
    assert(sumRightDiagonal(square,squareSize) == 870);

}

void ourintslib_uni_tests()
{
    unit_test_ints_sum();
    uni_test_sumColumn();
    uni_test_sumDiagonals();
    printf("All tests passed successfully!\n");
}







#endif //SPD_LAB1_OURINTS_LIB_H
