#include "Neuron.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <climits>
#include <array>
#include <random>


/**
 * Les neurones ont l'air d'être réfractaires dans les premières steps :
 * --> faire tests pour régler problème
*/


int main()
{
	
	double h(0.1); // time interval between two updates (in ms)
	int a; // simulation steps after which the current is active
	int b; // simulation steps after which the current is inactive again
	int n(2000); // final number of steps
	int global_step(0); // global time of simulation in steps
	double I_ext(0.0); // in pA (pico Ampère)
	
	constexpr int numb_of_neurons(12500);
	constexpr int numb_of_excitatory((numb_of_neurons*4)/5);
	// constexpr int numb_of_inhibitory(numb_of_neurons*(1/5);
	constexpr int numb_of_connections(numb_of_neurons/10);
	// constexpr int excitatory_connections(numb_of_excitatory/10);
	// constexpr int inhibitory_connections(numb_of_inhibitory/10);
	constexpr int lambda_(2);
	
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> random_neuron (0, numb_of_neurons - 1);
	
	
	/**
	std::array<Neuron, numb_of_excitatory> excitatory_neurons; // initialiser inhibitory à false
	std::array<Neuron, numb_of_inhibitory> inhibitory_neurons; // initialiser inhibitory à true
	*/
	
	std::vector <Neuron> neurons(numb_of_neurons);

	std::vector < std::vector <int> > targets(numb_of_neurons, std::vector<int> (numb_of_connections));
	
	/**
	for(int i(0); i < numb_of_neurons; ++i)
	{
		for(int j(0); j < excitatory_connections; ++j)
		{
			targets[i][j] = int random(0, excitatory_neurons);
		}
		
		for(int k(0); k < inhibitory_connections; ++k)
		{
			targets[i][excitatory_connections + k] = int random(0, inhibitory_neurons);
		}
	}
	*/
	
	for(int i(0); i < numb_of_neurons; ++i)
	{
		for(int j(0); j < numb_of_connections; ++j)
		{
			targets[i][j] = random_neuron(gen);
		}
	}
	
	
	/**
	 * asks the intensity of the current to the user
	 */
	std::cout << "What's the intensity of the external current?" << std::endl;
	do
	{
		std::cin >> I_ext;
		if ((I_ext < 0) or (std::cin.fail()))
		{
			/**
			 * makes sure the current is positive AND is a number !
			 */
			std::cout << "The external current intensity must be positive." << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	} while (I_ext < 0.0); // to make sure the current is positive
	
	std::cout << "After how many steps of simulation do you want the current to be active?" << std::endl;
	
	while ((a <= 0) or (a > n)) // must be positive AND smaller than the total number of steps
	{
		std::cin >> a;
		if ((a <= 0) or (a > n) or (std::cin.fail()))
		{
			std::cout << "The value must be positive, and smaller than the total number of steps : " << n << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	} 
	
	
	std::cout << "After how many steps of simulation do you want the current to be inactive again?" << std::endl;
	while ((b < a) or (b > n)) // must be positive AND smaller than the total number of steps
	{
		std::cin >> b;
		if ((b < a) or (b > n) or std::cin.fail())
		{
			std::cout << "The value must be equal or bigger than " << a << ", and smaller than the total number of steps : " << n << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
	
	/**
	 * opens the stream to write potentials in a txt file
	 */
	std::ofstream neuron_potential;
	neuron_potential.open("Potential.txt");
	
	double I(0.0); // not very useful, can do better
	
	std::random_device rd_;
	std::mt19937 gen_(rd_());
	std::poisson_distribution<> poisson_spikes(lambda_);
	
	while (global_step < n)
	{
		/**
		 * the current should be nill for the second neuron, but I've not done it yet
		 */
		if((global_step < a) or (global_step > b))
		{
			/**
			 * the current is nill when not in between a and b
			 */
			I = 0.0;
		}
		else
		{
			I = I_ext;
		}
		
		for (unsigned int i(0); i < neurons.size(); ++i) // checks every neuron
		{
			neurons[i].update(h, I, poisson_spikes(gen_));
			/**
			 * writes the membrane potentials in the txt file
			 */
			neuron_potential << neurons[i].getPot() << '\t'; // << " pA at t = " << global_step * h << " ms"<< '\t';
			
			if(neurons[i].hasJustSpiked(global_step))
			{
				for(int j(0); j < numb_of_connections; ++j)
				{
					if(i < numb_of_excitatory)
					{
						neurons[i].sendSpike(neurons[(targets[i][j])], false);
					}
					else
					{
						neurons[i].sendSpike(neurons[(targets[i][j])], true);
					}
				}
			}
		}
		/**
		 * every loop adds one step to the current number
		 */
		global_step += 1;
		neuron_potential << std::endl;
	}
	
	neuron_potential.close();
	return 0;
}
