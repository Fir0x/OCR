//
//  Liste.h
//  SEGMENTATION
//
//  Created by William Johnson on 20/10/2019.
//  Copyright © 2019 William Johnson. All rights reserved.
//

#ifndef Liste_h
#define Liste_h
#include "../Misc/matrix.h"
#include <stdio.h>
#include <err.h>
typedef struct MatCoor MatCoor;
struct MatCoor
{
    size_t x;
    size_t y;
    Matrix mat;
    
    MatCoor *next;
};

typedef struct Liste Liste;
struct Liste
{
    MatCoor  *first;
    size_t len;
};

MatCoor init(Matrix m, size_t x, size_t y);
Liste initialization(Matrix mat, size_t x, size_t y);
void insert(Liste *liste, MatCoor mat);
void printListe(Liste *liste);
void printListeTaille(Liste *liste);
MatCoor Get_elm(Liste *liste,size_t i );
void liberty(Liste *l);
#endif /* Liste_h */
