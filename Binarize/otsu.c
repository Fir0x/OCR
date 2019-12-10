#include "otsu.h"

int ApplyThreshold(SDL_Surface *image_surface,unsigned int treshold)
{
int output = 0;
//simple function that apply a treshold directly to a binarize image
for(int x = 0; x<image_surface->w;x++)
{
	for(int y = 0; y < image_surface->h;y++)
	{
		//take the pixel x/y  
		Uint32 pixel = get_pixel(image_surface, x, y);
		Uint8 r, g, b;
		//more especially his colors in 256 gray format
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		//set base color to white
		float average = 0;
		//if the color is under the treshold then set it to black
		if(r<=(treshold))
		{
			average = 1;
			output += 1;
		}
		//replace the pixel in the image;
		pixel= SDL_MapRGB(image_surface->format, average, average, average);
		put_pixel(image_surface, x, y, pixel);		
	}
}
return output;
}

void egalised(SDL_Surface *img_s)
/*
 * 	Histogram equalizer, it increases the contrast to facilitate the execution
 * 	of the Otsu method
*/
{
//initialising of the histogram
	double histo[256] = {0};
	int w = img_s->w;
	int h = img_s->h;
	double nb_pix = (double)(w*h);

//count the number of pixel of each rank in the image and make and histogram
//out of it
	for (int x =0; x<w ; x++)
	{
		for (int y = 0; y < h; y++)
		{
			Uint32 pixel = get_pixel(img_s, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img_s->format, &r, &g, &b);
			histo[r]++;
		}
	}

//equaliz the histogram
	unsigned long pix_cumul = 0;
	double mult = 255.0/nb_pix;
	for (int i = 0; i<256; i++)
	{
		pix_cumul += histo[i];
		histo[i] = (double)pix_cumul * mult;
	}

//put the equalized the histogramme in previous image
	for (int x =0; x<w ; x++)
	{
		for (int y = 0; y < h; y++)
		{
			Uint32 pixel = get_pixel(img_s, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img_s->format, &r, &g, &b);
			int average = (int)histo[r];
			pixel= SDL_MapRGB(img_s->format, average, average, average);
			put_pixel(img_s, x, y, pixel);
		}
	}
}

Matrix Image_to_Matrix_Otsu(SDL_Surface* img_s)
/*
 * 	This algorithm transform an image in a matrix of the same width and the
 * 	same height
*/
{
//initilising the output matrix
	Matrix output = Init_matrix(img_s->w,img_s->h);
	size_t w = img_s->w;
	size_t h = img_s->h;

//put each pixel in its representing place in the output matrix
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

SDL_Surface* Load_Image(char *path)
/*
 * 	load a image from the specified path
*/
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


void Otsu_better(SDL_Surface* img_s,SDL_Surface* img_s2)
/*
 * 	 binarized an image using the method of Otsu
*/
{
//Initialising of the different array representing the ranks of gray
	double histo[256] = {0};
	int threshold; /* threshold for binarization */
	int w = img_s->w;
	int h = img_s->h;
	double nb_pix = (double)(w*h);

	double histo2[256] = {0};
	int threshold2; /* threshold for binarization */
	
	double proba[256] = {0}, omega[256] = {0}; /* prob of graylevels */
	double mean[256] = {0};   /* mean value for separation */
	double max_sigma, sigma[256] = {0}; /* inter-class variance */

	double proba2[256] = {0}, omega2[256] = {0};/* prob of graylevels */
	double mean2[256] = {0};   /* mean value for separation */
	double max_sigma2, sigma2[256] = {0}; /* inter-class variance */
	
	//make the histogram
	for (int x =0; x<w ; x++)
	{
		for (int y = 0; y < h; y++) 
		{
			//1
			Uint32 pixel = get_pixel(img_s, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img_s->format, &r, &g, &b);
			histo[r]++;
			//2
			pixel = get_pixel(img_s2, x, y);
			SDL_GetRGB(pixel, img_s2->format, &r, &g, &b);
			histo2[r]++;
			
		}
	}

	//make the probability array for each levels of color
	for (int i = 0; i < 256; i ++ ){proba[i] = histo[i] / nb_pix;}
	for (int i = 0; i < 256; i ++ ){proba2[i] = histo2[i] / nb_pix;}

	//make omega and mean
	omega[0] = proba[0];
	mean[0] = 0.0;
	omega2[0] = proba2[0];
	mean2[0] = 0.0;
	for (int i = 1; i < 256; i++)
	{
		omega[i] = omega[i-1] + proba[i];
		mean[i] = mean[i-1] + i*proba[i];
		omega2[i] = omega2[i-1] + proba2[i];
		mean2[i] = mean2[i-1] + i*proba2[i];
	}

	//finding maximum sigma and so the optimum threshold
	threshold = 0;
	max_sigma = 0.0;
	threshold2 = 0;
	max_sigma2 = 0.0;

	for (int i = 0; i < 256-1; i++)
	{
		if (omega[i] > 0.0 && omega[i] < 1.0)
		{
			sigma[i]=pow(mean[256-1]*omega[i]-mean[i],2)/(omega[i]*
					(1.0-omega[i]));
		}
		else
		{
			sigma[i] = 0.0;
			if (omega2[i] > 0.0 && omega2[i] < 1.0)
			{
			sigma2[i]=pow(mean2[256-1]*omega2[i]-mean2[i],2)/(omega2[i]*
					(1.0-omega2[i]));
			}
			else {sigma2[i] = 0.0;}
			if (sigma[i] > max_sigma)
			{
			max_sigma = sigma[i];
			threshold = i;
			}
			if (sigma2[i] > max_sigma2)
			{
			max_sigma2 = sigma2[i];
			threshold2 = i;
			}
		}
		//reset the 2nd image like the 1st one
		img_s2 = img_s;
		//if best1 > 0.25 of the 1st image
		//it's certainly better to use the egalised one
		size_t best1 = (size_t)ApplyThreshold(img_s,threshold);
		if(best1 > (0.25*img_s->w*img_s->h))
		{
			best1 = ApplyThreshold(img_s,threshold2);
			img_s = img_s2;
		}
	}
}

Matrix otsu(char *image_filename)
{
SDL_Surface* img_s = Load_Image(image_filename);
SDL_Surface* img_s2 = Load_Image(image_filename);

//transform the images in grayscale 256
	for(int x = 0; x<img_s->w;x++)
	{
		for(int y = 0; y < img_s->h;y++)
		{
			Uint32 pixel = get_pixel(img_s, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img_s->format, &r, &g, &b);
			float  average = 0.3*r + 0.59*g + 0.11*b ;
			pixel = SDL_MapRGB(img_s->format, average, average, average);
			put_pixel(img_s, x, y, pixel);
			put_pixel(img_s2, x, y, pixel);
		}
	}

//egalised the second one
	egalised(img_s2);
//find the best one between the basic and the egalized one
	Otsu_better(img_s,img_s2);
//freedoooooooooooooooooom
	SDL_FreeSurface(img_s2);
//pass the image in matrix
	Matrix img = Image_to_Matrix_Otsu(img_s);
//freedoooooooooooooooooom
	SDL_FreeSurface(img_s);

	return img;
}
