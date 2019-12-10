#ifndef SAUVOLA_H_
#define SAUVOLA_H_

#include <stdlib.h>
#include <SDL/SDL.h>
#include "../Misc/matrix.h"
#include <math.h>
#include "pixel_operations.h"
#include "SDL/SDL_image.h"

Matrix Sauvola(char *image_filename);
Matrix Sauvola_transform(SDL_Surface* img_s);

#endif
