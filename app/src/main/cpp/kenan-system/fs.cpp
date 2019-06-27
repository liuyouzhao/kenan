#include "defines.h"
#include "fs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "DMath.h"
#include "png.h"
#include "PlatformConfig.h"

#if USE_SKIA_ANDROID
#include "SkImageDecoder.h"
#include "SkImageRef.h"
#include "SkStream.h"
#include "SkTemplates.h"
#endif

#undef LOG_TAG
#define LOG_TAG "FS"

namespace DCanvas {

static unsigned int g_counter_cache = 0;

unsigned int* dataFromFile(const char* filename, int& w, int& h) {
	//FIXME    to changed it to multiple process
#if USE_SKIA_ANDROID
	SkBitmap bitmap;
	int ret = -1, len;
	char* data;
	unsigned int * res;
	FILE* fp = NULL;

	if (access(filename, 0) != 0)
	{
		LOGE("file not exist!");
		return NULL;
	}

	fp = fopen(filename, "r");
	if (!fp)
	{
		LOGE("file error!");
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	data = new char[len+1];
	fread(data, len, 1, fp);
	data[len] = 0;
	fclose(fp);

	res = dataFromstream(data, w, h, len);
	delete[] data;
	return res;
#endif
	return NULL;
}

struct ByteStream {
	char* data;
	int length;
};

void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
		png_size_t byteCountToRead) {
	if (png_ptr->io_ptr == NULL)
		return; // add custom error handling here
	ByteStream& stream = *(ByteStream*) png_ptr->io_ptr;
	byteCountToRead = stream.length;
	//outBytes = stream.data;
	outBytes = (png_bytep)malloc(byteCountToRead * sizeof(unsigned char));
	memcpy(outBytes, stream.data, byteCountToRead * sizeof(unsigned char));

} // end ReadDataFromInputStream()
void error_callback(png_structp png, png_const_charp error_msg) {
	LOGE("PNG ERROR: %s", error_msg);
}
void abort_(const char * s, ...) {
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

unsigned int* read_png_file(char* file_name, int& width, int& height) {
	int x, y;
	png_byte color_type;
	png_byte bit_depth;
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	char header[8]; // 8 is the maximum size that can be checked
	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for reading",
				file_name);
	fread(header, 1, 8, fp);
	if (png_sig_cmp((png_bytep)header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file",
				file_name);

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, error_callback, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	width = info_ptr->width;
	height = info_ptr->height;
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	LOGE("Suceed! %d, %d", width, height);
	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++)
		row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);
	png_read_image(png_ptr, row_pointers);

	// read single row at a time
	LOGE("read_png_file---------------------------------------- 1");
	unsigned int* pixelInt32 = new unsigned int[width * height];
	unsigned int r = 0, g = 0, b = 0, a = 0;

	for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
//		const int rowOffset = rowIdx * width;
//		png_bytep* pRow = row_pointers + rowOffset;
		int byteIndex = 0;
		for (int colIdx = 0; colIdx < width; ++colIdx) {
			const char red = row_pointers[rowIdx][colIdx*4];
			const char green = row_pointers[rowIdx][colIdx*4 + 1];
			const char blue = row_pointers[rowIdx][colIdx*4 + 2];
			const char alpha = row_pointers[rowIdx][colIdx*4 + 3];

			r = 0x000000ff & (unsigned int) red;
			g = (0x000000ff & (unsigned int) green);
			b = (0x000000ff & (unsigned int) blue);
			a = (0x000000ff & (unsigned int) alpha);

			pixelInt32[rowIdx * width + colIdx] = r | (g << 8) | (b << 16)
					| (a << 24);
		}
	}

	fclose(fp);
	return pixelInt32;
}


unsigned int* dataFromstream(const char* data, int &w, int& h, int length) {
#if USE_SKIA_ANDROID
	SkBitmap bitmap;
	SkImageDecoder::Format fmt;
	unsigned int* pixelInt32 = NULL;
	unsigned int r = 0, g = 0, b = 0, a = 0;

	SkMemoryStream stream(data, length, false);

	bool res = SkImageDecoder::DecodeStream(&stream, &bitmap, SkBitmap::kARGB_8888_Config,
			SkImageDecoder::kDecodeBounds_Mode, &fmt);

	if (!res)
	{
		LOGE("decoder file fail!");
		return NULL;
	}
	res = SkImageDecoder::DecodeStream(&stream, &bitmap, SkBitmap::kARGB_8888_Config,
			SkImageDecoder::kDecodePixels_Mode, &fmt);
	if (!res)
	{
		LOGE("decoder file fail!");
		return NULL;
	}

	int channel = bitmap.rowBytes() / bitmap.width();
	if (channel == 2)
	{
		res = SkImageDecoder::DecodeStream(&stream, &bitmap, SkBitmap::kARGB_8888_Config,
				SkImageDecoder::kDecodePixels_Mode, &fmt);
		if (!res)
		{
			LOGE("decoder file fail!");
			return NULL;
		}
		channel = bitmap.rowBytes() / bitmap.width();
	}
	w = bitmap.width();
	h = bitmap.height();
	char *pixl = (char *) bitmap.getPixels();
	pixelInt32 = new unsigned int[w*h];

	for (int j=0; j< h; j++)
	{
		for (int i =0; i < w;++i)
		{

			switch (channel)
			{
				case 0:
				{
					LOGE("decode error");
					return NULL;
				}
				case 1:
				{
					r = g = b = a = 0x000000ff & (unsigned int)pixl[i];
					break;
				}
				case 2:
				{
					LOGE("decode error in 2 channel");
					return NULL;
				}
				case 3:
				{
					r = 0x000000ff & (unsigned int)pixl[3 * i];
					g = 0x000000ff & (unsigned int)pixl[3 * i + 1];
					b = 0x000000ff & (unsigned int)pixl[3 * i + 2];
					a = 0x000000ff;
					break;
				}
				case 4:
				{
					r = 0x000000ff & (unsigned int)pixl[4 * i];
					g = 0x000000ff & (unsigned int)pixl[4 * i + 1];
					b = 0x000000ff & (unsigned int)pixl[4 * i + 2];
					a = 0x000000ff & (unsigned int)pixl[4 * i + 3];
					break;
				}
			}
			pixelInt32[j * w + i] = r | (g << 8) | (b << 16) | (a << 24);
		}
		pixl += bitmap.rowBytes();
	}
	return pixelInt32;
#else
	//// to stream
	LOGE("-------------- 1");
	char path[256] = {0};
	sprintf(path, "%s/x_cache_%d", PlatformConfig::instance()->getDataDir(), g_counter_cache ++);
	FILE* fp = fopen(path, "wb");
	if (!fp) {
		LOGE("file open error: %s", path);
		return NULL;
	}
	LOGE("-------------- 2");
	fwrite(data, length, 1, fp);
	fflush(fp);
	fclose(fp);
	LOGE("-------------- 3");
	int width = 0, height = 0;
	unsigned int* pixelInt32 = read_png_file(path, width, height);
	LOGE("-------------- 4");
	//unsigned int* pixelInt32 = read_png_memory(data, width, height, length);
	w = width;
	h = height;
	return pixelInt32;
#endif
	return NULL;
}

static const int bytes_per_pixel = 2;

unsigned int* convertFromYUV(unsigned char* stream, int width, int height,
		int size) {
	if (size <= 0)
		return NULL;

	int imgSize = width * height;

	if (size != (imgSize >> 1) + imgSize) {
		LOGE("wrong format");
		return NULL;
	}

	unsigned char *pY = stream;
	unsigned char *pUV = stream + imgSize;
	int i, j;
	int nR, nG, nB;
	int nY, nU, nV;
	unsigned int *out = new unsigned int[imgSize];
	//unsigned int *pixelInt32 = new unsigned int[imgSize];
	unsigned int r, g, b, a;
	int offset = 0;
	// YUV 4:2:0
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			nY = *(pY + i * width + j);
			nV = *(pUV + (i / 2) * width + bytes_per_pixel * (j / 2));
			nU = *(pUV + (i / 2) * width + bytes_per_pixel * (j / 2) + 1);

			// Yuv Convert
			nY -= 16;
			nU -= 128;
			nV -= 128;

			if (nY < 0)
				nY = 0;
			nB = (int) (1192 * nY + 2066 * nU);
			nG = (int) (1192 * nY - 833 * nV - 400 * nU);
			nR = (int) (1192 * nY + 1634 * nV);

			nR = min(262143, max(0, nR));
			nG = min(262143, max(0, nG));
			nB = min(262143, max(0, nB));

			nR >>= 10;
			nR &= 0xff;
			nG >>= 10;
			nG &= 0xff;
			nB >>= 10;
			nB &= 0xff;

			r = 0x000000ff & (unsigned int) nR;
			g = 0x000000ff & (unsigned int) nG;
			b = 0x000000ff & (unsigned int) nB;
			a = 0x000000ff;
			out[(width - j - 1) * height + (height - i - 1)] = r + (g << 8)
					+ (b << 16) + (a << 24);
		}
	}
	return out;
}
////////////wrong
unsigned int* convertFromRGB565(char* stream, int w, int h, int len) {
	if (len <= 0)
		return NULL;

	int ImgSize = w * h;

	if (len < ImgSize)
		return NULL;

	unsigned int* pixelInt32 = new unsigned int[ImgSize];
	unsigned short * rgb = (unsigned short*) stream;
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; ++i) {
			uint allbits = (uint) rgb[i * w];
			uint blueHighBits = allbits & 0x001F; // 5 bits of blue in the bottom
			uint greenHighBits = allbits & 0x07E0; // 6 bits of green in the middle
			uint redHighBits = allbits & 0xF800; // 5 bits of red at the top
			pixelInt32[j * w + i] = (redHighBits << 8) | (greenHighBits << 5)
					| (blueHighBits << 3) | 0xFF000000;
		}
	}
	return pixelInt32;
}

} // namespace DCanvas
