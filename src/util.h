#include <stdlib.h>

#pragma once

// NOTE: Boundaries are inclusive
long random_range(long min, long max) {
	return random() % (max - min + 1) + min;
}
