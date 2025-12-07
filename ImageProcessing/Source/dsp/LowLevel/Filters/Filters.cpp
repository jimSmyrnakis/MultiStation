#include "Filters.h"

extern "C" {
    bool MedianFilterResolve(enum PBOType type, struct PBOAttributes* attrs) {
        ASSERT(attrs, "Bad Parameter !");

        if (type != PBOT_MEDIAN_FILTER) return false;

        attrs->type = type;
        attrs->PBOO_Func = MedianFilter;
        return true;
    }

    bool MedianFilter(
        struct PixelBufferObject* dest,
        const struct PixelBufferObject* src,
        struct Params* parms
    ) {
        if (!parms || !src || !dest) {
            ASSERT(0, "Bad Arguments !!!");
            return false;
        }

        uint32_t cols = src->width;
        uint32_t rows = src->height;
        uint32_t stride = src->stride;
        uint8_t  channels = src->channels;

        uint8_t* inBuf = (uint8_t*)src->buffer;
        uint8_t* outBuf = (uint8_t*)parms->allocator.memalloc(stride * rows);

        if (!outBuf) {
            ASSERT(0, "No memory !!!");
            return false;
        }

        dest->buffer = outBuf;
        dest->width = cols;
        dest->height = rows;
        dest->stride = stride;
        dest->channels = channels;

        // Take N Parameter , this defines NxN kernel
        uint32_t n = *((uint32_t*)parms->parameters[0]);
        uint32_t half = n / 2;

        // temporary array for each pixel neighborhood
        uint8_t* window = (uint8_t*)parms->allocator.memalloc(n * n);
        ASSERT(window, "No memory for window!");

        // --- MAIN LOOP ---
        for (uint32_t y = 0; y < rows; y++) { 
            // for each image row
            for (uint32_t x = 0; x < cols; x++) { 
                // for each image collum 

                for (uint32_t c = 0; c < channels; c++) {
                    // for each channel of this pixel 
                    
                    // gather pixel neighborhood
                    uint32_t count = 0; // this counter is the 1D index
                    // take a sub-section window and gather all values 
                    // as well sort them in ascending order . We translate 
                    // 2D array index to 2D kartecian index 
                    for (int ky = -((int)half); ky <= (int)half; ky++) {
                        int py = (int)y + ky;
                        // ky= kartecian y
                        // py= pixel y
                        // clamp boundaries
                        if (py < 0) py = 0;
                        if (py >= (int)rows) py = rows - 1;

                        for (int kx = -((int)half); kx <= (int)half; kx++) {
                            int px = (int)x + kx;
                            // kx = kartecian x
                            // px = pixel x
                            // clamp boundaries
                            if (px < 0) px = 0;
                            if (px >= (int)cols) px = cols - 1;

                            uint32_t idx = py * stride + px * channels + c;
                            // stride = number of actual bytes per row
                            // c = current channel 
                            window[count++] = inBuf[idx];
                        }
                    }

                    // sort window (simple bubble sort for clarity; replace with faster if you want)
                    for (uint32_t i = 0; i < count - 1; i++) {
                        for (uint32_t j = 0; j < count - i - 1; j++) {
                            if (window[j] > window[j + 1]) {
                                uint8_t tmp = window[j];
                                window[j] = window[j + 1];
                                window[j + 1] = tmp;
                            }
                        }
                    }

                    // median
                    uint8_t medianVal = window[count / 2];

                    uint32_t destIdx = y * stride + x * channels + c;
                    outBuf[destIdx] = medianVal;
                }
            }
        }

        parms->allocator.memfree(window);

        return true;
    }

    bool ÉmageMedianFilter(
        struct bmpImage* dest,
        const struct bmpImage* source,
        uint32_t N , 
        Allocator allocator) {

        struct PBOAttributes attrs;
        if (!getPBOAttributeMap(PBOT_MEDIAN_FILTER, &attrs)) return false;

        struct Params params;
        params.allocator = allocator;
        params.count = 1;
        params.parameters = (void**)allocator.memalloc(sizeof(void*));
        if (!params.parameters) return false;

        params.parameters[0] = &N;

        bool res = ImageProcess(dest, source, &attrs, &params);


        allocator.memfree(params.parameters);
        return res;
    }

}
