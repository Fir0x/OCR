#ifndef NEURALNET_H
#define NEURALNET_H

#include "layer.h"
#include "train.h"

typedef struct Net Net;
struct Net
{
	size_t nb_hlayer;
	Layer* hidden_layers;
	Layer output_layer;
};

Net Init_net(size_t nb_input, size_t nb_hlayer, size_t *nb_hidden,
		size_t nb_output);
Net Define_net(size_t nb_hlayer, Layer *layers);
Matrix Use_net(Net net, Matrix inputs);
void Delete_net(Net net);
void Print_net(Net net);

#endif
