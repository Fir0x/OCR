#include "sauvola.h"

Matrix img2Mat(SDL_Surface* img_s)
{
	Matrix output = Init_matrix(img_s->w,img_s->h);
	size_t w = img_s->w;
	size_t h = img_s->h;

//transform the image in grayscale
	for(size_t x = 0; x<w;x++)
	{
		for(size_t y = 0; y < h;y++)
		{
			Uint32 pixel = get_pixel(img_s, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img_s->format, &r, &g, &b);
			Put_nb(&output, (double)r,x,y);
		}
	}

	return output;
}

/*
 * 	use for Sauvola to normalised x and y before asking for a number in
 * 	the matrix
*/
double get_pix(int x, int y, size_t W, size_t H, Matrix img_as_mat)
{
	if(y<0){y=0;}
	if(y>=(int)H){y=(int)H-1;}
	if(x<0){x=0;}
	if(x>=(int)W){x=(int)W-1;}
	return Get_nb(img_as_mat, x, y);
}

Matrix Sauvola_transform(SDL_Surface* img_s)
/* w => size of the window center around the pixel
 * k => difined by user, 
 * 0.34 and 14 are the best compromise
 * */
{
//initializing of the matrix representing the image
	size_t w = 14;
	double k = 0.34;
	Matrix img_as_mat = img2Mat(img_s);
	int F = ((int)(w) -1)/2;

//initializing of diverse parameters such as the width or the height of
//the image
	size_t Width = img_s->w;
	size_t Height = img_s->h;

		//----- Calculus of the averages -----//

	Matrix Mean = Init_matrix(img_s->w,img_s->h);
	Matrix Var = Init_matrix(img_s->w,img_s->h);
	int x_img = -F-1;
	int y_img = -F;
	int right = 1;

//variable representing the window, it's only use to find the sum of the
//elements within so I use 'mean' to optimised my calculus
	double mean = 0;
//initialising 'mean'
	for(int x = -(int)(F)-1;x<(int)(F)-1;x+=1)
	{
		for(int y = -(int)(F);y<(int)(F);y+=1)
		{
			mean += get_pix(x,y,Width,Height,img_as_mat);
		}
	}
//calculus of the local average for each pixel
	while(y_img<(int)(Height-F))
	{
//the window goes from right to left, move down one time and then go in 
//the opposite way.
		if(right)
		{
			x_img+=1;
			if(x_img>=(int)(Width-F))
			{
				right = 0;
				x_img -= 1;
				y_img += 1;
				if(y_img<(int)(Height-F))
				{
					for(int k = 0; k<2*F; k+=1)
					{
						int x = x_img+k ;
						mean -= get_pix(x,y_img,Width,Height,img_as_mat);
						mean += get_pix(x,y_img+2*F,Width,Height,img_as_mat);
					}
				}
			}
			else
			{
				for(int k = 0; k<2*F; k+=1)
				{
					int y = y_img+k ;
					mean -= get_pix(x_img,y,Width,Height,img_as_mat);
					mean += get_pix(x_img+2*F,y,Width,Height,img_as_mat);
				}
			}
			
		}
		else
		{
			x_img-=1;
			if(x_img<-(int)F)
			{
				right = 1;
				x_img += 1;
				y_img += 1;
				if(y_img<(int)(Height-F))
				{
					for(int k = 0; k<2*F; k+=1)
					{
						int x = x_img+k ;
						mean -= get_pix(x,y_img,Width,Height,img_as_mat);
						mean += get_pix(x,y_img+2*F,Width,Height,img_as_mat);
					}
				}
			}
			else
			{
				for(int k = 0; k<2*F; k+=1)
				{
					int y = y_img+k ;
					mean += get_pix(x_img,y,Width,Height,img_as_mat);
					mean -= get_pix(x_img+2*F,y,Width,Height,img_as_mat);
				}
			}
			
		}
		if(y_img+F<(int)(Height))
		{
			Put_nb(&Mean,(mean/(w*w)),x_img+F,y_img+F);
		}
		
	}

		//----- Calculus of the variances -----//


	x_img = -F;
	y_img = -F;
	right = 1;
	mean = 0;
	double img_pix = 0;

//same as for the average, 'var' represent the sum of all the variances 
//inside the window, I use it ro simplify my calculus
	double var = 0;
//initilisation of the window's sum
	for(int x = -(int)(F)-1;x<(int)(F)-1;x+=1)
	{
		for(int y = -(int)(F);y<F;y+=1)
		{
			img_pix = get_pix(x,y,Width,Height,img_as_mat);
			mean = get_pix(x,y,Width,Height,Mean);
			var += (img_pix*img_pix);
			var -= (mean*mean);
		}
	}


//calculus of the local variance for each pixel
	while(y_img<(int)(Height-F))
	{
//the window goes from right to left, move down one time and then go in 
//the opposite way.
		if(right)
		{
			x_img+=1;
			if(x_img>=(int)(Width-F))
			{
				right = 0;
				x_img -= 1;
				y_img += 1;
				if(y_img<(int)(Height-F))
				{
					for(int k = 0; k<2*F; k+=1)
					{
						int x = x_img+k;

//after each move remove the old line/column and add the new one
						img_pix = get_pix(x,y_img,Width,Height,img_as_mat);
						mean = get_pix(x,y_img,Width,Height,Mean);
						var -= (img_pix*img_pix);
						var += (mean*mean);
						img_pix = get_pix(x,y_img+2*F,Width,Height,img_as_mat);
						mean = get_pix(x,y_img+2*F,Width,Height,Mean);
						var += (img_pix*img_pix);
						var -= (mean*mean);
					}
				}
			}
			else
			{
				for(int k = 0; k<2*F; k+=1)
				{
					int y = y_img+k ;

//here come the others cases
					img_pix = get_pix(x_img,y,Width,Height,img_as_mat);
					mean = get_pix(x_img,y,Width,Height,Mean);
					var -= (img_pix*img_pix);
					var += (mean*mean);
					img_pix = get_pix(x_img+2*F,y,Width,Height,img_as_mat);
					mean = get_pix(x_img+2*F,y,Width,Height,Mean);
					var += (img_pix*img_pix);
					var -= (mean*mean);
				}
			}
		}
		else
		{
			x_img-=1;
			if(x_img<-(int)F)
			{
				right = 1;
				x_img += 1;
				y_img += 1;
				if(y_img<(int)(Height-F))
				{
					for(int k = 0; k<2*F; k+=1)
					{
						int x = x_img+k;

						img_pix = get_pix(x,y_img,Width,Height,img_as_mat);
						mean = get_pix(x,y_img,Width,Height,Mean);
						var += (img_pix*img_pix);
						var -= (mean*mean);
						img_pix = get_pix(x,y_img+2*F,Width,Height,img_as_mat);
						mean = get_pix(x,y_img+2*F,Width,Height,Mean);
						var -= (img_pix*img_pix);
						var += (mean*mean);
					}
				}
			}
			else
			{
				for(int k = 0; k<2*F; k+=1)
				{
					int y = y_img+k ;

					img_pix = get_pix(x_img,y,Width,Height,img_as_mat);
					mean = get_pix(x_img,y,Width,Height,Mean);
					var += (img_pix*img_pix);
					var -= (mean*mean);
					img_pix = get_pix(x_img+2*F,y,Width,Height,img_as_mat);
					mean = get_pix(x_img+2*F,y,Width,Height,Mean);
					var -= (img_pix*img_pix);
					var += (mean*mean);
				}
			}
		}
		if(y_img<(int)(Height)-F)
		{
			Put_nb(&Var,(var/(w*w)),x_img+F,y_img+F);
		}
		
	}


		//----- Calculus of the standart deviations-----//

//now the locals variances are calculated we can use them to
//find the locals standart deviations
	Matrix E_type = Init_matrix(img_s->w,img_s->h);
	for(size_t i = 0; i<Width; i+=1)
	{
		for(size_t j = 0; j<Height; j+=1)
		{
			Put_nb(&E_type,sqrt(Get_nb(Var,i,j)),i,j);
		}
	}

		//----- Calculus of the tresholds -----//

//apply of the formula on which the Sauvol algorithm is base
	for(size_t i = 0; i<Width; i+=1)
	{
		for(size_t j = 0; j<Height; j+=1)
		{
			double seuil = (Get_nb(Mean, i, j)*(1 + k*((Get_nb(E_type, i, j)
								/256)-1)));
			Put_nb(&Var,seuil,i,j);
		}
	}
	free(img_as_mat.data);
	img_as_mat = img2Mat(img_s);

		//----- Apply of tresholds -----//

//simple apply of the local threshold on each pixel
	for(size_t i = 0; i<Width; i+=1)
	{
		for(size_t j = 0; j<Height; j+=1)
		{
			if(Get_nb(img_as_mat,i,j)<=Get_nb(Var,i,j))
			{ 
				Put_nb(&img_as_mat,0,i,j);
			}
			else
			{
				Put_nb(&img_as_mat,1,i,j);
			}
		}
	}

//Freedoooooooooom
	free(Var.data);
	free(E_type.data);
	free(Mean.data);
	return img_as_mat;
}


SDL_Surface* Load_Image(char *path)
{
	SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

Matrix Sauvola(char *image_filename)
// hat function to call Sauvola_transform which apply the sauvola method
{
	SDL_Surface* image_surface = Load_Image(image_filename);
	return Sauvola_transform(image_surface);
}
