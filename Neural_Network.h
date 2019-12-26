#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H


#include <iostream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <fstream>


struct LayerNeuron
{
	float **weight;
	size_t w_row;
	size_t w_column;

	float *value;
	float *x;
	float *error;
	size_t v_row;

	bool neuron_displacement;
};


struct Options
{
	float *answer;
	int a_row;

	float *input;
	int i_row;	
};


class NeuralNetwork 
{

	LayerNeuron *layer;
	int number_of_layers;

	void ActivationFunction_RectifiedLinea(LayerNeuron layer);
	void MultiplicationOfMatrices(LayerNeuron layer, float *r_matrix, float *x);


public:

	NeuralNetwork();
	NeuralNetwork(int number_of_layers, int *number_of_neurons_in_layer);
	NeuralNetwork(const NeuralNetwork &nn);
	~NeuralNetwork();

	void setNeuronDisplacement(int layer_number, bool value){ layer[layer_number].neuron_displacement = value; }

	float *StartNN(float *input, size_t size);
	float LearningNN(Options *options, float final_percentage, float training_factor);
	void ReadFileNN();
	void ReadFileOptions();
	void WriteFile();
//	void WeightsOfNeurons(float ***weight);
	void ShowNeuralNetwork();
	void RandWeight();
};



#endif
