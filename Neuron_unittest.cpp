#include "Neuron.h"
#include "gtest/gtest.h"

/**
 * This test checks whether the membrane potential of the neuron is 
 * computed as it should.
 */
TEST (NeuronTest, MembranePotential)
{
	Neuron neuron;
	neuron.update(1.0, 0);
	EXPECT_EQ(neuron.getPot(), 1.0 * (20.0/1.0) * (1 - exp(-(0.1/20.0))));
}

/**
 * This test checks whether the storeSpike function works correctly, 
 * thus whether it puts the spike in the good position in the buffer.
 */	
TEST (NeuronTest, SpikeStored)
{
	Neuron neuron;
	neuron.storeSpike(17, false); // excitatory spike
	EXPECT_EQ(neuron.getBuffer()[1], 1);
}

/**
 * This test checks whether the sendSpike function works correctly, thus 
 * whether it gives the good information to storeSpike.
 */
TEST (NeuronTest, SpikeSent)
{
	Neuron neuron_1;
	Neuron neuron_2;
	neuron_1.sendSpike(neuron_2, false);
	EXPECT_EQ(neuron_2.getBuffer()[15], 1);
}

/**
 * This test checks whether the delay is indeed 15 steps. That means
 * that a spike sent by a neuron to another one is added to the membrane
 * potential of the other one 15 steps later.
 */ 
TEST (NeuronTest, RightDelay)
{
	Neuron neuron_1;
	Neuron neuron_2;
	neuron_1.sendSpike(neuron_2, false);
	for(size_t i(0); i < 15; ++i) // we update neuron_2 15 times
	{
		neuron_2.update(0.0, 0);
	}
	EXPECT_EQ(neuron_2.getPot(), 0.1);	
}

/**
 * This test checks whether an excitatory spike is indeed a positive
 * value the buffer.
 */
TEST (NeuronTest, ExcitatorySpike)
{
	Neuron neuron_1;
	Neuron neuron_2;
	neuron_1.sendSpike(neuron_2, false);
	EXPECT_EQ(neuron_2.getBuffer()[15], 1);
}

/**
 * This test checks whether an inhibitory spike is indeed a negative 
 * value in the buffer.
 */
TEST (NeuronTest, InhibitorySpike)
{
	Neuron neuron_1;
	Neuron neuron_2;
	neuron_1.sendSpike(neuron_2, true);
	EXPECT_EQ(neuron_2.getBuffer()[15], (-5));
}

/**
 * This test checks whether the hasJustSpiked function works as it 
 * should.
 */
TEST (NeuronTest, hasJustSpiked)
{
	Neuron neuron;
	size_t step(0);
	neuron.update(300.0, 0); // enough to make the membrane potential rise above the treshold potential and make it spike
	step+=1;
	/*
	 * "step += 1" because when a neuron spikes, the spike is written
	 * in the "current step + 1" step, so we have to check if there's 
	 * a spike after one step and not zero.
	 */
	EXPECT_EQ(neuron.hasJustSpiked(step), true);
}


/**
 * This test checks whether the isRefractory function works as it 
 * should. The neuron spikes right away and is then refractory for 20
 * steps.
 */
TEST (NeuronTest, RefractoryPeriod)
{
	/*
	 * step 0 : the membrane potential is computed in the update 
	 * function and exceeds the treshold, the neuron spikes. The neuron
	 * is refractory for 20 steps
	 * step 20 : last step where it's refractory
	 * at the end, step = 21, because it goes one last time through the 
	 * 97th line of code (step+=1)
	 */
	Neuron neuron;
	int step(0);
	while (neuron.hasJustSpiked(step - 1) == false)
	{
		/*
		 * current of 0.3 nA, and no poisson spikes (arbitrary choice)
		 * to make the neuron spike at step 0
		 */
		neuron.update(300.0, 0); 
		step+=1;
	}
	while (neuron.isRefractory(step))
	{
		neuron.update(15.0, 0); // the current shouldn't make any difference here
		step+=1;
	}
	EXPECT_EQ(step, 21);
}


/**
 * This test checks whether a neuron spikes with an input current of 1 pA.
 * It should get closer and closer to the treshold potential, but never
 * reach it. So it checks whether after 2'000 steps the neuron hasn't 
 * spiked once, and that it's very close to spiking.
 */
TEST (NeuronTest, NeverSpiking)
{
	Neuron neuron;
	unsigned int p(0); // it's here to make sure we compare two unsigned integers
	for(size_t i(0); i < 2000; ++i)
	{
		neuron.update(1.0, 0); // poisson only useful when there's a network
	}
	EXPECT_EQ(neuron.getNum(), p); // the neuron hasn't spiked
	EXPECT_NEAR(neuron.getPot(), 20, 0.01); // the potential is almost 20
}


/**
 * This test checks whether a neuron spikes with an input current of 
 * 1.01 pA. It should spike after 924 steps, and thus it checks whether
 * it has spiked after 940.
 */
TEST (NeuronTest, SpikingLowCurrent)
{
	Neuron neuron;
	size_t step(0);
	unsigned int p(1); // it's here to make sure we compare two unsigned integers
	for(size_t i(0); i < 940; ++i)
	{
		neuron.update(1.01, 0);
		step+=1;
	}
	EXPECT_EQ(neuron.getNum(), p);
}
	
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
