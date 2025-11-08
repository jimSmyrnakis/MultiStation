#pragma once


#include "Core.hpp"
extern "C" {
	bool Image_rotate(const struct bmpImage* srcImage, struct bmpImage* dstImage, float rads);
}