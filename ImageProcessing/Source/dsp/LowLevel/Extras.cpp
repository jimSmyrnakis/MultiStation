#include "Extras.h"

extern "C" uint64_t mapp_value(
	uint64_t f,
	uint64_t Lmax, uint64_t Lmin,
	uint64_t Pmax, uint64_t Pmin) {

	return ((f - Lmax) / (Lmax - Lmin)) * (Pmax - Pmin) + Pmin;
}