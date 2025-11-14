#pragma once
#include "Core.hpp"

extern "C" bool image_gamma_transform( struct bmpImage* dest , const struct bmpImage* source, float gamma, float epsilon, float c);