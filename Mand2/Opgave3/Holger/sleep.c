#include <stdlib.h>
#include "sleep.h"

/* Random sleep function */
void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand())*wait_time_ms / (float) RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}