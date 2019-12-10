#ifndef SAVE_NET_H
#define SAVE_NET_H

#include <stdlib.h>
#include "neural_net.h"
#include <err.h>

char* Serialize_layer(Layer layer);
Layer Deserialize_layer(char *str);
void Write_layer(Layer layer, FILE *file);
void Save_net(Net net, char filename[]);
Net Load_net(char filename[]);

#endif
