#include "layer.h"

Layer Init_layer(size_t nb_input, size_t nb_node)
{
	/*Represent a layer of the network. It is composed of nb_node nodes. The
	 * layer receives nb_input inputs. As a node has as much weights as there
	 * are inputs and as much bias as there are nodes, layer.weights is a
	 * nb_input x nb_node dimensionnal matrix containg random values between
	 * the bound given by Xavier weights initializer. Likewise, layer.bias is a
	 * nb_node x 1 matrix contaning random values between 0 and 1.*/

	Layer layer;
	layer.nb_input = nb_input;
	layer.nb_node = nb_node;
	double w_gap = sqrt(2 / (double) nb_input); //Xavier weight init
	layer.weights = Random_matrix(nb_input, nb_node, -w_gap, w_gap);
	layer.bias = Random_matrix(nb_node, 1, -1, 1);
	return layer;
}

Layer Define_layer(Matrix weights, Matrix biases)
{
	//Create layer from data loaded from file.

	Layer layer;
	layer.nb_input = weights.width;
	layer.nb_node = weights.height;
	layer.weights = Copy_matrix(weights);
	layer.bias = Copy_matrix(biases);

	return layer;
}

void Update_layer(Layer *layer, Matrix inputs, Matrix delta, double lr)
{
	/*Update the layer thanks to his delta and the learning rate lr. temp1 and
	 * temp2 are only used to save matrices until they are used, and so to free
	 * the memory allocation of these matrices*/

	//Update the weights
	Matrix temp1 = Transpose(inputs);
	Matrix temp2 = Dot(temp1, delta);
	free(temp1.data);

	temp1 = Transpose(temp2);
	free(temp2.data);

	Matrix correct_w = Scal_wise(temp1, lr);
	free(temp1.data);

	Matrix save_w = layer->weights;
	layer->weights = Substract(save_w, correct_w);
	free(correct_w.data);
	free(save_w.data);

	//Update the biases
	Matrix save_b = layer->bias;
	temp1 = Self_sum(delta);
	temp2 = Scal_wise(temp1, lr);
	layer->bias = Substract(save_b, temp2);
	free(temp1.data);
	free(temp2.data);
	free(save_b.data);
}

void Free_layer(Layer layer)
{
	//Free weights and biases of layer.

	free(layer.weights.data);
	free(layer.bias.data);
}

void Print_layer(Layer layer)
{
	//Print layer data in the console.

	printf("Nb inputs = %zi\nNb neurons = %zi\n", layer.nb_input,
			layer.nb_node);
	printf("Weights:\n");
	Print_matrix(layer.weights, 0);
	printf("Biases:\n");
	Print_matrix(layer.bias, 0);
}
