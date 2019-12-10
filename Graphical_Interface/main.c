#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include "../Binarize/sauvola.h"
#include "../Misc/list.h"
#include "../Segmentation/bloc.h"
#include "../NeuralNet/save_net.h"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *window_show;
GtkWidget *grid1;
GtkWidget *Openfile;
GtkWidget *selectedfile;
GtkWidget *setting_window;
GtkWidget *separator1;
GtkWidget *layout;
GtkWidget *result;
GtkWidget *start_button;
GtkWidget *Entrypath;
GtkWidget *Entrypath2;
GtkWidget *Showdoc;
GtkWidget *imagedoc;
GtkWidget *Openfile2;
GtkWidget *Exit;
GtkWidget *Exit2;
GtkWidget *image;
gchar *filename;
gchar *foldername;

void on_exit2()
{
	gtk_widget_destroy(window_show);
}

void on_show_image()
{
  gchar *extention = ".png";
  gchar *extention2 = ".jpg";
  size_t j = 0;
  size_t b = 1;
  if(filename != NULL){
  for(size_t i = strlen(filename)-4; i < strlen(filename); i++)
  {
    if(extention[j] != filename[i])
      b = 0;
    j++;
  }
  if(b == 0)
  {
    j = 0;
    b = 1;
    for(size_t i = strlen(filename)-4; i < strlen(filename); i++)
    {
      if(extention2[j] != filename[i])
        b = 0;
      j++;
    }
  }
  if(b == 0)
  {
    gtk_label_set_text(GTK_LABEL(result), "result :\nFail (invalid arguments)");
  }
  else
  {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(~0);
    SDL_Surface* image_surface;
    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    image_surface = IMG_Load(filename);
    SDL_SaveBMP(image_surface, "result2.bmp");
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "main_window.glade", NULL);
    window_show = GTK_WIDGET(gtk_builder_get_object(builder, "Window_document"));
    Exit2 = GTK_WIDGET(gtk_builder_get_object(builder, "Exit2"));
    g_signal_connect(G_OBJECT(Exit2), "clicked", G_CALLBACK(on_exit2), NULL);
    gtk_widget_show_all(window_show);
  }
  }

}

void newfileset(GtkFileChooserButton *b)
{
  filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(b));
  if(filename != NULL)
    gtk_entry_set_text(GTK_ENTRY(Entrypath), filename);
}

void pathset(GtkFileChooserButton *b)
{
  foldername = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(b));
  if(foldername != NULL)
    gtk_entry_set_text(GTK_ENTRY(Entrypath2), foldername);
}

void pathchanged(GtkEntry *entry)
{
  filename = gtk_entry_get_text(GTK_ENTRY(entry));
  if(filename[strlen(filename)-1] != '/')
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(Openfile), filename);
}

void pathchanged2(GtkEntry *entry)
{
  foldername = gtk_entry_get_text(GTK_ENTRY(entry));
  if(foldername[strlen(foldername)-1] != '/')
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(Openfile2), foldername);
}

void on_start()
{
	if(filename == NULL || foldername == NULL)
	{
		gtk_label_set_text(GTK_LABEL(result), "result :\nFail (invalid arguments)");
	}
	else
	{
    	gchar *extention = ".png";
    	gchar *extention2 = ".jpg";
    	size_t j = 0;
    	size_t b = 1;
    	for(size_t i = strlen(filename)-4; i < strlen(filename); i++)
    	{
			if(extention[j] != filename[i])
				b = 0;
			j++;
		}
		if(b == 0)
		{
			j = 0;
			b = 1;
			for(size_t i = strlen(filename)-4; i < strlen(filename); i++)
			{
				if(extention2[j] != filename[i])
					b = 0;
				j++;
			}
		}
		if(b == 0)
		{
			gtk_label_set_text(GTK_LABEL(result), "result :\nFail (invalid arguments)");
		}
		else
		{
			struct stat myFile;
			if(stat(foldername, &myFile) < 0)
			{
				gtk_label_set_text(GTK_LABEL(result), "result :\nFail (invalid arguments)");
			}
			else if (!S_ISDIR(myFile.st_mode))
			{
				gtk_label_set_text(GTK_LABEL(result), "result :\nFail (invalid arguments)");
			}
			else
			{
				gtk_label_set_text(GTK_LABEL(result), "result :\nSucess");

				Net net = Load_net("NeuralNet/net.txt");
				Matrix bin = Sauvola(filename);
				splith(bin);
				splitv2(bin);
				Liste dataset = find(bin);
				MatCoor *actuel = dataset.first;
				char *s = malloc(sizeof(char) * (strlen(foldername) + 10));
				sprintf(s, "%s/output.txt", foldername);
				FILE *file = NULL;
				file = fopen(s, "w+");
				if (file == NULL)
					errx(1,"Error in on_start (main.c): file opening failed.");
				double max = 0;
				size_t imax = 0;
				double value;
				Matrix res;
				size_t espacevalue = 0;
				size_t y = actuel->y;
				for(size_t i = 0; Get_elm(&dataset, i).y == y; i++)
				{
					if(((Get_elm(&dataset, i+1).x - Get_elm(&dataset, i).x)- (Get_elm(&dataset, i).mat.width + Get_elm(&dataset, i+1).mat.width)) > espacevalue)
					{
						espacevalue = (Get_elm(&dataset, i+1).x - Get_elm(&dataset, i).x)- (Get_elm(&dataset, i).mat.width + Get_elm(&dataset, i+1).mat.width);
					}
				}
				while (actuel != NULL)
				{
					actuel->mat = redim(actuel->mat,28,28);
					res = Use_net(net, actuel->mat);
					for (size_t j = 0; j < res.width; j++)
					{
						value = Get_nb(res, j, 0);
						if (value > max)
						{
							max = value;
							imax = j;
						}
					}
					if (imax < 26)
					{
						fprintf(file, "%c", (char)('a'+imax));
						if(actuel->next != NULL &&
								actuel->next->x - actuel->x >= espacevalue)
							fprintf(file, " ");
						if(actuel->next != NULL &&
								actuel->next->y != actuel->y)
							fprintf(file, "\n");
					}

					else
					{
						fprintf(file, "%c", (char)('A'+imax%26));
						if(actuel->next != NULL &&
								actuel->next->x - actuel->x >= espacevalue)
							fprintf(file, " ");
						if(actuel->next != NULL &&
								actuel->next->y != actuel->y)
							fprintf(file, "\n");
					}
					max = 0;
					imax = 0;

					actuel = actuel->next;
				}

				free(bin.data);
				//SDL_FreeSurface(img_s);
				fclose(file);
			}
		}
	}
}
// called when window is closed
void on_exit_button()
{
    gtk_main_quit();
}

int main()
{

    gtk_init(NULL, NULL);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "main_window.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, NULL);
    grid1 = GTK_WIDGET(gtk_builder_get_object(builder,"grid1"));
    Openfile = GTK_WIDGET(gtk_builder_get_object(builder,"Openfile"));
    Openfile2 = GTK_WIDGET(gtk_builder_get_object(builder,"Openfile2"));
    Entrypath2 = GTK_WIDGET(gtk_builder_get_object(builder,"Entrypath2"));
    Entrypath = GTK_WIDGET(gtk_builder_get_object(builder,"Entrypath"));
    imagedoc = GTK_WIDGET(gtk_builder_get_object(builder,"imagedoc"));
    Exit = GTK_WIDGET(gtk_builder_get_object(builder,"Exit"));
    /*GdkColor color;
    color.red = 0xf00f;
    color.green = 0;
    color.blue = 0;
    gtk_widget_modify_bg(Exit, GTK_STATE_NORMAL, &color);*/
    Showdoc = GTK_WIDGET(gtk_builder_get_object(builder,"Showdoc"));
    selectedfile = GTK_WIDGET(gtk_builder_get_object(builder,"selectedfile"));
    start_button = GTK_WIDGET(gtk_builder_get_object(builder,"Startbutton"));
    separator1 = GTK_WIDGET(gtk_builder_get_object(builder,"separator1"));
    result = GTK_WIDGET(gtk_builder_get_object(builder,"result"));
    g_signal_connect(G_OBJECT(Exit), "clicked", G_CALLBACK(on_exit_button), NULL);
    g_signal_connect(G_OBJECT(Openfile), "selection-changed", G_CALLBACK(newfileset), &Openfile);
    g_signal_connect(G_OBJECT(Openfile2), "selection-changed", G_CALLBACK(pathset), &Openfile2);
    g_signal_connect(G_OBJECT(Entrypath2), "changed", G_CALLBACK(pathchanged2), &Entrypath2);
    g_signal_connect(G_OBJECT(Entrypath), "changed", G_CALLBACK(pathchanged), &Entrypath);
    g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(on_start), NULL);
    g_signal_connect(G_OBJECT(Showdoc), "clicked", G_CALLBACK(on_show_image), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
