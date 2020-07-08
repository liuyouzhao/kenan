//
// Created by hujia on 08/07/20.
//

#include <vector>
#include "Assert.h"
#include "defines.h"
#include "Log.h"
#include "ImageImpl.h"
#include "PlatformConfig.h"
#include "png.h"
#include "Base64.h"

#undef LOG_TAG
#define  LOG_TAG    "ImageImpl"

using namespace DCanvas;

namespace kenan_graphics {
static unsigned int* doLoadPNGFromFile(const char* file_name, int& width, int& height);
/**
 * @Function: temporarily create a native file in default storage for further load.
*/
static unsigned int *solidifyAndRead(const char *data, int length, int &outWidth, int &outHeight) {

    char path[PATH_MAX_LENGTH] = {0};
    sprintf(path, "%s/kenan_cache_tmp", PlatformConfig::instance()->getDataDir());

    FILE* fp = fopen(path, "wb");
    if (!fp) {
        LOGE("file open error: %s", path);
        return NULL;
    }

    fwrite(data, length, 1, fp);
    fflush(fp);
    fclose(fp);

    unsigned int* pixelInt32 = doLoadPNGFromFile(path, outWidth, outHeight);

    remove(path);

    return pixelInt32;
}

/**
 * @Function: Decode base64 and save to file, then load from temp-file and delete the file
*/
unsigned int *loadImageDataFromBase64String(const char* base64, int &outWidth, int &outHeight) {
    std::string strBase64 = std::string(base64);
    return loadImageDataFromBase64String(strBase64, outWidth, outHeight);
}

unsigned int *loadImageDataFromBase64String(std::string strBase64, int &outWidth, int &outHeight) {

    const char *base64Split = "base64,";
    char *base64 = (char*)strBase64.c_str();
    int len = strBase64.length();
    int position = strBase64.find(base64Split) + strlen(base64Split);
    if(position == std::string::npos) {
        return NULL;
    }

    std::vector<char> v;
    if (!base64Decode(base64 + position, len - position, v))
    {
        LOGE("base64Decode faile!");
        return NULL;
    }

    int width = 0, height = 0;
    return solidifyAndRead(&v[0], v.size(), outWidth, outHeight);
}

/**
* @Function: load image from file
* filePath is relative file path inside assets,
*
* for example:
* /your-project/app/src/main/assets/images/character.png
* you must give images/a.png or /images/character.png
*
*/
unsigned int *loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight) {
    int width = 0, height = 0;

    char fullPath[PATH_MAX_LENGTH] = {0};
    sprintf(fullPath, "%s/%s", PlatformConfig::instance()->getAssetsDir(), filePath);

    return doLoadPNGFromFile(fullPath, outWidth, outHeight);
}

unsigned int *loadImageDataFromFile(std::string filePath, int &outWidth, int &outHeight) {
    return loadImageDataFromFile(filePath.c_str(), outWidth, outHeight);
}

static void error_callback(png_structp png, png_const_charp error_msg) {
	LOGE("PNG ERROR: %s", error_msg);
}

static void pixelsCharArr2UInt32Arr(png_bytep *row_pointers, int width, int height, unsigned int *pixelInt32) {
    unsigned int r = 0, g = 0, b = 0, a = 0;

    for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
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
}

/**
 * @Function: load png from file using libpng
*/
__RETURN_ALLOCATED_HEAP__
static unsigned int* doLoadPNGFromFile(const char* file_name, int& width, int& height) {
	int x, y;
	png_byte color_type;
	png_byte bit_depth;
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
	unsigned int* pixelInt32 = NULL;

	char header[8]; // 8 is the maximum size that can be checked
	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp) {
		LOGE("[read_png_file] File %s could not be opened for reading", file_name);
        goto exit;
    }
	fread(header, 1, 8, fp);
	if (png_sig_cmp((png_bytep)header, 0, 8)) {
		LOGE("[read_png_file] File %s is not recognized as a PNG file", file_name);
		goto exit;
    }
	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, error_callback, NULL);

	if (!png_ptr) {
		LOGE("[read_png_file] png_create_read_struct failed");
		goto exit;
    }
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		LOGE("[read_png_file] png_create_info_struct failed");
		goto exit;
    }
	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGE("[read_png_file] Error during init_io");
		goto exit;
    }
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	width = info_ptr->width;
	height = info_ptr->height;
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGE("[read_png_file] Error during read_image");
		goto exit;
    }
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++)
		row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);
	png_read_image(png_ptr, row_pointers);

	// read single row at a time
	pixelInt32 = new unsigned int[width * height];
    pixelsCharArr2UInt32Arr(row_pointers, width, height, pixelInt32);
    LOGD("[read_png_file] finished %s", file_name);
exit:
	fclose(fp);
	return pixelInt32;
}

}