#include "neural_net.h"
#include "save_net.h"
#include "../Misc/list.h"
#include "../Segmentation/bloc.h"
#include "../Binarize/sauvola.h"

int main()
{
	srand(time(NULL));
	//srand(1);
	/*Matrix in = Init_matrix(2, 4);
	Put_nb(&in, 1, 1, 1);
	Put_nb(&in, 1, 0, 2);
	Put_nb(&in, 1, 0, 3);
	Put_nb(&in, 1, 1, 3);*/
	/*Matrix in1 = Init_matrix(2, 1);
	Matrix in2 = Init_matrix(2, 1);
	Put_nb(&in2, 1, 0, 0);
	Matrix in3 = Init_matrix(2, 1);
	Put_nb(&in3, 1, 1, 0);
	Matrix in4 = Init_matrix(2, 1);
	Put_nb(&in4, 1, 0, 0);
	Put_nb(&in4, 1, 1, 0);
	Matrix in[] = {in1, in2, in3, in4};*/

	/*Matrix out = Init_matrix(2,4);
	Put_nb(&out, 1, 1, 0);
	Put_nb(&out, 1, 0, 1);
	Put_nb(&out, 1, 0, 2);
	Put_nb(&out, 1, 1, 3);*/

	/*Matrix in[13];
	char path[] = "DataBase/ .png";
	for (size_t i = 0; i < 13; i++)
	{
		path[9] = ('a' + i);
		in[i] = otsu(path);
		//printf("path: %s\n", path);
		//Print_matrix(in[i]);
	}*/

	printf("Segment database\n");
	//SDL_Surface *img_s = otsu_binarize("DataBase/alphabet.png");
	//Matrix bin = img2Mat(img_s);
	Matrix bin = Sauvola("DataBase/alphabet.png");
	splith(bin);
	splitv2(bin);
	Liste dataset = find(bin);
	MatCoor *actuel = dataset.first;
	while (actuel != NULL)
	{
		free(actuel->mat.data);
		actuel->mat = redim(actuel->mat,28,28);
		actuel = actuel->next;
	}
	printf("Len: %zi\n", dataset.len);
	free(bin.data);
	//SDL_FreeSurface(img_s);

	printf("Build expected\n");
	Matrix out = Init_matrix(26*2, 26*2);
	FILE* file = fopen("DataBase/alphabet.txt", "r");
	if (file == NULL)
		printf("Not ok\n");
	char c;
	size_t i = 0;
	do
	{
		c = fgetc(file);
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			printf("Char in file: %c\n", c);
			Put_nb(&out, 1, i, i);
			i++;
		}
	} while (c != EOF);
	Print_matrix(out,1);
	printf("Build ok\n");
	fclose(file);

	/*Matrix test = Init_matrix(2, 1);
	Put_nb(&test, 1, 0, 0);
	//Put_nb(&test, 1, 1, 0);*/
	/*Matrix test = Init_matrix(in[0].width * in[0].height, 1);
	for (size_t i = 0; i < in[0].height; i++)
	{
		for (size_t j = 0; j < in[0].height; j++)
			Put_nb(&test, Get_nb(in[1], j, i), i * in[0].width + j, 0);
	}*/

	//printf("Tested matrix:\n");
	//Print_matrix(test);

	size_t hlayer[] = {20};
	Net net = Init_net(28*28, 1, hlayer, 52);
	printf("#######################\n");

	//printf("Origin:\n");
	//Print_net(net);

	printf("#######################\n");
	printf("Result on test matrix without training:\n");
	//Matrix result1 = Use_net(net, test);
	Matrix result1;
	double max = 0;
	size_t imax = 0;
	double value;
	Matrix selected;
	for (size_t i = 0; i < dataset.len; i++)
	{
		selected = Get_elm(&dataset, i).mat;
		result1 = Use_net(net, selected);
		for (size_t j = 0; j < result1.width; j++)
		{
			value = Get_nb(result1, j, 0);
			if (value > max)
			{
				max = value;
				imax = j;
			}
		}
		if (imax < 26)
			printf("%c", (char)('a'+imax));
		else
			printf("%c", (char)('Z'+imax%26));
		max = 0;
		imax = 0;
		free(result1.data);
	}
	printf("\n");

	//Print_matrix(result1, 0);

	Train(net.hidden_layers, &net.output_layer, net.nb_hlayer, dataset, out, 400, 0.7);

	printf("#######################\n");

	//printf("New:\n");
	//Print_net(net);

	printf("######################\n");
	printf("Result on test matrix after training:\n");
	//Matrix result2 = Use_net(net, test);
	Matrix result2;
	for (size_t i = 0; i < dataset.len; i++)
	{
		selected = Get_elm(&dataset, i).mat;
		result2 = Use_net(net, selected);
		//Print_matrix(result2, 1);
		for (size_t j = 0; j < result1.width; j++)
		{
			value = Get_nb(result2, j, 0);
			if (value > max)
			{
				max = value;
				imax = j;
			}
		}
		if (imax < 26)
			printf("%c", (char)('a'+imax));
		else
			printf("%c", (char)('A'+imax%26));
		max = 0;
		imax = 0;
		free(result2.data);
	}
	printf("\n");
	//Print_matrix(result2, 0);

	printf("#######################\n");
	char filename[] = "net.txt";
	Save_net(net, filename);
	printf("Serilalization: see %s\n", filename);
	printf("Deserialization:\n");
	Net n = Load_net(filename);
	Print_net(n);
	Delete_net(n);

	bin = Sauvola("DataBase/lorem.png");
	splith(bin);
	splitv2(bin);
	Liste lorem = find(bin);
	MatCoor *actuel1 = lorem.first;
	Matrix res;
	while (actuel1 != NULL)
	{
		free(actuel1->mat.data);
		actuel1->mat = redim(actuel1->mat,28,28);
		res = Use_net(net, actuel1->mat);

		for (size_t j = 0; j < result1.width; j++)
		{
			value = Get_nb(res, j, 0);
			if (value > max)
			{
				max = value;
				imax = j;
			}
		}
		if (imax < 26)
			printf("%c", (char)('a'+imax));
		else
			printf("%c", (char)('A'+imax%26));
		max = 0;
		imax = 0;

		actuel1 = actuel1->next;
	}
	free(bin.data);

	//for (size_t i = 0; i < 13; i++)
	//	free(in[i].data);
	free(out.data);
	//free(test.data);
	//free(result1.data);
	//free(result2.data);
	Delete_net(net);
}
