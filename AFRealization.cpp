// Accounting Functions Realization

#include "Neural_Network.h"
#include <stdio.h>


void NeuralNetwork::MultiplicationOfMatrices(LayerNeuron layer, float *r_matrix, float *x)
{
	int w_column = layer.w_column - 1;


	for(int row = 0; row < layer.w_row; ++row)
	{
		r_matrix[row] = 0;
		for(int column = 0; column < w_column-1; ++column)
			r_matrix[row] = r_matrix[row] + layer.weight[row][column] * layer.value[column];
		x[row] = r_matrix[row];
	}


	if(layer.neuron_displacement)
		for(int row = 0; row < layer.w_row; ++row)
			r_matrix[row] = r_matrix[row] + layer.weight[row][layer.w_column-1];

	return;
}



void NeuralNetwork::RandWeight()
{
	srand(time(0));
	for(int number_l = 0; number_l < number_of_layers; ++number_l)
		for(int row = 0; row < layer[number_l].w_row; ++row)
	        	for(int column = 0; column < layer[number_l].w_column; ++column)
				layer[number_l].weight[row][column] = (float)rand() / RAND_MAX / 150;
}



void NeuralNetwork::ActivationFunction_RectifiedLinea(LayerNeuron layer)
{
	for(int row = 0; row < layer.v_row; ++row)
		if(layer.value[row] <= 0)
			layer.value[row] = 0;
}


