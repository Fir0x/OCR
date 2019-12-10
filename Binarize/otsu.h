#ifndef OTSU_H
#define OTSU_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "math.h"
#include "../Binarize/pixel_operations.h"
#include "../Misc/matrix.h"
#include <err.h>


Matrix otsu(char *image_filename);

#endif
