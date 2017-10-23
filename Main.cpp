#include "Neuron.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <climits>


/*
Les neurones ont l'air d'être réfractaires dans les premières steps :
 --> faire tests pour régler problème
*/

int main()
{
	
	std::vector<Neuron> neurons;
	Neuron neuron;
	neurons.push_back(neuron);
	Neuron neuron1(10.0,0);
	neurons.push_back(neuron1);
	
	double h(0.1); // time interval between two updates (in ms)
	int a; // simulation steps after which the current is active
	int b; // simulation steps after which the current is inactive again
	int n(2000); // final number of steps
	int global_step(0); // global time of simulation in steps
	double I_ext(0.0); // in pA (pico Ampère)
	
	/*
	 * asks the intensity of the current to the user
	 */
	std::cout << "What's the intensity of the external current?" << std::endl;
	while (I_ext <= 0.0) // to make sure the current is positive
	{
		std::cin >> I_ext;
		if ((I_ext <= 0) or (std::cin.fail()))
		{
			/*
			 * makes sure the current is positive AND is a number !
			 */
			std::cout << "The external current intensity must be positive." << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
	
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
	
	/*
	 * opens the stream to write potentials in a txt file
	 */
	std::ofstream neuron_potential;
	neuron_potential.open("Potential.txt");
	
	double I(0.0); // not very useful, can do better
	
	while (global_step < n)
	{
		/*
		 * the current should be nill for the second neuron, but I've not done it yet
		 */
		if((global_step < a) or (global_step > b))
		{
			/*
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
			neurons[i].update(h, I);
			/*
			 * writes the membrane potentials in the txt file
			 */
			neuron_potential << neurons[i].getPot() << " pA at t = " << global_step * h << " ms"<< '\t';
			
			if(i == 0)
			{
				/*
				 * primary (not the best) way to link two neurons
				 */
				if(neurons[i].hasJustSpiked(global_step))
				{
					std::cout << "sendSpike" << std::endl;
					neurons[i].sendSpike(neurons[i+1]);
				}
			}
		}
		/*
		 * every loop adds one step to the current number
		 */
		global_step += 1;
		neuron_potential << std::endl;
	}
	
	neuron_potential.close();
	return 0;
}
