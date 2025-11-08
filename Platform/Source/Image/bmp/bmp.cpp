#include "bmp.h"

static bool initialized = false;
uint8_t default_color_table[1024]; // 256 colors * 4 bytes per color (BGRA)
static void init_table(void) {
	if (initialized) {
		return;
	}
	for (int i = 0; i < 256; i++) {
		default_color_table[i * 4 + 0] = (uint8_t)i; // Blue
		default_color_table[i * 4 + 1] = (uint8_t)i; // Green
		default_color_table[i * 4 + 2] = (uint8_t)i; // Red
		default_color_table[i * 4 + 3] = 0;          // Reserved
	}
	initialized = true;
}

extern "C" bool bmp_load(const char* filename, struct bmpImage* image_out) {
	init_table();
	if (!filename || !image_out) {
		// Handle null pointer error
		return false;
	}
	image_out->pixels = NULL;
	FILE* file = fopen(filename, "rb");
	if (!file) {
		// Handle file open error
		return false;
	}

	struct BMPHeader header;
	size_t read_bytes = fread(&header, 1, sizeof(struct BMPHeader), file);
	if (read_bytes != sizeof(struct BMPHeader) || header.signature != BMP_SIGNATURE) {
		// Handle invalid BMP file error
		fclose(file);
		return false;
	}

	image_out->header = header;

	uint32_t bytesPerPixel = header.bitCount / 8;
	uint32_t rowSize = (header.width * bytesPerPixel + 3) & ~3; // ��������������� ��� ������� ����������� ��� 4
	uint32_t totalBytes = rowSize * header.height;
	image_out->pixels = (uint8_t*)malloc(totalBytes);
	if (!image_out->pixels) {
		// Handle memory allocation error
		fclose(file);
		return false;
	}


	image_out->width = header.width;
	image_out->height = header.height;

	if (header.bitCount == 24) {
		image_out->channels = 3; // RGB
	}
	else if (header.bitCount == 32) {
		image_out->channels = 4; // RGBA
	}
	else if (header.bitCount == 8) {
		image_out->channels = 1; // Grayscale
	}
	else {
		// Unsupported bit count
		free(image_out->pixels);
		fclose(file);
		return false;
	}

	fseek(file, header.dataOffset, SEEK_SET);
	read_bytes = fread(image_out->pixels, 1, totalBytes, file);
	if (read_bytes != totalBytes) {
		// Handle pixel data read error
		free(image_out->pixels);
		fclose(file);
		return false;
	}

	fseek(file, sizeof(struct BMPHeader), SEEK_SET);
	// Read color table if present (for 8-bit images)
	uint32_t colorTableSize = header.dataOffset - sizeof(struct BMPHeader);
	if (colorTableSize > 0) {
		image_out->colorTable = (uint8_t*)malloc(colorTableSize);
		if (!image_out->colorTable) {
			// Handle memory allocation error
			free(image_out->pixels);
			fclose(file);
			return false;
		}
		size_t colorTableBytesRead = fread(image_out->colorTable, 1, colorTableSize, file);
		if (colorTableBytesRead != colorTableSize) {
			// Handle color table read error
			free(image_out->pixels);
			free(image_out->colorTable);
			fclose(file);
			return false;
		}

	}
	else {
		image_out->colorTable = NULL;
	}

	fclose(file);



	return true;
}
extern "C" bool bmp_store(const char* filename, const struct bmpImage* image_in) {
	init_table();
	if (!filename || !image_in || !image_in->pixels) {
		// Handle null pointer error
		return false;
	}

	FILE* file = fopen(filename, "wb");
	if (!file) {
		// Handle file open error
		return false;
	}
	// rewrite from zero
	fseek(file, 0, SEEK_SET);

	//write header
	size_t written_bytes = fwrite(&image_in->header, 1, sizeof(struct BMPHeader), file);
	if (written_bytes != sizeof(struct BMPHeader)) {
		// Handle header write error
		fclose(file);
		return false;
	}

	//write pixel data
	uint32_t bytesPerPixel = image_in->header.bitCount / 8;
	uint32_t rowSize = (image_in->header.width * bytesPerPixel + 3) & ~3; // ��������������� ��� ������� ����������� ��� 4
	uint32_t totalBytes = rowSize * image_in->header.height;
	fseek(file, image_in->header.dataOffset, SEEK_SET);
	written_bytes = fwrite(image_in->pixels, 1, totalBytes, file);

	if (written_bytes != totalBytes) {
		// Handle pixel data write error
		fclose(file);
		return false;
	}

	// write color table if present
	if (image_in->colorTable) {
		uint32_t colorTableSize = image_in->header.dataOffset - sizeof(struct BMPHeader);
		fseek(file, sizeof(struct BMPHeader), SEEK_SET);
		written_bytes = fwrite(image_in->colorTable, 1, colorTableSize, file);
		if (written_bytes != colorTableSize) {
			// Handle color table write error
			fclose(file);
			return false;
		}
	}

	fclose(file);
	return true;
}