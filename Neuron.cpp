#include "Neuron.h"
#include <cmath>


Neuron::Neuron(double pot, unsigned int spikes)
	: membrane_pot(pot), num_spikes(spikes), time_spikes()
	{
		for(size_t i = 0; i < delay_steps + 1; ++i)
		{
			buffer.push_back(0);
		}
	}
 
Neuron::Neuron()
	: membrane_pot(0.0), num_spikes(0), time_spikes()
	{
		/*
		 * provisional way to initiate the buffer
		 */
		for(size_t i = 0; i < delay_steps + 1; ++i)
		{
			buffer.push_back(0);
		}
	}

/*
Neuron::Neuron(const Neuron& neuron)
	: MembranePot(neuron.getMembranePot()), NumberSpikes(neuron.getNumberSpikes()), TimeOfSpikes(neuron.getTimeOfSpikes())
	{}
*/	

Neuron::~Neuron() {}

double Neuron::getPot() const
{
	return membrane_pot;
}

unsigned int Neuron::getNum() const
{
	return num_spikes;
}

double Neuron::getTimeSpikes(unsigned int spike_number) const
{
	return time_spikes[(spike_number)-1];
} 

/*
void Neuron::setPot(double pot)
{
	membrane_pot = pot;
}
			
void Neuron::setNum(unsigned int spikes)
{
	num_spikes = spikes;
}

void Neuron::setTimeSpikes(std::vector<double> time)
{
	time_spikes = time;
}
*/

/*
 * not very well coded
 * isRefractory if the last spike was less than 20 steps ago (refractory period)
 */
bool Neuron::isRefractory(int steps)
{
	if(time_spikes.empty())
	{
		/*
		 * to avoid a seg fault by searching a value in a slot
		 * of the vector which doesn't exist
		 */
		return false;
	}
	else if(time_spikes.back() > steps - refractory_period)
	{
		return true;
	}
	else
	{
	return false;
	}
}

bool Neuron::hasJustSpiked(int steps)
{
	if(time_spikes.empty())
	{
		/*
		 * preventinf from checking an empty slot of the vector
		 */
		return false;
	}
	
	if(steps == time_spikes.back()) // check is alright because both are integers
	{
		return true;
	}
	return false;
}

void Neuron::update(double dt, double courant)
{
	if(isRefractory(neuron_steps))
	{
		/*
		 * should be V_reset and not 0
		 */
		membrane_pot = 0.0;
	}
	else 
	{
		/*
		 * computation of the membrane potential
		 */
		membrane_pot = exp(-dt/tau_) * membrane_pot + courant * (tau_ / C_) * (1 - exp(-dt/tau_)) + J_ * buffer[neuron_steps % (delay_steps + 1)];
		buffer[neuron_steps % (delay_steps + 1)] = 0; // reset the buffer's slot to zero, because it has been used
		if(membrane_pot > treshold_potential)
		{
			/*
			 * the membrane potential has reached the treshold --> spike
			 */
			std::cout << "Seuil atteint" << std::endl;
			time_spikes.push_back(neuron_steps + 1); // adds the time (in steps) of spike in the vector
			std::cout << "Time : " << neuron_steps * dt << " ms" << std::endl;
			num_spikes += 1;
		}
	}
	neuron_steps += 1; // the local time of the vector gets 1 step further
}

void Neuron::storeSpike(int steps)
{
	/*
	 * The current number of steps modulo the delay (the size of the buffer)
	 * is the rest from the division of the first by the latter. 
	 */
	buffer[steps % (delay_steps + 1)] += 1;
}

void Neuron::sendSpike(Neuron& n)
{
	n.storeSpike(neuron_steps + delay_steps - 1);
}


