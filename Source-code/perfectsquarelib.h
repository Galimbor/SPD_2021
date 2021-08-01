//
// Created by artur on 3/5/21.
//


#include "ourints_lib.h"
#include "timemeasuringlib.h"
#include <assert.h>
#ifndef SPD_LAB1_PERFECTSQUARELIB_H
#define SPD_LAB1_PERFECTSQUARELIB_H

//----------MAGIC SQUARE PROBLEM RELATED FUNCTIONS---------------



/*Checks if all sums of each corresponding  rows of a given bidimensional array
is equal to the passed value.
 Returns 1 is true, 0 if not.
 */
int areALlRowsPerfect(int **square, int squareSize, long value) {

//    struct timeval t0;
//    gettimeofday(&t0, 0);
//    struct timeval t1;
//    float elapsed;
    int result = 1;

    for (int i = 0; i < squareSize; ++i) {
        if (ints_sum(square[i], squareSize) != value) {

            result = 0;
            break;
        }
    }

//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Rows executed in %f milliseconds.\n", elapsed);

    return result;
}

void uni_test_areALlRowsPerfect()
{

    int input[9] = {7,6,2,5,1,3,3,8,4};
    int squareSize = 3;

    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);

    long value = 15;

    assert(areALlRowsPerfect(square, squareSize, value) == 0);

    int input2[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
    fillMatrix(input2, square, squareSize);

    value = 15;

    assert(areALlRowsPerfect(square, squareSize, value) == 1);

    int input3[9] = {7,6,2,
                     5,1,9,
                     3,8,4};

    fillMatrix(input3, square, squareSize);

    assert(areALlRowsPerfect(square, squareSize, value) == 1);

}


/*Checks if all sums of each corresponding column of a given bidimensional array
are equal to the passed value.
 Returns 1 is true, 0 if not.
 */
int areALlColumnsPerfect(int **square, int squareSize, long value) {

//    struct timeval t0;
//    gettimeofday(&t0, 0);
//    struct timeval t1;
//    float elapsed;

    int result = 1;

    for (int j = 0; j < squareSize; ++j) {
        if (sumColumn(square, squareSize, j) != value) {
            result = 0;
            break;
        }
    }

//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Columns executed in %f milliseconds.\n", elapsed);

    return result;
}

void uni_test_areALlColumnsPerfect()
{

    int input[9] = {7,6,2,5,1,3,3,8,4};
    int squareSize = 3;

    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);

    long value = 15;

    assert(areALlColumnsPerfect(square, squareSize, value) == 0);

    int input2[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
    fillMatrix(input2, square, squareSize);

    value = 15;

    assert(areALlColumnsPerfect(square, squareSize, value) == 1);

    int input3[9] = {7,6,2,
                     5,1,9,
                     3,8,4};

    fillMatrix(input3, square, squareSize);

    assert(areALlColumnsPerfect(square, squareSize, value) == 1);

}




// Checks if the sum of the right and left diagonal of a given bidimensional array
//are equal to the passed value.
// Returns 1 is true, 0 if not.
int areDiagonalsPerfect(int **square, int squareSize, long value) {
//    struct timeval t0;
//    gettimeofday(&t0, 0);
//    struct timeval t1;
//    float elapsed;

    int isLeftDiagonalPerfect = sumLeftDiagonal(square, squareSize) == value;
    int isRightDiagonalPerfect = sumRightDiagonal(square,squareSize) == value;

//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Diagonals executed in %f milliseconds.\n", elapsed);

    return   isLeftDiagonalPerfect && isRightDiagonalPerfect;
}

void uni_test_areALlDiagonalsPerfect()
{

    int input[9] = {7,6,2,5,1,3,3,8,4};
    int squareSize = 3;

    int **square = ints_matrix_new(squareSize, squareSize);
    fillMatrix(input, square, squareSize);

    long value = 15;

    assert(areDiagonalsPerfect(square, squareSize, value) == 0);

    int input2[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
    fillMatrix(input2, square, squareSize);

    value = 15;

    assert(areDiagonalsPerfect(square, squareSize, value) == 1);

    int input3[9] = {7,6,2,
                     5,1,9,
                     3,8,4};

    fillMatrix(input3, square, squareSize);

    value = 6;

    assert(areDiagonalsPerfect(square, squareSize, value) == 0);

}




void perfectSquare_uni_tests()
{
    uni_test_areALlRowsPerfect();
    uni_test_areALlColumnsPerfect();
    uni_test_areALlDiagonalsPerfect();
    printf("All tests passed successfully!\n");
}



#endif //SPD_LAB1_PERFECTSQUARELIB_H
