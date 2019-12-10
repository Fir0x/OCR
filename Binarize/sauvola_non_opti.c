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


Matrix Sauvola_transform(SDL_Surface* img_s)
/* w => size of the square window center around the pixel
 * k => defined in advance, is use in the calculus of the treshold
 * 0.34 and 14 are respectively the best compromises
 * */
{
//initializing of the matrix representing the image
	size_t w = 14;
	double k = 0.34;
	Matrix img_as_mat = img2Mat(img_s);
	size_t F = (w -1)/2;

//initializing of diverse parameters such as the width or the height of
//the image

//making a bigger image to facilitate the passing of the window
	size_t Width = img_s->w;
	size_t Height = img_s->h;
	Matrix Col_1 = Init_matrix(Width,1);
	Matrix Col_2 = Init_matrix(Width,1);
	for(size_t i = 0; i<Width; i+=1)
	{
		Put_nb(&Col_1,Get_nb(img_as_mat,i,0),i,0);
		Put_nb(&Col_2,Get_nb(img_as_mat,i,Height-1),i,0);
	}

//First augmenting the height
	Matrix img_buff = Init_matrix(Width+2*F,Height + 2*F);
	for(size_t i = 0;i<Width;i+=1)
	{
		double df = Get_nb(Col_1,i,0);
		double dd = Get_nb(Col_2,i,0);
		for(size_t j = 0; j<F; j+=1)
		{
			Put_nb(&img_buff,df,F+i,j);
			Put_nb(&img_buff,dd,F+i,j+F+Height);
		}
	}

//Freedoooooooooom
	free(Col_1.data);
	free(Col_2.data);
	Col_1 = Init_matrix(1,Height);
	Col_2 = Init_matrix(1,Height);

//Second augmenting the width
	for(size_t i = 0; i<Height; i+=1)
	{
		Put_nb(&Col_1,Get_nb(img_as_mat,0,i),0,i);
		Put_nb(&Col_2,Get_nb(img_as_mat,Width-1,i),0,i);
	}

	for(size_t i = F;i<Height;i+=1)
	{
		double df = Get_nb(Col_1,0,i);
		double dd = Get_nb(Col_2,0,i);
		for(size_t j = 0; j<F; j+=1)
		{
			Put_nb(&img_buff,df,j,i);
			Put_nb(&img_buff,dd,j+F+Width,i);	
		}
	}

//Freedoooooooooom
	free((Col_2).data);
	free((Col_1).data);

//Finaly coping the content of img_as_mat in img_buff
	for(size_t i = F; i<Width+F-1; i+=1)
	{
		for(size_t j = F; j<Height+F; j+=1)
		{
			Put_nb(&img_buff,Get_nb(img_as_mat,i-F,j-F),i-1,j-1);
		}
	}

//Freedoooooooooom
	free(img_as_mat.data);
	img_as_mat = img_buff;


		//----- Calculus of the averages -----//

	Matrix Mean = Init_matrix(img_s->w,img_s->h);
	Matrix Var = Init_matrix(img_s->w,img_s->h);

//simple calculus of the local average for each pixel
	for(size_t i = F; i<Width+F; i+=1)
	{
		for(size_t j = F; j<Height+F; j+=1)
		{
			double moy = 0;
			for(size_t x = 0; x<2*F; x++ )
			{
				for(size_t y = 0; y<2*F; y++)
				{
					moy += Get_nb(img_as_mat, i+x-F, j+y-F);
				}
			}
		moy = moy/(w*w);
		Put_nb(&Mean,moy,i-F,j-F);
		}
	}


		//----- Calculus of the variances -----//

//calculus of the local variance for each pixel
	for(size_t i = 0; i<Width; i+=1)
	{
		for(size_t j = 0; j<Height; j+=1)
		{
			double var = 0;
			for( size_t x = 0; x<2*F; x++ )
			{
				for( size_t y = 0; y<2*F; y++)
				{
					double buf = Get_nb(img_as_mat, i+x, j+y);
					int x_mean = i+x-F;
					int y_mean = j+y-F;
					if(y_mean<0){y_mean=0;}
					if(y_mean>=(int)Height){y_mean=Height-1;}
					if(x_mean<0){x_mean=0;}
					if(x_mean>=(int)Width){x_mean=Width-1;}
					double moy = Get_nb(Mean, x_mean, y_mean);
					var += (buf * buf) - (moy * moy);
				}
			}
			var = var/(w*w);
			Put_nb(&Var,var,i,j);
		}
	}

		//----- Calculus of the standart deviations-----//

//using of the locals variances to find the locals standart deviations
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
