#include "train.h"

#define BATCH_SIZE 4

//Activation functions
Matrix Sigmoid(Matrix x)
{
	Matrix result = Init_matrix(x.width, x.height);
	double value;
	for (size_t i = 0; i < x.height; i++)
	{
		for (size_t j = 0; j < x.width; j++)
		{
			value = 1 / (1 + exp(-Get_nb(x, j, i)));
			Put_nb(&result, value, j, i);
		}
	}

	return result;
}

Matrix Softmax(Matrix x)
{
	Matrix sums = Init_matrix(x.height, 1);
	Matrix result = Init_matrix(x.width, x.height);
	double value;
	double z;
	double maxz;

	//Ensure softmax stability
	for (size_t i = 0; i < x.height; i++)
	{
		maxz = Get_nb(x,0,i);
		for (size_t j = 1; j < x.width; j++)
		{
			z = Get_nb(x,j,i);
			if (z > maxz)
				maxz = z;
		}
		for (size_t j = 0; j < x.width; j++)
			Put_nb(&x, Get_nb(x,j,i) - maxz,j,i);
	}
	
	//Sum exp of all values of neurons (by case) before activation
	for (size_t i = 0; i < x.height; i++)
	{
		value = 0;
		for (size_t j = 0; j < x.width; j++)
			value += exp(Get_nb(x, j, i));

		Put_nb(&sums, value, i, 0);
	}

	//Apply softmax function
	for (size_t i = 0; i < x.height; i++)
	{
		for (size_t j = 0; j < x.width; j++)
		{
			value = exp(Get_nb(x, j, i)) / Get_nb(sums, i, 0);
			Put_nb(&result, value, j, i);
		}
	}

	free(sums.data);
	return result;
}

//Activation functions derivatives
Matrix Sigmoid_dv(Matrix x)
{
	Matrix result = Init_matrix(x.width, x.height);
	double value;
	double fvalue;
	for (size_t i = 0; i < x.height; i++)
	{
		for (size_t j = 0; j < x.width; j++)
		{
			value = Get_nb(x, j, i);
			fvalue = value * (1 - value);
			Put_nb(&result, fvalue, j, i);
		}
	}

	return result;
}

Matrix Softmax_dv(Matrix x)
{
	Matrix result = Init_matrix(x.width, x.height);
	double value;
	double fvalue;
	for (size_t i = 0; i < x.height; i++)
	{
		for (size_t j = 0; j < x.width; j++)
		{
			value = Get_nb(x, j, i);
			fvalue = value * ((i == j) - value);
			Put_nb(&result, fvalue, j, i);
		}
	}

	return result;
}

//Training functions
Matrix Activ(Layer layer, Matrix input, Matrix (*activ_func)(Matrix))
{
	/*Active the layer. Return the resulting values after inputs have been
	 * processed by the layer.*/

	Matrix temp = Transpose(layer.weights);
	Matrix pre_activ = Dot(input, temp);
	free(temp.data);
	double value;
	for (size_t i = 0; i < pre_activ.height; i++)
	{
		for (size_t j = 0; j < pre_activ.width; j++)
		{
			//Apply activation function for all versions of inputs 
			value = Get_nb(pre_activ, j, i) + Get_nb(layer.bias, j, 0);
			Put_nb(&pre_activ, value, j, i);
		}
	}

	Matrix result = (*activ_func)(pre_activ);
	free(pre_activ.data);

	return result;
}

Matrix Delta(Matrix error, Matrix prediction, Matrix (*activ_dv)(Matrix))
{
	//Process the delta of a layer thanks to its error and its prediction
	Matrix temp = (*activ_dv)(prediction);
	Matrix result = Element_wise(error, temp);
	free(temp.data);

	return result;
}

void Backprop(Matrix *d_hidden, Matrix *d_out, size_t nb_hlayer,
		Matrix *hidden_out, Matrix expected, Matrix predicted,
		Matrix output_weights, Layer *h_layers)
{
	/*Modify pointers *d_hidden and *d_out to return the delta of the layers.
	 *Process the prediction error and the deltas thanks to the others
	 *parameters.*/

	Matrix error = Substract(predicted, expected);
	*d_out = error;//Delta(error, predicted, Softmax_dv);

	Matrix *error_hidden = (Matrix*)malloc(sizeof(Matrix) * nb_hlayer);
	if (error_hidden == NULL)
		errx(1, "Error in Backprop (train.c): allocation failed.\n");
	error_hidden[nb_hlayer-1] = Dot(*d_out, output_weights);
	d_hidden[nb_hlayer-1] = Delta(error_hidden[nb_hlayer-1],
			hidden_out[nb_hlayer-1], Sigmoid_dv);

	//index shift in loop because size_t can not be less than 0
	for (size_t i = nb_hlayer-1; i > 0; i--)
	{
		error_hidden[i-1] = Dot(d_hidden[i], h_layers[i].weights);
		d_hidden[i-1] = Delta(error_hidden[i-1], hidden_out[i-1], Sigmoid_dv);
	}

	//Free all matrix that will not be used again
	for (size_t i = 0; i < nb_hlayer; i++)
		free(error_hidden[i].data);
	free(error_hidden);
}

Matrix* Create_batches(Liste dataset, Matrix expected, Matrix *new_expected,
		size_t nb_batches)
{
	Matrix *all_batches = malloc(sizeof(Matrix) * nb_batches);
	if (all_batches == NULL)
		errx(1, "error in Create_batches (train.c): allocation failed.\n");

	size_t width = dataset.first->mat.width;
	size_t height = dataset.first->mat.height;
	double value;
	size_t dataset_size = dataset.len;
	int *used = calloc(dataset_size, sizeof(int));
	size_t choosed;
	Matrix choosed_mat;

	for (size_t n = 0; n < nb_batches; n++)
	{
		all_batches[n] = Init_matrix(width * height, BATCH_SIZE);
		new_expected[n] = Init_matrix(expected.width, BATCH_SIZE);

		for (size_t i = 0; i < BATCH_SIZE; i++)
		{
			do
			{
				choosed = Random_size(0, dataset_size);
			}
			while (used[choosed] == 1);
			used[choosed] = 1;

			for (size_t j = 0; j < expected.width; j++)
			{
				value = Get_nb(expected, j, choosed);
				Put_nb(&new_expected[n], value, j, i);
			}

			choosed_mat = Get_elm(&dataset, choosed).mat;
			for (size_t j = 0; j < height; j++)
			{
				for (size_t k = 0; k < width; k++)
				{
					value = Get_nb(choosed_mat, k, j);
					Put_nb(&all_batches[n], value, j * width + k, i);
				}
			}
		}
	}

	free(used);

	return all_batches;
}

void Train(Layer *h_layers, Layer *o_layer, size_t nb_hlayer, Liste dataset,
		Matrix expected, size_t epochs, double lr)
{
	/*Train the network with bunch of inputs. Apply forward propagation, then
	 * back propagation, adn finally update the layers. Activation functions:
	 * - hidden layers: sigmoid
	 * - output layer: softmax*/

	size_t nb_batches = dataset.len / BATCH_SIZE;
	Matrix *new_expected = malloc(sizeof(Matrix) * nb_batches);
	if (new_expected == NULL)
		errx(1, "error in Train (train.c): allocation failed.\n");

	Matrix *mini_batches = Create_batches(dataset, expected, new_expected,
			nb_batches);
	Matrix inputs;

	for (size_t i = 0; i < epochs; i++)
	{
		printf("\rEpoch %zi/%zi", i+1, epochs);
		fflush(stdout);
		for (size_t n = 0; n < nb_batches; n++)
		{
			inputs = mini_batches[n]; //Select mini batch

			//Forward propagation
			Matrix *hidden_out = (Matrix*)malloc(sizeof(Matrix) * nb_hlayer);
			if (hidden_out == NULL)
				errx(1, "Error in Train (train.c): allocation failed.\n");

			hidden_out[0] = Activ(h_layers[0], inputs, Sigmoid);
			for (size_t j = 1; j < nb_hlayer; j++)
				hidden_out[j] = Activ(h_layers[j], hidden_out[j-1], Sigmoid);

			Matrix predicted = Activ(*o_layer, hidden_out[nb_hlayer-1],
					Softmax);

			//Back propagation
			Matrix *d_hidden = (Matrix*)malloc(sizeof(Matrix) * nb_hlayer);
			if (d_hidden == NULL)
				errx(1, "Error in Train (train.c): allocation failed.\n");

			Matrix d_out;
			Backprop(d_hidden, &d_out, nb_hlayer, hidden_out, new_expected[n],
					predicted, o_layer->weights, h_layers);

			//Update layers
			Update_layer(&h_layers[0], inputs, d_hidden[0], lr);
			for (size_t j = 1; j < nb_hlayer; j++)
				Update_layer(&h_layers[j], hidden_out[j-1], d_hidden[j], lr);

			Update_layer(o_layer, hidden_out[nb_hlayer-1], d_out, lr);

			//Free all matrices that will ne be used again
			free(predicted.data);
			for (size_t j = 0; j < nb_hlayer; j++)
			{
				free(hidden_out[j].data);
				free(d_hidden[j].data);
			}
			free(hidden_out);
			free(d_hidden);
			free(d_out.data);
		}
	}

	//Free mini batches
	for (size_t n = 0; n < nb_batches; n++)
	{
		free(mini_batches[n].data);
		free(new_expected[n].data);
	}
	free(mini_batches);
	free(new_expected);
	printf("\nDone\n");
}
