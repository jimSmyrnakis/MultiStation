#include "EdgeDetectionMasks.h"

static float ver[3][3] = {
	{-1 , 0 ,  1} ,
	{-2 , 0 ,  2} ,
	{-1 , 0 ,  1}
};

static float hor[3][3] = {
	{-1 , -2 , -1},
	{ 0 ,  0 ,  0},
	{ 1 ,  2 ,  1}
};


extern "C" bool sobelMask(enum MaskType type, struct MaskAttributes* attrs) {
	ASSERT(attrs, "Bad Parameter !");

	switch (type) {
	case EDGE_DETECTION_SOBEL_VERTICAL: attrs->mask = ver; break;
	case EDGE_DETECTION_SOBEL_HORIZONTAL: attrs->mask = hor; break;
	default: return false;
	}

	attrs->type = type;
	attrs->height = attrs->width = 3;
	return true;
}
