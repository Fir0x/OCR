#ifndef TRAIN_H
#define TRAIN_H

#include <math.h>
#include "layer.h"
#include "../Misc/matrix.h"
#include "../Misc/list.h"

Matrix Sigmoid(Matrix x);
Matrix Softmax(Matrix x);
Matrix Sigmoid_dv(Matrix x);
Matrix Softmax_dv(Matrix x);
Matrix Activ(Layer layer, Matrix input, Matrix (*activ_func)(Matrix));
Matrix Delta(Matrix error, Matrix prediction, Matrix (*activ_dv)(Matrix));
void Backprop(Matrix *d_hidden, Matrix *d_out, size_t nb_hlayer,
		Matrix *hidden_out, Matrix expected, Matrix predicted,
		Matrix output_weights, Layer *h_layers);
void Train(Layer *h_layers, Layer *o_layer, size_t nb_hlayer, Liste dataset,
		Matrix expected, size_t epochs, double lr);

#endif
