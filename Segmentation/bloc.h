//
//  bloc.h
//  SEGMENTATION
//
//  Created by William Johnson on 19/10/2019.
//  Copyright © 2019 William Johnson. All rights reserved.
//

#ifndef bloc_h
#define bloc_h
#include "../Misc/matrix.h"
#include "../Misc/list.h"
#include <stdlib.h>
#include <stdio.h>

void colorv(Matrix mat, size_t mid,size_t st);
void colorh(Matrix mat, size_t mid,size_t st);
void splith(Matrix mat);
void splitv(Matrix mat);
void splitv2(Matrix mat);
Matrix trace(Matrix mat,size_t x,size_t y);
Matrix copy(Matrix mat,Matrix m2, size_t x, size_t y);
Liste find(Matrix mat/*,size_t  w2, size_t h2*/);
Matrix redim(Matrix mat, size_t w, size_t h);
//void save(char filename[]);
#endif /* bloc_h */
