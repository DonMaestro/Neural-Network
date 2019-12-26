#include <iostream>
#include <fstream>
#include "Read_bmp.h"
#include "Neural_Network.h"

//g++ -o run main.cpp NNRealization.cpp AFRealization.cpp Read_bmp.h

void fun(Options *op, Image *im, char *addres);

void read_all_file(Options *op, Image *im, int size, char *addres, int *neurons);

int read_image(Options *op, Image *im, char *addres, int quantity_exit);

int main(int argc, char *argv[])
{
	int rand_im = 0;
	float losses = 1;
	float cache;

	int neurons[4] = {144, 320, 320, 10};
	float *result; 

	NeuralNetwork NN(4, neurons);


	if(argc == 2)
	{
		NN.ReadFileNN();	
		
		std::cout << argv[1] << std::endl;
	
		Options opt;
		Image img;

		if(read_image(&opt, &img, argv[1], neurons[3]))
		{
			return 1;
		}


	
		result = NN.StartNN(opt.input, opt.i_row);
		for(int number = 0; number < neurons[3]; ++number)
			std::cout << "figure [" << number << "]: " << result[number] << std::endl;


		delete_image(&img);
		delete[] opt.input;
	
	}
	else
	{
	
		NN.RandWeight();
		//NN.setNeuronDisplacement(1, 1);

		char addres[11] = { 'n', 'u', 'm', '/', '7', '_', '1', '.', 'b', 'm', 'p' };
		Options *op = new Options[50];
		Image *im = new Image[50];
		
		read_all_file(op, im, 50, addres, neurons);

		for(int num = 0; num < 50; ++num)
		{
			for(int number = 0; number < op[num].i_row; ++number)
				std::cout << op[num].input[number];
			std::cout << std::endl;
		}
	
	
		for(int number = 0; losses > 0.001; ++number)
		{
			//rand_im = rand() % 50;
			cache = losses;
			losses = 0;
			for(int rand_im = 0; rand_im < 50; ++rand_im)
			{	
				std::cout << "::" << number * 50 + rand_im << "::";
				std::cout << "image number: " << rand_im;
		
				losses = losses + NN.LearningNN(&op[rand_im], 0.2, 0.1);
			
				std::cout << "losses " << cache << std::endl;
	
				std::cout << std::endl << std::endl;
			}
		
		}
	
		for(int number = 0; number < 50; ++number)
		{
			delete_image(&im[number]);
			delete[] op[number].input;
			delete[] op[number].answer;
		}
		delete[] im;
		delete[] op;

		//NN.ShowNeuralNetwork();
		//NN.WriteFile();
	
	}






	return 0;
}

int read_image(Options *op, Image *im, char *address, int quantity_exit)
{
	std::ifstream file;

	file.open(address, std::ios::in | std::ios::binary);

	if(!file.is_open())
	{
		std::cout << "ERROR: file does not exist or cannot be opened." << std::endl;
		return 1;
	}


	read_bmp(&file, im);

	for(int y = 0; y < 12; ++y)
	{
		for(int x = 0; x < 12; ++x)
			std::cout << (char)(im->map[y][x] + 47 ) << ' ';
		std::cout << std::endl;
	}

	file.close();


	op->i_row = im->height * im->width;
	op->input = new float[op->i_row];
	op->a_row = quantity_exit;
	
	for(int row = 0; row < im->height; ++row)
		for(int column = 0; column < im->width; ++column)	
			op->input[row * im->width + column] = im->map[row][column];

	return 0;	
}


void read_all_file(Options *op, Image *im, int size, char *addres, int *neurons)
{
	int num_figure = 1;
	int fig_fig = 0;

	std::ifstream file;

	for(int number = 0; number < size; ++number)
	{
		addres[4] = fig_fig + 48; 
		addres[6] = num_figure + 48;

		file.open(addres, std::ios::in | std::ios::binary);

		if(!file.is_open())
		{
			std::cout << "error" << std::endl;
			return;
		}


		read_bmp(&file, &im[number]);
	
		file.close();


		op[number].i_row = im[number].height * im[number].width;
		op[number].input = new float[op[number].i_row];
		op[number].a_row = neurons[3];
		op[number].answer = new float[op[number].a_row];
	
		fun(&op[number], &im[number], addres);
	
	
//		for(int num = 0; num < op[number].i_row; ++num)
//			std::cout << op[number].input[num] << ' ';
//		std::cout << std::endl;


		++num_figure;

		if(num_figure > 5)
		{
			++fig_fig;
			num_figure = 1;
		}
	}

}

void fun(Options *op, Image *im, char *addres)
{

	int ll = 0;
	for(int row = 0; row < im->height; ++row)
		for(int column = 0; column < im->width; ++column)	
		{
			op->input[ll] = im->map[row][column];
			++ll;
		}
	for(int number = 0; number < op->a_row; ++number)
	{
		if(number == addres[4] - 48)
			op->answer[number] = 1;
		else
			op->answer[number] = 0;
	}	
}

