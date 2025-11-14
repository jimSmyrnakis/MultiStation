#include "EdgeDetectionMasks.h"

static float N[3][3] = {
	{-1, 0, 1},
	{-2, 0, 2},
	{-1, 0, 1}
};

static float NW[3][3] = {
	{ 0, 1, 2},
	{-1, 0, 1},
	{-2,-1, 0}
};

static float W[3][3] = {
	{ 1, 2, 1},
	{ 0, 0, 0},
	{-1,-2,-1}
};
static float SW[3][3] = {
	{ 2, 1, 0},
	{ 1, 0,-1},
	{ 0,-1,-2}
};

static float E[3][3] = {
	{-1,-2,-1},
	{ 0, 0, 0},
	{ 1, 2, 1}
};
static float NE[3][3] = {
	{-2,-1, 0},
	{-1, 0, 1},
	{ 0, 1, 2}
};

static float S[3][3] = {
	{ 1, 0,-1},
	{ 2, 0,-2},
	{ 1, 0,-1}
};
static float SE[3][3] = {
	{ 2, 1, 0},
	{ 1, 0, 1},
	{ 0, 1, 2}
};



extern "C" bool robinsonMask(enum MaskType type, struct MaskAttributes* attrs) {
	ASSERT(attrs, "Bad parameter !");

	switch (type) {
		case EDGE_DETECTION_ROBINSON_N:
			attrs->mask = N; break;
		case EDGE_DETECTION_ROBINSON_NW:
			attrs->mask = NW; break;
		case EDGE_DETECTION_ROBINSON_W:
			attrs->mask = W; break;
		case EDGE_DETECTION_ROBINSON_SW:
			attrs->mask = SW; break;
		case EDGE_DETECTION_ROBINSON_S:
			attrs->mask = S; break;
		case EDGE_DETECTION_ROBINSON_NE:
			attrs->mask = NE; break;
		case EDGE_DETECTION_ROBINSON_E:
			attrs->mask = E; break;
		case EDGE_DETECTION_ROBINSON_SE:
			attrs->mask = SE; break;
		default: return false;
	}

	attrs->type = type;
	attrs->height = attrs->width = 3;
	return true;
}
