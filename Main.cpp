#include "Network.h"

/**
 * Our simulation replaces the external current by a background noise
 * created by a random Poisson distribution, and thus isn't really
 * needed anymore, so I just put it in commented, so that it can still
 * be used if wanted.
 */
int main()
{
	Network network;
	// network.set_current();
	network.run_simulation();
	
	return 0;
}
	
	
