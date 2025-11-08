#pragma once
#include "Core.hpp"
extern "C" {
	uint64_t mapp_value(
		uint64_t f,
		uint64_t Lmax, uint64_t Lmin,
		uint64_t Pmax, uint64_t Pmin);

	//bool image_find_minmax(struct bmpImage* image, uint64_t max, uint64_t min);
}
