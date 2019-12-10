#include "matrix.h"

Matrix Init_matrix(size_t width, size_t height)
{
	Matrix matrix;
	matrix.width = width;
	matrix.height = height;
	matrix.data = (double*)calloc(width * height, sizeof(double));
	if (matrix.data == NULL)
		errx(1, "Error in Init_matrix (matrix.c): allocation failed.\n");

	return matrix;
}

void Put_nb(Matrix *m, double nb, size_t x, size_t y)
{
	if (x >= m->width)
		errx(1, "Error in Put_nb (matrix.c): x > width, x=%zi width=%zi.\n",
				x, m->width);
	if (y >= m->height)
		errx(1, "Error in Put_nb (matrix.c): y > height, y=%zi height=%zi.\n",
				y, m->height);

	(m->data)[y * (m->width) + x] = nb;
}

double Get_nb(Matrix m, size_t x, size_t y)
{
	if (x >= m.width)
		errx(1, "Error in Get_nb (matrix.c): x > width, x=%zi width=%zi\n",
				x, m.width);
	if (y >= m.height)
		errx(1, "Error in Get_nb (matrix.c): y > height, y=%zi height=%zi\n",
				y, m.height);

	return m.data[y * m.width + x];
}

Matrix Transpose(Matrix m)
{
	Matrix result = Init_matrix(m.height, m.width);
	for (size_t i = 0; i < m.height; i++)
	{
		for (size_t j = 0; j < m.width; j++)
		{
			Put_nb(&result, Get_nb(m, j, i), i, j);
		}
	}

	return result;
}

Matrix Sum(Matrix m1, Matrix m2)
{
	Matrix result = Init_matrix(m1.width, m1.height);
	for (size_t i = 0; i < m1.height; i++)
	{
		for (size_t j = 0; j < m1.width; j++)
			Put_nb(&result, Get_nb(m1, j, i) + Get_nb(m2, j, i), j, i);
	}

	return result;
}

Matrix Substract(Matrix m1, Matrix m2)
{
	Matrix result = Init_matrix(m1.width, m1.height);
	for (size_t i = 0; i < m1.height; i++)
	{
		for (size_t j = 0; j < m1.width; j++)
			Put_nb(&result, Get_nb(m1, j, i) - Get_nb(m2, j, i), j, i);
	}

	return result;
}

Matrix Self_sum(Matrix m)
{
	Matrix result = Init_matrix(m.width, 1);
	double value;
	for (size_t i = 0; i < m.width; i++)
	{
		value = 0;
		for (size_t j = 0; j < m.height; j++)
			value += Get_nb(m, i, j);
		Put_nb(&result, value, i, 0);
	}

	return result;
}


Matrix Dot(Matrix m1, Matrix m2)
{
	Matrix result = Init_matrix(m2.width, m1.height);
	double value;
	for (size_t i = 0; i < m1.height; i++)
	{
		for (size_t j = 0; j < m2.width; j++)
		{
			value = 0;
			for (size_t k = 0; k < m1.width; k++)
				value += Get_nb(m1, k, i) * Get_nb(m2, j, k);

			Put_nb(&result, value, j, i);
		}
	}

	return result;
}

Matrix Element_wise(Matrix m1, Matrix m2)
{
	Matrix result = Init_matrix(m1.width, m1.height);
	for (size_t i = 0; i < m1.height; i++)
	{
		for (size_t j = 0; j < m1.width; j++)
			Put_nb(&result, Get_nb(m1, j, i) * Get_nb(m2, j, i), j, i);
	}

	return result;
}

Matrix Scal_wise(Matrix m, double scal)
{
	Matrix result = Init_matrix(m.width, m.height);
	for (size_t i = 0; i < m.height; i++)
	{
		for (size_t j = 0; j < m.width; j++)
			Put_nb(&result, Get_nb(m, j, i) * scal, j, i);
	}

	return result;
}

Matrix Copy_matrix(Matrix m)
{
	Matrix result = Init_matrix(m.width, m.height);
	for (size_t i = 0; i < m.height; i++)
	{
		for (size_t j = 0; j < m.width; j++)
			Put_nb(&result, Get_nb(m, j, i), j, i);
	}

	return result;
}

Matrix Shuffle_matrix(Matrix m)
{
	Matrix result = Init_matrix(m.width, m.height);
	//*filled is a list which contains the index of result lines which have
	//already been used. It avoids to copy several lines of m in the same
	//line of result.
	size_t *filled = (size_t*)calloc(m.height, sizeof(size_t));
	if (filled == NULL)
		errx(1, "Error in Shuffle_matrix (matrix.c): allocation failed.\n");

	size_t y;
	for (size_t i = 0; i < m.height; i++)
	{
		do
			{
				y = Random_size(0, m.height);
			}while (filled[y] != 0);
		filled[y] = 1; //Line y is now filled so save info in list *filled

		for (size_t j = 0; j < m.width; j++)
			Put_nb(&result, Get_nb(m, j, i), j, y);
	}

	free(filled); //filled is useless out of the func

	return result;
}

Matrix Random_matrix(size_t width, size_t height, float min, float max)
{
	Matrix result = Init_matrix(width, height);
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
			Put_nb(&result, Random_double(min, max), j, i);
	}

	return result;
}

void Print_matrix(Matrix m, int isChar)
{
	if (isChar == 1)
	{
		double value;
		for(size_t y = 0; y < m.height; y++)
		{
			for(size_t x = 0; x < m.width; x++)
			{
				value = Get_nb(m, x, y);
				if (value == 0 || value == 1)
					printf("%c", value ? '*' : ' ');
				else
					printf("%i ", (int)value);
			}
			printf("\n");
		}
	}
	else
	{
		for(size_t y = 0; y < m.height; y++)
		{
			for(size_t x = 0; x < m.width; x++)
				printf("%f     (%zi,%zi)\n", m.data[y * m.width + x], x, y);
		}
	}
	printf("\n");
}
