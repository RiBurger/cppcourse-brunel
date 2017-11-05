#include "Network.h"


/**
 * The default constructor initializes the vector of neurons, and builds
 * the connections.
 */
Network::Network()
: neurons(numb_of_neurons), targets(numb_of_neurons, std::vector<int> (numb_of_connections))
{
	build_connections();
}


/**
 * This function is useful if you want to initialize a current in an
 * interval of steps. It interacts with the user by asking him to chose
 * the values of the current and the two edges of the interval, and 
 * makes sure the values are positive numbers.
 */
void Network::set_current()
{
	
	//-------------- THE USER ENTERS THE CURRENT ---------------------//
	
	std::cout << "What's the intensity of the external current?" << std::endl;
	do
	{
		std::cin >> I_ext;
		if ((I_ext < 0.0) or (std::cin.fail()))
		{
			/*
			 * It makes sure the current is positive AND is a number !
			 */
			std::cout << "The external current intensity must be positive." << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			I_ext = -1;
		}
	} while (I_ext < 0.0); // To make sure the current is positive
	
	
	//--------------------- THE USER ENTERS A ------------------------//
	
	std::cout << "After how many steps of simulation do you want the current to be active?" << std::endl;
	
	while ((a <= 0) or (a > n)) // a must be positive AND smaller than the total number of steps
	{
		std::cin >> a;
		if ((a <= 0) or (a > n) or (std::cin.fail()))
		{
			std::cout << "The value must be positive, and smaller than the total number of steps : " << n << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	} 
	
	
	//--------------------- THE USER ENTERS B ------------------------//
	
	std::cout << "After how many steps of simulation do you want the current to be inactive again?" << std::endl;
	while ((b < a) or (b > n)) // b must be larger or equal to a AND smaller than the total number of steps
	{
		std::cin >> b;
		if ((b < a) or (b > n) or std::cin.fail())
		{
			std::cout << "The value must be equal or bigger than " << a << ", and smaller than the total number of steps : " << n << std::endl;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}


/**
 * This function fills the targets vector with zeroes, and then links
 * every neuron with a random group of other neurons (via their ID,
 * represented as integers in the vector)
 */
void Network::build_connections()
{
	/*
	 * Creation of a random integer distribution, to link the neurons
	 */
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> random_neuron (0, numb_of_neurons - 1);
	
	/*
	 * Filling the targets vector with zeroes
	 */
	for(int i(0); i < numb_of_neurons; ++i)
	{
		std::vector<int> connections;
		for(int j; j < numb_of_connections; ++j)
		{
			connections.push_back(0);
		}
		targets.push_back(connections);
	}
	
	/*
	 * Actually linking the neurons (with their ID)
	 */
	for(int i(0); i < numb_of_neurons; ++i)
	{
		for(int j(0); j < numb_of_connections; ++j)
		{
			targets[i][j] = random_neuron(gen);
		}
	}
}


/**
 * This function runs the whole simulation. From the first step, to the
 * last, the neurons are updated one by one. It also writes the times
 * of spikes, and the ID of the neuron which spiked, in the 
 * NeuronGraph.txt file
 */
void Network::run_simulation()
{
	/* 
	 * Opens the stream to write potentials in a txt file
	 */
	// std::ofstream neuron_potential;
	// neuron_potential.open("Potential.txt");
	
	/*
	 * Creation of the ofstream to write the spikes in "NeuronGraph.txt"
	 */
	std::ofstream neuron_graph;
	neuron_graph.open("NeuronGraph.txt");
	
	/*
	 * Creation of a random Poisson distribution, to mimic the background noise
	 */
	std::random_device rd_;
	std::mt19937 gen_(rd_());
	std::poisson_distribution<> poisson_spikes(lambda_);
	
	double I_(0.0); // I_ is the parameter we're going to put in the update
	
	while (global_step < n)
	{
		if((global_step < a) or (global_step > b))
		{
			/*
			 * the current is nill when not in between a and b
			 */
			I_ = 0.0;
		}
		else
		{
			I_ = I_ext;
		}
		
		
		//------------------ LOOP ON EVERY NEURON --------------------//
		
		
		for (unsigned int i(0); i < numb_of_neurons; ++i)
		{
			neurons[i].update(I_, poisson_spikes(gen_));
			/*
			 * writes the membrane potentials in the txt file
			 */
			// neuron_potential << neurons[i].getPot() << '\t'; // << " pA at t = " << global_step * h << " ms"<< '\t';
			
			if(neurons[i].hasJustSpiked(global_step))
			{
				for(int j(0); j < numb_of_connections; ++j)
				{
					if(i < numb_of_excitatory) 
					{
						/*
						 * If the neuron is excitatory, it sends an excitatory spike
						 */
						neurons[i].sendSpike(neurons[(targets[i][j])], false);
					}
					else
					{
						neurons[i].sendSpike(neurons[(targets[i][j])], true);
					}
				}
				/*
				 * That's where we fill the NeuronGraph.txt file.
				 * At each spike, we write the current step, and the ID of the neuron having just spiked
				 */
				neuron_graph << global_step << '\t' << i << std::endl;
			}
		}
		
		//------------------------------------------------------------//
		
		/*
		 * every loop adds one step to the current number of steps
		 */
		global_step += 1;
		// neuron_potential << std::endl;
	}
	// neuron_potential.close();
	neuron_graph.close(); // Ofstream is closed
}
