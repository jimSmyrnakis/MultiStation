#include "EdgeDetectionMasks.h"
// North (N)
static float N[3][3] = {
    { +5, +5, +5 },
    { -3,  0, -3 },
    { -3, -3, -3 }
};

// North-West (NW)
static float NW[3][3] = {
    { +5, +5, -3 },
    { +5,  0, -3 },
    { -3, -3, -3 }
};

// West (W)
static float W[3][3] = {
    { +5, -3, -3 },
    { +5,  0, -3 },
    { +5, -3, -3 }
};

// South-West (SW)
static float SW[3][3] = {
    { -3, -3, -3 },
    { +5,  0, -3 },
    { +5, +5, -3 }
};

// South (S)
static float S[3][3] = {
    { -3, -3, -3 },
    { -3,  0, -3 },
    { +5, +5, +5 }
};

// South-East (SE)
static float SE[3][3] = {
    { -3, -3, -3 },
    { -3,  0, +5 },
    { -3, +5, +5 }
};

// East (E)
static float E[3][3] = {
    { -3, -3, +5 },
    { -3,  0, +5 },
    { -3, -3, +5 }
};

// North-East (NE)
static float NE[3][3] = {
    { -3, +5, +5 },
    { -3,  0, +5 },
    { -3, -3, -3 }
};



extern "C" bool KirschMask(enum MaskType type, struct MaskAttributes* attrs) {
	ASSERT(attrs, "Bad parameter !");

	switch (type) {
		case EDGE_DETECTION_KIRSCH_N:
			attrs->mask = N; break;
		case EDGE_DETECTION_KIRSCH_NW:
			attrs->mask = NW; break;
		case EDGE_DETECTION_KIRSCH_W:
			attrs->mask = W; break;
		case EDGE_DETECTION_KIRSCH_SW:
			attrs->mask = SW; break;
		case EDGE_DETECTION_KIRSCH_S:
			attrs->mask = S; break;
		case EDGE_DETECTION_KIRSCH_NE:
			attrs->mask = NE; break;
		case EDGE_DETECTION_KIRSCH_E:
			attrs->mask = E; break;
		case EDGE_DETECTION_KIRSCH_SE:
			attrs->mask = SE; break;
		default: return false;
	}

	attrs->type = type;
	attrs->height = attrs->width = 3;
	return true;
}
