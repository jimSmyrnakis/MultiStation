#pragma once
#include "Core.hpp"

extern "C" {

	bool image_salt_papper_noise(struct bmpImage* dest, const struct bmpImage* source , float prob);
}