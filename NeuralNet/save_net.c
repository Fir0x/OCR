#include "save_net.h"

void Write_layer(Layer layer, FILE *file)
{
	//Serialize layer and write the serialization in file.

	fprintf(file, "%zi %zi\n", layer.nb_input, layer.nb_node);

	for (size_t i = 0; i < layer.nb_node; i++)
	{
		for (size_t j = 0; j < layer.nb_input; j++)
			fprintf(file, "%f ", Get_nb(layer.weights, j, i));
	}
	fprintf(file, "\n");

	for (size_t i = 0; i < layer.nb_node; i++)
		fprintf(file, "%f ", Get_nb(layer.bias, i, 0));
	fprintf(file, "\n\n");
}

Layer Read_layer(FILE *file)
{
	//Load a layer from file.

	size_t nb_input;
	size_t nb_node;
	double value;
	int check;

	check = fscanf(file, "%zi %zi\n", &nb_input, &nb_node);
	if (check != 2)
		errx(1, "Error in Read_layer (save_net.c): invalid save file.");
	Matrix weights = Init_matrix(nb_input, nb_node);
	Matrix biases = Init_matrix(nb_node, 1);
	for (size_t i = 0; i < nb_node; i++)
	{
		for (size_t j = 0; j < nb_input; j++)
		{
			check = fscanf(file, "%lf ", &value);
			if (check != 1)
				errx(1,"Error in Read_layer (save_net.c): invalid save file.");
			Put_nb(&weights, value, j, i);
		}
	}
	check = fscanf(file, "\n");
	if (check != 0)
		errx(1, "Error in Read_layer (save_net.c): invalid save file.");

	for (size_t i = 0; i < nb_node; i++)
	{
		check = fscanf(file, "%lf ", &value);
		if (check != 1)
			errx(1, "Error in Read_layer (save_net.c): invalid save file.");
		Put_nb(&biases, value, i, 0);
	}
	check = fscanf(file, "\n\n");
	if (check != 0)
		errx(1, "Error in Read_layer (save_net.c): invalid save file.");

	Layer layer = Define_layer(weights, biases);
	free(weights.data);
	free(biases.data);

	return layer;
}

void Save_net(Net net, char path[])
{
	//Save net in a file.

	FILE *file = NULL;
	file = fopen(path, "w");
	fprintf(file, "%zi\n\n", net.nb_hlayer);
	for (size_t i = 0; i < net.nb_hlayer; i++)
		Write_layer(net.hidden_layers[i], file);
	Write_layer(net.output_layer, file);
	fclose(file);
}

Net Load_net(char path[])
{
	/*Load a neural network serialized in this application format from a file
	 * at path.*/

	FILE *file = NULL;
	file = fopen(path, "r");
	if (file == NULL)
		errx(1, "Error in Load_net (save_net.c): file loading failed.");

	size_t nb_hlayer;
	int check;
	check = fscanf(file, "%zi\n", &nb_hlayer);
	if (check != 1)
		errx(1, "Error in loading net.");
	Layer *layers = (Layer*)malloc(sizeof(Layer) * (nb_hlayer + 1));
	for (size_t i = 0; i < nb_hlayer+1; i++)
		layers[i] = Read_layer(file);
	Net net = Define_net(nb_hlayer, layers);
	fclose(file);

	free(layers);
	return net;
}
