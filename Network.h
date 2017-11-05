#ifndef NETWORK_H
#define NETWORK_H

#include "Neuron.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <climits>
#include <array>
#include <random>
	

/**
 * \brief This Network class makes it easier to simulate the states and
 * interactions of a group of neurons. 
 * 
 * Its default constructor creates the neurons, and then functions are 
 * available to link the neurons, and thus making them interact, to add 
 * a current to the Network, and finally to run the whole simulation 
 * (and write the time of spikes in a file to be able to create a plot).
 */
class Network
{
	private :
	
		int a; 															//!< Simulation steps after which the current is active
		int b; 															//!< Simulation steps after which the current is inactive again
		int n = 2000; 													//!< Final number of steps
		int global_step = 0; 											//!< Global time of simulation in steps
		double I_ext = 0.0; 											//!< Current in pA (pico Ampère)
			
		static constexpr int numb_of_neurons = 12500; 						//!< The total number of neurons
		static constexpr int numb_of_excitatory = ((numb_of_neurons*4)/5);	//!< The total number of excitatory neurons
		static constexpr int numb_of_connections = (numb_of_neurons/10);	//!< The number of connections per neuron
		static constexpr int lambda_ = 2;									//!< The variable in the Poisson random distribution
		
		/*
		 * The first 10'000 neurons (= numb_of_excitatory) are excitatory.
		 * Then, all the neurons remaining are inhibitory.
		 */
		std::vector <Neuron> neurons;									//!< The vector containing all our neurons

		/*
		 * The targets vector contains the same number of slots as the number of neurons.
		 * Each one contains a vector of integers randomly chosen, which represents the ID of the vectors it is connected to.
		 */
		std::vector < std::vector <int> > targets;						//!< The vector containing each neuron and its connections
		
		
	public :
	
		Network();														//!< The network default constructor
		void set_current();												//!< Function setting the current and its interval of "action"
		void create_neurons();											//!< Function creating the neurons
		void build_connections();										//!< Function linking randomly the neurons
		void run_simulation();											//!< Function simulating the network 
	
};

#endif
