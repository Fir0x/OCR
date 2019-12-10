
#include "bloc.h"


void colorv(Matrix mat, size_t mid,size_t st) //Color the vertical line starting at (st,mid)
{    
	size_t height = mat.height;
    size_t y = st;
    
    while(y < height && Get_nb(mat,mid,y) != 0 && Get_nb(mat,mid,y) != 3  /* && Get_nb(mat,mid,y) != 3*//* && y != st)*/  )
    {
		
		Put_nb(&mat,3,mid,y);
			
		y++;
        
    }
    
}

void colorh(Matrix mat, size_t mid,size_t st) //Color the line starting at (mid,st)
{
    size_t width = mat.width;
    size_t x = st;
    
  
    while(x < width && Get_nb(mat,x,mid) != 0/* && Get_nb(mat,x,mid) != 3*/)
    
    {
        Put_nb(&mat,3,x,mid);
        x++;
    }
}

void splith(Matrix mat) //course the matrix and insert call colorh when he can draw a line
{
    size_t width = mat.width;
    size_t height = mat.height;
    int boo = 0;
    size_t compt = 0;
    int good =0;
    for(size_t i = 0; i < height ; i++)
    {
			boo=0;
			compt =0;
			good = 0;
            for(size_t j = 0; j < width  ; j++)
            {
                    if (Get_nb(mat,j,i) == 0)
                    {
                        boo = 1;
                        compt = 0;
                        good = 0;
                        //break;
                    }
                    if ( (Get_nb(mat,j,i) == 3 || j == width -1)
                        && boo== 0 && good != 0)
                        {
                            colorh(mat, i,compt);
                        }
                if (Get_nb(mat,j,i) == 3)
                {
                    boo=0;
                    compt = j;
                    good = 0;
                }
                good = 1;
            }
    }
}


void splitv(Matrix mat)//course the matrix and insert call colorv when he can draw a vertical line
{
    size_t width = mat.width;
    size_t height = mat.height;
    size_t start = 0;
    int boo = 0;
    int good = 0;
    for(size_t i = 0; i < width ; i++ )
    {
			boo=0;
			start = 0;
			good = 0;
			
            for(size_t j = 0; j < height  ; j++)
            {
				if (Get_nb(mat,i,j) == 1 && start == 0)
                    {
                       start = j;
                        
                    }
                    if (Get_nb(mat,i,j) == 0)
                    {
                        boo = 1;
                        start = 0;
                        good = 0;
                        
                    }
                    
                    if ( (Get_nb(mat,i,j) == 3 || j== height -1)
                        && boo== 0 && good != 0)
                    {
                        colorv(mat, i,start);
                    }
                    if (Get_nb(mat,i,j) == 3)
                    {
                        //start = j;
                        boo = 0;
                        good = 0;
                    }
                good = 1;
            }
    }
}

void splitv2(Matrix mat)//course the matrix and insert call colorv when he can draw a vertical line
{
    size_t width = mat.width;
    size_t height = mat.height;
    size_t start = 0;
    int ok = 0;
    
    for(size_t i = 0; i < height ; i++ )
    {
			
			start = 0;
			
			
            for(size_t j = 0; j < width && Get_nb(mat,j,i) != 3 ; j++)
            {	
				size_t compt = i;
				//if (Get_nb(mat,j,i) == 1)
                   // {
                        start = i;
                        
                   // }
                ok = 0;
            
				while(Get_nb(mat,j,compt) != 3 && Get_nb(mat,j,compt) !=   0)
				{
                   
                    
                    if  (Get_nb(mat,j,compt+1) == 3) //|| compt== height -1 )
                       
                    {
						
                        ok = 1;
                    }
                    compt++;
                    
                    
				} 
				if(ok==1)
				{
					
					colorv(mat, j,start);
				}
                            
            }
    
	}
}




Matrix trace(Matrix mat,size_t x,size_t y)//copy the size of the matrix and init it(pas utiliser)
{
    size_t width = mat.width;
    size_t height = mat.height;
    size_t i = 0;
    size_t j = 0;
    int l = 0;
    size_t j2 = 0;
    while (i+y < height && Get_nb(mat, j+x, i+y) != 3)
    {
        while (j+x < width && Get_nb(mat, j+x, i+y) != 3)
        {
            j++;
        }
        if (l == 0) {
            j2 = j;
            l =1;
        }
        j=0;
        i++;
    }
    if (i+y >= height) {
        i--;
    }
    if (j+x >= width) {
        j--;
    }
    return Init_matrix(j2, i);
}

Matrix copy(Matrix mat,Matrix m2, size_t x, size_t y)//copy mat -> mat2
{
    for (size_t i = 0; i < m2.height; i++) {
        for (size_t j = 0; j < m2.width; j++) {
			if(Get_nb(mat, x+j, y+i) == 0)
				Put_nb(&m2, 1, j, i);
			if(Get_nb(mat, x+j, y+i) == 1)
				Put_nb(&m2, 0, j, i);
        }
    }
    return m2;
}

Liste find(Matrix mat/*, size_t w2, size_t h2*/)//course the matrix and put in a list all the bloc/carac
{   int first = 0;
    Liste new;
   // int comp = 0;
   // FILE* file = fopen("test.txt","w");
    
    for (size_t y = 0; y < mat.height; y++) {
		//comp = 0;
		//fputc('\n',file);
		
        for (size_t x = 0; x < mat.width; x++) {
			
            if (x == 0 && y == 0 && Get_nb(mat, x, y) != 3)  {
                if (first == 1) {
                    Matrix bloc2 =
                    copy(mat,trace(mat,x,y), x, y);
                    MatCoor bloc2coo = init(bloc2, x, y);
                    insert(&new, bloc2coo);
                  //  fputc('o',file);
                  //  comp = 0;
                }
                if (first == 0) {
                    Matrix bloc = copy(mat,trace(mat,x,y), x, y);
                    new = initialization(bloc, x,y );
                    first = 1;
                  //  fputc('o',file);
                 //   comp = 0;
                }
            }
            if(x>0){
            if (y == 0 && Get_nb(mat, x-1, y) == 3
                && Get_nb(mat, x, y) != 3 ) {
                if (first == 1) {
                    Matrix bloc2 = copy(mat,trace(mat,x,y) , x, y);
                    MatCoor bloc2coo = init(bloc2, x, y);
                    insert(&new, bloc2coo);
                   // fputc('o',file);
                   // comp = 0;
                }
                if (first == 0) {
                    Matrix bloc = copy(mat, trace(mat,x,y), x, y);
                    new = initialization(bloc, x,y );
                    first = 1;
                  //  fputc('o',file);
                  //  comp = 0;
                }
            }
		}
		if(y>0){
            if (x == 0 && Get_nb(mat, x, y-1) == 3
                && Get_nb(mat, x,y ) != 3) {
                if (first == 1) {
                    Matrix bloc2 = copy(mat,trace(mat,x,y) , x, y);
                    MatCoor bloc2coo = init(bloc2, x, y);
                    insert(&new, bloc2coo);
                  //  fputc('o',file);
                  //  comp = 0;
                }
                if (first == 0) {
                    Matrix bloc = copy(mat, trace(mat,x,y), x, y);
                    new = initialization(bloc, x,y );
                    first = 1;
                   // fputc('o',file);
                   // comp = 0;
                }
            }
		}
		if(x>0 && y>0){
            if ( Get_nb(mat, x, y-1) == 3 && Get_nb(mat, x-1, y) == 3
                && Get_nb(mat, x, y) != 3 && x != 0 && y != 0) {
                if (first == 1) {
                    Matrix bloc2 = copy(mat,trace(mat,x,y) , x, y);
                    MatCoor bloc2coo = init(bloc2, x, y);
                    insert(&new, bloc2coo);
                   // fputc('o',file);
                  //  comp = 0;
                }
                if (first == 0) {
                    Matrix bloc = copy(mat,trace(mat,x,y), x, y);
                    new = initialization(bloc, x,y );
                    first = 1;
                   // fputc('o',file);
                    //comp = 0;
                }
            }
		}
		//if(Get_nb(mat, x, y) == 3 && first ==1)
		//{comp+=1;}
		//if(comp %850== 0)
		//{fputc(' ',file);}
        }
    }
    //fclose(file);
    return new;
}

Matrix redim(Matrix mat, size_t w, size_t h)
{
    size_t coefw = mat.width / w;
    size_t coefh = mat.height/h;
    Matrix re = Init_matrix(w, h);
    size_t i = 0;
    size_t j = 0;
    if (coefw == 0) {
        coefw =1;
    }
    if (coefh == 0) {
        coefh = 1;
    }
    for (size_t y = 0; y<h && y< mat.height; y++) {
        i = 0;
        for (size_t x =0; x<w && x< mat.width; x++) {
            Put_nb(&re, Get_nb(mat, i, j), x, y);
            i+=coefw;
        }
        j+=coefh;
    }
    return re;
    

}
/*void save(char filename[])
{
		SDL_Init(SDL_INIT_VIDEO);
	 IMG_Init(~0);
	 SDL_Surface* image_surface;
		
		

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    image_surface = IMG_Load(filename);
	Matrix x = img2Mat(image_surface);
	IMG_SavePNG(image_surface, "return1.png");
    splith(x);
    
    splitv2(x);
    
	image_surface = mat2img(x,image_surface);
    IMG_SavePNG(image_surface, "return2.png");
}*/

//int main()
//{
	
    /*Matrix mat = Init_matrix(5,4);
     Put_nb(&mat,1,0,1);
     Put_nb(&mat,1,0,2);
    // Put_nb(&mat,1,0,4);
     Put_nb(&mat,1,0,3);
    Put_nb(&mat,1,0,0);
    
     
     
    // splitv(mat);
     splith(mat);
     splitv(mat);
    // splith(mat);
    // splitv(mat);
    // splith(mat);
     Print_matrix(mat);
     
     
    //Matrix mot = trace(mat, 1, 0);
     //Print_matrix(mot);
     //copy(mat, mot, 1, 0);
     //Print_matrix(mot);
     
     Liste list = find(mat);
     printListe(&list);*/
   /* Matrix mat = Init_matrix(10,10);
     Put_nb(&mat,1,0,0);
    Put_nb(&mat,1,1,0);
    Put_nb(&mat,1,9,0);
    Put_nb(&mat,1,8,0);
    
    Put_nb(&mat,1,0,1);
    Put_nb(&mat,1,2,1);
    Put_nb(&mat,1,7,1);
    Put_nb(&mat,1,8,1);
    Put_nb(&mat,1,9,1);
    Put_nb(&mat,1,1,1);
    
    Put_nb(&mat,1,1,2);
    Put_nb(&mat,1,2,2);
    Put_nb(&mat,1,3,2);
    Put_nb(&mat,1,6,2);
    Put_nb(&mat,1,7,2);
    Put_nb(&mat,1,8,2);
    
    Put_nb(&mat,1,2,3);
    Put_nb(&mat,1,3,3);
    Put_nb(&mat,1,6,3);
    Put_nb(&mat,1,7,3);
    
    Put_nb(&mat,1,4,4);
    Put_nb(&mat,1,5,4);
    
    Put_nb(&mat,1,4,5);
    Put_nb(&mat,1,5,5);
    
    Put_nb(&mat,1,2,6);
    Put_nb(&mat,1,3,6);
    Put_nb(&mat,1,6,6);
    Put_nb(&mat,1,7,6);
    
    Put_nb(&mat,1,1,7);
    Put_nb(&mat,1,2,7);
    Put_nb(&mat,1,3,7);
    Put_nb(&mat,1,6,7);
    Put_nb(&mat,1,7,7);
    Put_nb(&mat,1,8,7);
    
    Put_nb(&mat,1,0,8);
    Put_nb(&mat,1,1,8);
    Put_nb(&mat,1,2,8);
    Put_nb(&mat,1,7,8);
    Put_nb(&mat,1,8,8);
    Put_nb(&mat,1,9,8);
    
    Put_nb(&mat,1,0,9);
    Put_nb(&mat,1,1,9);
    Put_nb(&mat,1,8,9);
    Put_nb(&mat,1,9,9);*/
     
   /* Print_matrix(mat,1);
     
    
    Matrix x = redim(mat, 20, 20);
    Print_matrix(x, 1);*/
    
	

	

	
     
    
//}
