#pragma once
#include "Core.hpp"



extern "C" bool convert_rgb_to_grayscale_avarage(
	struct bmpImage* dest , 
	const struct bmpImage* source );

extern "C" bool convert_rgb_to_grayscale_luminosity(
	struct bmpImage* dest, 
	const struct bmpImage* source,
	float r_s , float g_s , float b_s);
