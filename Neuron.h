#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <iostream>
#include <cmath>


/**
 * \brief The Neuron class contains the caracteristics and actions of a
 * single neuron. 
 * 
 * It can be initialized with a default constructor, updated with a 
 * function, and you can obtain some of its attributes' values via 
 * getters.
 */
class Neuron
{
	private :
	
		//--------------------- CONSTANTS ----------------------------//
		
		static constexpr double C_ = 1.0; //!< Capacitance
		static constexpr double tau_ = 20.0; //!< Neuron time constant
		static constexpr int refractory_period = 20; //!< The neuron is refractory during this number of steps
		static constexpr double treshold_potential = 20.0; //!< The potential above which the neuron spikes
		static constexpr double J_ = 0.1; //!< Spike amplitude
		static constexpr double reset_potential = 0.0; //!< The potential at which a neuron which has just spiked comes back
		static constexpr double c1 = exp(-(0.1/tau_)); //!< First constant useful in update
		static constexpr double c2 = (tau_/C_ * (1 - c1)); //!< Second constant useful in update
		static constexpr int delay_steps = 15; /** Delay time between the time a spike is sent by the first neuron
													and the time it is received by the post-synaptic neuron */
		
		//------------------------------------------------------------//
		
		int neuron_steps = 0; //!< Local time of the neuron
		double membrane_pot; //!< The potential of the neuron's membrane
		unsigned int num_spikes; //!< The number of times the neuron has spiked
		std::vector<int> time_spikes; //!< A vector containing each step at which the neuron has spiked
		std::vector<int> buffer; //!< Ring buffer of received spikes
	
	
	public :
	
		Neuron(double pot, unsigned int spikes); //!< Constructor
		Neuron(); //!< Default constructor
		~Neuron(); //!< Destructor
		
		//------------------------ GETTERS ---------------------------//
		
		double getPot() const; //!< Function returning the membrane potential of the vector
		unsigned int getNum() const; //!< Function returning the number of times the neuron has spiked
		double getTimeSpikes(unsigned int spike_number) const; //!< Function returning the time at which a particular spike occured
		std::vector<int> getBuffer() const; //!< Function returning the ring buffer
		
		//------------------------------------------------------------//
		
		bool isRefractory(int steps); //!< Function returning true if the neuron is in a refractory state
		bool hasJustSpiked(int steps); //!< Function returning true if the neuron has just spiked
		void update(double courant, int external_spikes); //!< Function updating the neuron, and adding a step to the local buffer
		void storeSpike(int steps, bool is_inhibitory); //!< Function storing a spike received in the parameters
		void sendSpike(Neuron& neuron, bool isInhibitory); //!< Function sending a spike to another neuron
	
};

#endif
