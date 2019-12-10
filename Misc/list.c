//
//  Liste.c
//  SEGMENTATION
//
//  Created by William Johnson on 20/10/2019.
//  Copyright © 2019 William Johnson. All rights reserved.
//

#include "list.h"
//#include "bloc.h"

/*typedef struct MatCoor MatCoor;
struct MatCoor
{
    size_t x;
    size_t y;
    Matrix mat;
    MatCoor *next;
};*/

MatCoor init(Matrix m, size_t x, size_t y)
{
    MatCoor mat;
    mat.mat = m;
    mat.x = x;
    mat.y = y;
   // MatCoor *next;
    return mat;
}



Liste initialization(Matrix mat, size_t x, size_t y)
{
    Liste liste;
    MatCoor *element = malloc(sizeof(MatCoor));

    if (element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->x = x;
    element->y = y;
    element->mat=mat;
    element->next = NULL;
    liste.first = element;
    liste.len =1;

    return liste;
}

void insert(Liste *liste, MatCoor mat)
{
    /* Création du nouvel élément */
    MatCoor *nouveau = malloc(sizeof(MatCoor));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->x = mat.x;
    nouveau->y = mat.y;
    nouveau->mat = mat.mat;
    MatCoor *actuel = liste->first;
     while (actuel->next != NULL)
    {
        
        actuel = actuel->next;
    }

    /* Insertion de l'élément au début de la liste */
    nouveau->next = actuel->next;
    actuel->next = nouveau;
    liste->len+=1;
}

void printListe(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MatCoor *actuel = liste->first;

    while (actuel != NULL)
    {
        printf("(%ld,%ld) -> ", actuel->x,actuel->y);
        actuel = actuel->next;
    }
    printf("NULL\n");
}

void printListeTaille(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MatCoor *actuel = liste->first;

     while (actuel != NULL)
    {
        printf("(%ld,%ld) -> \n", actuel->mat.width,actuel->mat.height);
        actuel = actuel->next;
    }
    printf("NULL\n");
}

MatCoor Get_elm(Liste *liste,size_t i )
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
	if (i >= liste->len)
		errx(1,
		"Error in Get_elm (Liste.c): i >= len, i = %zi len = %zi.\n", i,
		liste->len);

    MatCoor *actuel = liste->first;
	size_t x = 0;
     while (actuel != NULL && x != i)
    {
        
        actuel = actuel->next;
        x++;
    }
    return *actuel;
}
void liberty_rec(MatCoor *m)
{
	if (m != NULL)
	{
		liberty_rec(m->next);
		free(m->mat.data);
		free(m->next);
	}
}

void liberty(Liste *l)
{
	/*if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }

    MatCoor *actuel = l->first;
	MatCoor *lib;
	//free(l->first);
	//free(l);
     while (actuel != NULL)
    {
        lib = actuel;
        actuel = actuel->next;
        free(lib->mat.data);
        if (lib->next != NULL)
			free(lib->next);
    }*/
    liberty_rec(l->first);
		
   /* for(size_t i = l->len-1; i>=1 ;i--){
		free(Get_elm(l,i).mat.data);
    }
    free(l->first);
    free(l);*/
    
}
