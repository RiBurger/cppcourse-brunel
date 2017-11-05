#include "Neuron.h"


/**
 * This constructor initializes the membrane potential and the number
 * of spikes from the values put in the parameters. It also fills the
 * buffer with zeroes.
 * 
 * @param pot : the membrane potential.
 * @param spikes : the number of spikes.
 */
Neuron::Neuron(double pot, unsigned int spikes)
	: membrane_pot(pot), num_spikes(spikes), time_spikes()
	{
		for(size_t i = 0; i < delay_steps + 1; ++i)
		{
			buffer.push_back(0);
		}
	}
 

/**
 * The default constructor initializes the membrane potential and the
 * number of spikes to zero. It also fills the buffer with zeroes.
 */
Neuron::Neuron()
	: membrane_pot(0.0), num_spikes(0), time_spikes()
	{
		for(size_t i = 0; i < delay_steps + 1; ++i)
		{
			buffer.push_back(0);
		}
	}


Neuron::~Neuron() {}


/**
 * @return : the membrane potential.
 */
double Neuron::getPot() const
{
	return membrane_pot;
}


/**
 * @return : the number of spikes.
 */
unsigned int Neuron::getNum() const
{
	return num_spikes;
}


/**
 * @return : the step of a specific spike occurance.
 */
double Neuron::getTimeSpikes(unsigned int spike_number) const
{
	return time_spikes[(spike_number)-1];
} 


/**
 * @return : the whole ring buffer.
 */
std::vector<int> Neuron::getBuffer() const
{
	return buffer;
}


/**
 * This function checks whether the vector of the spikes' times is
 * empty, to avoid a seg fault. It returns true when the last spike
 * occured less than 20 step (refractory period) ago. Otherwise, it
 * returns false.
 * 
 * @param steps : the current steps.
 * 
 * @return true : if the neuron is in a refractory state
 * @return false : if the neuron is not in a refractory state
 */
bool Neuron::isRefractory(int steps)
{
	if(time_spikes.empty())
	{
		/*
		 * To avoid a seg fault by searching a value in a slot
		 * of the vector which doesn't exist
		 */
		return false;
	}
	else if(time_spikes.back() > steps - refractory_period)
	{
		return true;
	}
	return false;
}


/**
 * This function checks whether the vector of the spikes' times is
 * empty, to avoid a seg fault. It returns true when the time of the 
 * last spike of the neuron is equal to the current step of simulation.
 * 
 * @param steps : the current step.
 * 
 * @return true : if the neuron has spiked at the current step.
 * @return false : if the neuron has not spiked at the current step.
 */
bool Neuron::hasJustSpiked(int steps)
{
	if(time_spikes.empty())
	{
		/*
		 * preventing from checking an empty slot of the vector
		 */
		return false;
	}
	
	if(steps == time_spikes.back())
	{
		return true;
	}
	return false;
}


/**
 * This function is the core of the simulation. If the neuron is
 * refractory, the membrane potential comes back to the reset potential.
 * Otherwise, the membrane potential is computed with the formula
 * from the Brunel paper, and with the help of the buffer, and the
 * external spikes computed via a Poisson distribution. Then the buffer's
 * slot is put back to zero. If the treshold has been reached, the time
 * is added to the vector of time_spikes, and one is added to the number
 * of spikes. At the end, one is added to the local step of neuron.
 * 
 * @param current : the external current.
 * @param external_spikes : the spikes from the background noise.
 */
void Neuron::update(double current, int external_spikes)
{
	if(isRefractory(neuron_steps))
	{
		membrane_pot = reset_potential;
	}
	else 
	{
		/*
		 * computation of the membrane potential
		 */
		membrane_pot = c1 * membrane_pot + current * c2 + J_ * ( buffer[(neuron_steps + 1)% (delay_steps + 1)] + external_spikes );
		buffer[neuron_steps % (delay_steps + 1)] = 0; // reset the buffer's slot to zero, because it has been used
		if(membrane_pot > treshold_potential)
		{
			/*
			 * The membrane potential has reached the treshold --> spike
			 * 
			 * Since I've decided to compute the membrane potential at the start of the update function,
			 * when a neuron spikes, the step added to the vector time_spikes is "current step + 1", and
			 * not simply the current step.
			 */
			time_spikes.push_back(neuron_steps + 1); // adds the time (in steps) of spike in the vector
			num_spikes += 1;
		}
	}
	neuron_steps += 1; // the local time of the vector gets 1 step further
}


/**
 * @param steps : the step at which the spike is received.
 * @param is_inhibitory : is true if the spike is inhibtory.
 */
void Neuron::storeSpike(int steps, bool is_inhibitory)
{
	/*
	 * The current number of steps modulo the delay (the size of the buffer)
	 * is the rest from the division of the first by the latter. 
	 */
	 if(is_inhibitory)
	 {
		 buffer[steps % (delay_steps + 1)] += (-5);
	 }
	 else
	 {
		 buffer[steps % (delay_steps + 1)] += 1;
	 }
}


/**
 * @param n : the neuron receiving the spike.
 * @param isInhibitory : is true if the neuron sending the spike is inhibitory
 */
void Neuron::sendSpike(Neuron& n, bool isInhibitory)
{
	n.storeSpike(neuron_steps + delay_steps, isInhibitory);
}



