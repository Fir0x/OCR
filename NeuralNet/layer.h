#ifndef LAYER_H
#define LAYER_H

#include "../Misc/matrix.h"
#include <math.h>

typedef struct Layer Layer;
struct Layer
{
	size_t nb_input;
	size_t nb_node;
	Matrix weights;
	Matrix bias;
};

Layer Init_layer(size_t nb_input, size_t nb_node);
Layer Define_layer(Matrix weights, Matrix biases);
void Update_layer(Layer *layer, Matrix inputs, Matrix delta, double lr);
void Free_layer(Layer layer);
void Print_layer(Layer layer);

#endif
