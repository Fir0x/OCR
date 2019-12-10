#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <err.h>
#include "random.h"

typedef struct Matrix Matrix;
struct Matrix
{
	size_t width;
	size_t height;
	double* data;
};

Matrix Init_matrix(size_t width, size_t height);
void Put_nb(Matrix *m, double nb, size_t x, size_t y);
double Get_nb(Matrix m, size_t x, size_t y);
Matrix Transpose(Matrix m);
Matrix Sum(Matrix m1, Matrix m2);
Matrix Substract(Matrix m1, Matrix m2);
Matrix Self_sum(Matrix m);
Matrix Dot(Matrix m, Matrix m2);
Matrix Element_wise(Matrix m1, Matrix m2);
Matrix Scal_wise(Matrix m1, double scal);
Matrix Copy_matrix(Matrix m);
Matrix Shuffle_matrix(Matrix m);
Matrix Random_matrix(size_t width, size_t height, float min, float max);
void Print_matrix(Matrix m, int isChar);

#endif
