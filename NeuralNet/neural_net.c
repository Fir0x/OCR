#include "neural_net.h"


Net Init_net(size_t nb_input, size_t nb_hlayer, size_t *nb_hidden,
		size_t nb_output)
{
	/*Represent the neural network. The network is composed of nb_hlayer
	 * layers. The network receives nb_input inputs. The hidden layers are
	 * composed of nb_hidden neurons. The output layer is composed of nb_output
	 * neurons.*/

	Net net;
	net.nb_hlayer = nb_hlayer;
	net.hidden_layers = (Layer*)malloc(sizeof(Layer) * nb_hlayer);
	if (net.hidden_layers == NULL)
		errx(1, "Error in Init_net (neural_net.c): allocation aborted.\n");

	net.hidden_layers[0] = Init_layer(nb_input, nb_hidden[0]);
	for (size_t i = 1; i < nb_hlayer; i++)
		net.hidden_layers[i] = Init_layer(nb_hidden[i-1], nb_hidden[i]);
	net.output_layer = Init_layer(nb_hidden[nb_hlayer-1], nb_output);

	return net;
}

Net Define_net(size_t nb_hlayer, Layer *layers)
{
	//Create a neural network from data load from a file.
	
	Net net;
	net.nb_hlayer = nb_hlayer;
	net.hidden_layers = (Layer*)malloc(sizeof(Layer) * nb_hlayer);
	if (net.hidden_layers == NULL)
		errx(1, "Error in Define_net (neural_net.c): allocation aborted.\n");

	for (size_t i = 0; i < nb_hlayer; i++)
		net.hidden_layers[i] = layers[i];
	net.output_layer = layers[nb_hlayer];

	return net;
}

Matrix Use_net(Net net, Matrix inputs)
{
	Matrix new_in = Init_matrix(inputs.width * inputs.height, 1);
	double value;
	for (size_t i = 0; i < inputs.height; i++)
	{
		for (size_t j = 0; j < inputs.width; j++)
		{
			value = Get_nb(inputs, j, i);
			Put_nb(&new_in, value, i * inputs.width + j, 0);
		}
	}

	//Use the network with inputs. No train will be performed.
	Matrix hidden_out = Activ(net.hidden_layers[0], new_in, Sigmoid);
	Matrix temp;

	for (size_t i = 1; i < net.nb_hlayer; i++)
	{
		temp = hidden_out;
		hidden_out = Activ(net.hidden_layers[i], temp, Sigmoid);
		free(temp.data);
	}

	Matrix predicted = Activ(net.output_layer, hidden_out, Softmax);

	free(new_in.data);
	free(hidden_out.data);

	return predicted;
}

void Delete_net(Net net)
{
	//Free all layers of net.
	for (size_t i = 0; i < net.nb_hlayer; i++)
		Free_layer(net.hidden_layers[i]);

	free(net.hidden_layers);
	Free_layer(net.output_layer);
}

void Print_net(Net net)
{
	//Print network in console.
	printf("////////// Network detail \\\\\\\\\\\\\\\\\\\\\n");
	printf("Number of hidden layer: %zi\n", net.nb_hlayer);
	printf("Layers detail:\n");
	for (size_t i = 0; i < net.nb_hlayer; i++)
	{
		printf("####### Hidden layer n.%zi #######\n", i);
		Print_layer(net.hidden_layers[i]);
	}
	printf("####### Output layer #######\n");
	Print_layer(net.output_layer);
	printf("////////////// Detail end \\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
}
