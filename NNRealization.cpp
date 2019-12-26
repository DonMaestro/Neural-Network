#include "Neural_Network.h"

float derivative(float x);


NeuralNetwork::NeuralNetwork()
{
	
}

NeuralNetwork::NeuralNetwork(int number_of_layers, int *number_of_neurons_in_layer)
{
	this->number_of_layers = number_of_layers;
	
	layer = new LayerNeuron[number_of_layers];

	for(int number = 0; number < number_of_layers; ++number)
	{
		layer[number].v_row = number_of_neurons_in_layer[number];
		layer[number].value = new float[layer[number].v_row];
		layer[number].x = new float[layer[number].v_row];
	}

	for(int number = 0; number < number_of_layers-1; ++number)
	{
		layer[number].w_column = number_of_neurons_in_layer[number] + 1;
		layer[number].w_row = number_of_neurons_in_layer[number+1];
		layer[number].weight = new float*[layer[number].w_row];
		for(int row = 0; row < layer[number].w_row; ++row)
			layer[number].weight[row] = new float[layer[number].w_column]; // number of neurons and neuron displacement 
	}
	layer[number_of_layers-1].w_column = 0; 
	layer[number_of_layers-1].w_row = 0;

	//error new memory
	for(int number_l = 0; number_l < number_of_layers; ++number_l)
		layer[number_l].error = new float[layer[number_l].v_row];
}



float *NeuralNetwork::StartNN(float *input, size_t size)
{
	if(layer[0].v_row != size)
	{
		std::cout << "the number of inputs is incorrect" << std::endl;
		return 0;
	}

	for(int number = 0; number < size; ++number)
		layer[0].value[number] = input[number];


	for(int number_l = 0; number_l < number_of_layers-1; ++number_l)
	{
		MultiplicationOfMatrices(layer[number_l], layer[number_l+1].value, layer[number_l+1].x);		
		ActivationFunction_RectifiedLinea(layer[number_l+1]);
	}
	return layer[number_of_layers-1].value;
}


float NeuralNetwork::LearningNN(Options *options, float final_percentage, float training_factor)
{

	StartNN(options->input, options->i_row);

	//cout
	std::cout << std::endl << "answer: ";
	for(int row = 0; row < layer[number_of_layers-1].v_row; ++row)
		std::cout << options->answer[row] << ' ';

	std::cout << std::endl << "   output: ";
	for(int number = 0; number < layer[number_of_layers-1].v_row; ++number)
		std::cout << layer[number_of_layers-1].value[number] << ' ';
	std::cout << std::endl;
	////

/*
	for(int number = 0; number < options->a_row; ++number)
		if(options->answer[number] < (layer[number_of_layers-1].value[number] - final_percentage) && 
				options->answer[number] > layer[number_of_layers-1].value[number] + final_percentage)
		{
			std::cout << "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n";
			return -1;
		}
*/


	

	std::cout << "error out: ";
	for(int row = 0; row < layer[number_of_layers-1].v_row; ++row)
	{
		layer[number_of_layers-1].error[row] = ( options->answer[row] - layer[number_of_layers-1].value[row] );
		std::cout << layer[number_of_layers-1].error[row] << ' ';
	}
	std::cout << std::endl;

	//error calculation
	for(int number_l = number_of_layers-1; number_l > 0; --number_l)
		for(int column = 0; column < layer[number_l-1].w_column-1; ++column)
		{
			layer[number_l-1].error[column] = 0;
			for(int row = 0; row < layer[number_l].v_row; ++row)
				layer[number_l-1].error[column] = layer[number_l-1].error[column] + layer[number_l-1].weight[row][column] * layer[number_l].error[row]; 	
			//std::cout << layer[number_l-1].error[column] << ' ';
		}
	//override w
	for(int number_l = 0; number_l < number_of_layers-1; ++number_l)
		for(int row = 0; row < layer[number_l].w_row; ++row)
			for(int column = 0; column < layer[number_l].w_column-1; ++column)
				layer[number_l].weight[row][column] = layer[number_l].weight[row][column] + training_factor * layer[number_l+1].error[row] * derivative(layer[number_l+1].x[row]) * layer[number_l].value[column];

	float average_error = 0;	
	for(int row = 0; row < layer[number_of_layers-1].v_row; ++row)
		average_error = average_error + layer[number_of_layers-1].error[row] * layer[number_of_layers-1].error[row];

	return average_error;
	//ShowNeuralNetwork();

}

float derivative(float e)
{
	if(e > 0)
		return 1;
	else
		return 0;
}



void NeuralNetwork::ShowNeuralNetwork()
{
	for(int number_l = 0; number_l < number_of_layers; ++number_l)
	{
		std::cout << "layer " << number_l << std::endl;

		std::cout << "v_row " << layer[number_l].v_row << std::endl;
		for(int row = 0; row < layer[number_l].v_row; ++row)
			std::cout << "| " << layer[number_l].value[row] << " | " << layer[number_l].x[row] << " |" << std::endl;

		std::cout << "neuron_displacement " << layer[number_l].neuron_displacement << std::endl;

		std::cout << "w_row " << layer[number_l].w_row << std::endl; 
		std::cout << "w_column " << layer[number_l].w_column << std::endl;
		for(int row = 0; row < layer[number_l].w_row; ++row)
		{
			std::cout << '|';
			for(int column = 0; column < layer[number_l].w_column; ++column)
				std::cout << ' ' << layer[number_l].weight[row][column] << ' ';
			std::cout << '|' << std::endl;
		}
		std::cout << std::endl;
	}

}


void NeuralNetwork::ReadFileNN()
{
	std::ifstream fin("neural_network.nen");
	
	fin >> number_of_layers;

	for(int number_l = 0; number_l < number_of_layers-1; ++number_l)
	{
		fin >> layer[number_l].w_row;
		fin >> layer[number_l].w_column;
		fin >> layer[number_l].neuron_displacement;

		for(int row = 0; row < layer[number_l].w_row; ++row)
			for(int column = 0; column < layer[number_l].w_column; ++column)
				fin >> layer[number_l].weight[row][column];
	}
	fin.close();
}



void NeuralNetwork::WriteFile()
{
	std::ofstream fout("neural_network.nen");
	
	fout << number_of_layers << std::endl;

	for(int number_l = 0; number_l < number_of_layers-1; ++number_l)
	{
		fout << layer[number_l].w_row << ' ';
		fout << layer[number_l].w_column << ' ';
		fout << layer[number_l].neuron_displacement << ' ';

		fout << std::endl;
		for(int row = 0; row < layer[number_l].w_row; ++row)
			for(int column = 0; column < layer[number_l].w_column; ++column)
				fout << layer[number_l].weight[row][column] << ' ';
		fout << std::endl;
	}
	fout.close();
}



NeuralNetwork::NeuralNetwork(const NeuralNetwork &nn)
{
	number_of_layers = nn.number_of_layers;

	layer = new LayerNeuron[number_of_layers]; 

	for(int number = 0; number < number_of_layers; ++number)
	{
		layer[number].w_row = nn.layer[number].w_row;
		layer[number].w_column = nn.layer[number].w_column;
		layer[number].v_row = nn.layer[number].v_row;
		
//allocate memory
		layer[number].weight = new float*[layer[number].w_row];
		for(int row = 0; row < layer[number].w_row; ++row)
			layer[number].weight[row] = new float[layer[number].w_column];

		layer[number].value = new float[layer[number].v_row];
		layer[number].x = new float[layer[number].v_row];
////


		for(int row = 0; row < layer[number].w_row; ++row)
			for(int column = 0; column < layer[number].w_column; ++column)
				layer[number].weight[row][column] = nn.layer[number].weight[row][column];

		for(int row = 0; row < layer[number].v_row; ++row)
		{
			layer[number].value[row] = nn.layer[number].value[row];
			layer[number].x[row] = nn.layer[number].x[row];
		}

		layer[number].neuron_displacement = nn.layer[number].neuron_displacement;
	}
}

NeuralNetwork::~NeuralNetwork()
{
	if(layer == NULL)
	{
		std::cout << "blet" << std::endl;
		return;
	}


	for(int number = 0; number < number_of_layers; ++number)
	{
		delete[] layer[number].value;
		delete[] layer[number].x;
		for(int row = 0; row < layer[number].w_row; ++row)
			delete[] layer[number].weight[row];
		delete[] layer[number].weight;
		delete[] layer[number].error;
	}
	delete[] layer;	
}
